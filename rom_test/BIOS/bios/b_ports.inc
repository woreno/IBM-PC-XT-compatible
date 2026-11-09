;  1. constantes
;  2. dados
;  3. macros
;  4. p_sys_configSerial
;   4.1 p_sys_prnPort
;   4.2 p_sys_configSerial_test
;  5. p_sys_testParallel
;  6. do_serial_test
;       6.1 do_serial_test_rxhandler
;  7. do_plel_test
;		7.1 plel_testReg
;		7.2 plel_prnReg
;  8. do_joy_test
;  9. do_io_test
; 10. p_rs232_io (nao implementado)
; 11. p_printer_io (nao implementado)
; 12. p_keyb_int (dummy)
; 13. p_services_io 

;----------------------------------------------------------------------
; 1. constantes

	SERIAL_CFG_STOPBITS	EQU 0		; 1bit
	SERIAL_CFG_DATABITS	EQU 3		; 8bits
	SERIAL_CFG_FREDIV	EQU 12		; 9600bps
	SERIAL_CFG_PARITY   EQU 0		; 0

;----------------------------------------------------------------------
; 2. dados

plel_layout:							
	; o desenho
	; [ <pino inicial> <offset da porta i/o> <bits> <FF ou FE> ]
	; _________________________
	; \ 13                  1 / (femea)
	;  \_25_______________14_/

	; pino
	db 13
		; porta  ; bits				; FF:muda registo/FE:dobra a linha
		db 1,    4, 5, 7, 6, 0FFh
		db 0,    7, 6, 5, 4, 3, 2, 1, 0, 0FFh
		db 2,    0, 0FEh
	; pino
	db 25
		; porta  ; bits				; FF:muda registo/FE:dobra a linha
		db 0Fh,  9,9,9,9,9,9,9,9, 0FFh
		db 2,    3, 2, 0FFh
		db 1,    3, 0FFh
		db 2,    0, 0FEh
	db 0

plel_layoutWrite:						; indica se os pinos
	db 0,0,0,0,1,1,1,1,1,1,1,1,1		; 0: leitura/ 1: escrita/ 2: X
	db 2,2,2,2,2,2,2,2,1,1,0,1
plel_str_regData:						; nomes dos registos
	db "data", 0
plel_str_regStatus:
	db "status",0
plel_str_regCtrl:
	db "ctrl",0

plel_str_regDataBits:
	db "  D0","  D1","  D2","  D3","  D4", "  D5","  D6", "  D7"
plel_str_regStatusBits:
	db "  X ","  X ","  X ","/ERR","  ON","/PAP","/ACK","/BSY"
plel_str_regCtrlBits:
	db "STRB"," LFA", "/INI", " DSL", " INT", " SSP", "  X ", "  X "
plel_str_regNoName:
	db "...."
plel_str_regs:
	dw plel_str_regData, plel_str_regStatus, plel_str_regCtrl
plel_str_regsBits:
	dw plel_str_regDataBits, plel_str_regStatusBits, plel_str_regCtrlBits


;----------------------------------------------------------------------
; 3. macros 

M_SERIAL_INREG MACRO PORT_OFFSET_
	push dx
		add dl, PORT_OFFSET_
		in al, dx
	pop dx
ENDM

M_SERIAL_OUTREG MACRO PORT_OFFSET_
	push dx
		add dl, PORT_OFFSET_
		out dx, al
	pop dx
ENDM

M_SERIAL_DEFREG MACRO PORT_OFFSET_, NAME_
LOCAL @@skipPrn1, @@Slabel
	jmp @@skipPrn1
	@@Slabel: 
		db	NAME_, 0
	@@skipPrn1:
	mov si, offset  @@Slabel
	push dx
		add dx, PORT_OFFSET_
		call p_sys_configSerial_test
	pop dx
ENDM

;----------------------------------------------------------------------
; 4. p_sys_configSerial
;	se bp==1 mostra todos os registos
p_sys_configSerial:
	M_push3 bp, cx, dx

	M_IRQ_OFF SERIAL_IRQ
	cli

	call ds2cs

	; configurar porta série
	mov al, 80h					; DLAB=1
	M_SERIAL_DEFREG SERIAL_DATAFMT, "datafmt"
	
		mov al, SERIAL_CFG_FREDIV
		M_SERIAL_DEFREG SERIAL_FREQDIV_LO, "fLow"

		mov al, SERIAL_CFG_FREDIV SHR 8
		M_SERIAL_DEFREG SERIAL_FREQDIV_HI, "fHi"

	mov al, (SERIAL_CFG_PARITY SHL 3) OR (SERIAL_CFG_STOPBITS SHL 2) OR (SERIAL_CFG_DATABITS)
	M_SERIAL_DEFREG SERIAL_DATAFMT, "datafmt"
	
			; /OUT2 = 0
	mov al, 1 SHL 3
	M_SERIAL_DEFREG SERIAL_MODEMCTL, "modemClt"

				; não usar FIFO (por agora)
	push dx
		add dx, SERIAL_FIFOCTL
		xor al, al
		out dx, al					; este registo nao é de leitura!
		;M_SERIAL_DEFREG SERIAL_FIFOCTL, "fifoClt"	; pelo que nao se faz o teste RW
	pop dx

				; se receber interromper
	mov al, 1b		
	M_SERIAL_DEFREG SERIAL_INTCFG, "intCfg"
	
	p_sys_configSerial_nok:
	
	M_pop3 bp, cx, dx
ret

;----------------------------------------------------------------------
; 4.1 p_sys_prnPort

p_sys_prnPort:
	push ax
		call tty_crlf
		call tty_sp
		push ax
			mov ax, dx			; imprimir porta
			call tty_Hex16
			mov al, 'h'
			call tty_char
		pop ax
		call tty_sp
		call tty_str			; nome do registo
		call tty_ddot
		call tty_bin8			; valor AL = escrito
		call tty_sp
		mov al, cl				
		call tty_bin8			; valor BL = lido
	pop ax
ret

;----------------------------------------------------------------------
; 4.2. p_sys_configSerial_test

p_sys_configSerial_test:
	push cx

	out dx, al			; escrever
	mov cl, al			; 
	in al, dx			; ler
	xchg al, cl			; AL=escrito BL=lido
	
						
	cmp bp, 0			; se não imprimir saltar
	je p_sys_configSerial_tSkip
		call p_sys_prnPort	
	p_sys_configSerial_tSkip:

	cmp al, cl			; confirmar se igual
	je p_sys_configSerial_testOK
									; erro!!!

		cmp bp, 0		; se imprimiu saltar
		jne p_sys_configSerial_tSkip2
			call p_sys_prnPort	; imprimir
		p_sys_configSerial_tSkip2:
			
			call tty_sp
			call tty_err
			mov bp, 0FFh

	p_sys_configSerial_testOK:
	pop cx
ret


;----------------------------------------------------------------------
; 5. p_sys_testParallel
  p_sys_testParallel:
  M_push3 ax, cx, si
		mov si, offset plel_str_regData

							; verificar se o registo de dados aguenta
		call ds2cs
		mov cl, 0
		p_sys_testParallel_loop:
			dec cl
			mov al, cl
			out dx, al		; emitir byte
			xor al, al
			in al, dx		; mesmo byte?
			cmp al, cl
			je p_sys_testParallel_cont 
				
				;call p_sys_prnPort
				;call tty_sp
				;call tty_err
				;jz p_sys_testParallel_exit

			p_sys_testParallel_cont:

		or cl, cl
		jnz p_sys_testParallel_loop

		p_sys_testParallel_exit:
  
  M_pop3 ax, cx, si
  ret

 
;----------------------------------------------------------------------
; 6. do_serial_test

do_serial_test:

	mov bp, 1					; mostrar todos os registos
	call p_sys_configSerial

	cli
	xor ax, ax					; definir tratador de interrupção RX
	mov ds, ax
	M_defIRQ SERIAL_IRQ, do_serial_test_rxhandler
	
	mov cx, 16
	loop_dummy:
		M_SERIAL_INREG SERIAL_RXTX
		loop loop_dummy


	M_IRQ_ON KEYB_IRQ				; ativar teclado
	M_IRQ_ON SERIAL_IRQ			; ligar interrupções
	sti	

	M_SERIAL_INREG SERIAL_STATUS ; ler dummy (para soltar interrupções)
	
	
	call tty_crlf
	M_tty_wrStr "'>' indica que pode TX; '*' indica caracter recebido"
	call tty_crlf
	call beep
	call beep

	serial_loop:
		
		push ax  
			M_SERIAL_WAIT_TX  ; esperar que o buffer de TX livre
			mov al,'>'	
			call tty_char
		pop ax

		xor ax, ax
		INT 16h							; esperar tecla
		call tty_char

		M_SERIAL_OUTREG SERIAL_RXTX     ; e enviar byte
		out dx, al
		call beep

	jmp serial_loop
ret

;----------------------------------------------------------------------
; 6.1. do_serial_test_rxhandler

do_serial_test_rxhandler:
		push ax
		push dx
		
		call tty_crlf
		mov al, '*'
		call tty_char

		M_SERIAL_INREG SERIAL_INTCAUSE
		test al, 1				; 0: interrupção a tratar?
		jnz serialHandler_skip	; 1: não, sair

		and al, 6				; motivo de int, é rx?
		cmp al, 4 
		jne serialHandler_skip	

		M_SERIAL_INREG SERIAL_RXTX
		call tty_char

		call beep

		serialHandler_skip:
	
		M_SERIAL_INREG SERIAL_STATUS	; necessário ler para nova interrupção

		mov al, PIC_CMD_EOI
		out PIC0_L0, al
	
		pop dx
		pop ax
	iret
  
;----------------------------------------------------------------------
; 7. do_plel_test


do_plel_test:
	xor bx, bx
	in al, dx
	call tty_crlf
	M_tty_wrStr "1. INT: ON/OFF"
	call tty_crlf
	M_tty_wrStr "2. BIDIRECIONAL ON/OFF"
	call tty_crlf
	M_tty_wrStr "Outra tecla continuar"
	call tty_crlf
	call tty_crlf
	M_tty_wrStr "Ao conetor (femea):"
	call tty_crlf
	call tty_saveCur

	disp_plelst:

	call ds2cs
	call tty_crlf
	call tty_restoreCur
	mov si, offset plel_layout
	mov di, offset plel_layoutWrite

	disp_plel:			 

		lodsb		   ; al= nº do pino
		or al, al
		jz disp_plelst

		mov ch, al     ; ch= nº do pino

		disp_reg:
			lodsb				; al=nº do registo
			mov dx, LPT_BASE
			xor ah, ah
			add dx, ax			; dx=porta
			disp_bits:
				lodsb			; cl=nº do bit
				mov cl, al
				cmp cl, 0FFh		; mudança de registo
				je disp_reg
				cmp cl, 0FEh
				jne disp_bits_show
					call tty_crlf	; dobrar a linha
					call tty_crlf
					call tty_crlf
					call tty_crlf
					call tty_sp
					jmp disp_plel

				disp_bits_show:		
					push ds			
					push si				; imprimir numero do pino
						call ds2data
						inc byte ptr ds:[offset tty_row]
						mov al, ch
						call tty_sp
						call tty_sp
						call tty_U8time
						sub byte ptr ds:[offset tty_col], 4
						inc byte ptr ds:[offset tty_row]
								
											; imprimir nome do pino
						cmp cl, 9
						jne disp_bits_show_next

							mov si, offset plel_str_regNoName
							jmp disp_bits_show_prname

						disp_bits_show_next:

						mov si, dx
						sub si, LPT_BASE
						shl si, 1 
						mov si, cs:[offset plel_str_regsBits+si]
						push cx
							xor ch, ch
							shl cx, 2
							add si, cx
						pop cx
						
						disp_bits_show_prname:
						push ax				
							mov al, cs:[si]
							call tty_char
							mov al, cs:[si+1]
							call tty_char
							mov al, cs:[si+2]
							call tty_char
							mov al, cs:[si+3]
							call tty_char
		 
											; imprimir indicador do pino
							inc byte ptr ds:[offset tty_row]
							sub byte ptr ds:[offset tty_col], 1
							mov al, '^' 
							call tty_char

							sub byte ptr ds:[offset tty_row], 3
							sub byte ptr ds:[offset tty_col], 1

						dec ch					; decrementar nº do pino
						pop ax
					pop si
					pop ds
												; imprimir valor registo
					call plel_testReg
												; apagar indicador do pino
					add byte ptr ds:[offset tty_row], 3
					sub byte ptr ds:[offset tty_col], 1
					call tty_sp
					sub byte ptr ds:[offset tty_row], 3
					add byte ptr ds:[offset tty_col], 1
					call tty_sp
				jmp disp_bits
				
;----------------------------------------------------------------------
; 7.1. plel_testReg
;		dx: porta
;		cl: bit do registo

plel_testReg:
	jmp plel_testReg_start
	
	plel_testReg_end:			
		ret

	plel_testReg_start:				
		mov al, cs:[di]			; AL=0 leitura, 1=escrita, 2=dont care
		inc di
		or al, al
		jz plel_testReg_read
		cmp al, 2
		je plel_testReg_dc
		jmp plel_testReg_write

		plel_testReg_read:
			in al, dx				; se o bit é de leitura
			shr al, cl				; imprimir valor e sair
			call tty_bit
			call plel_prnReg
			call plel_pushKey
			jmp plel_testReg_end
			
		plel_testReg_dc:			; se o bit é dontcare
			mov al, '-'				; imprimir X
			call tty_char
			call plel_prnReg
			call plel_pushKey
			jmp plel_testReg_end

		plel_testReg_write:			; se o bit é de escrita
			xor al, al				; rodar 0/1
			call tty_bit
			out dx, al
			call plel_prnReg
			call plel_pushKey
			push ds
				call ds2data
				dec byte ptr ds:[offset tty_col]
			pop ds
			mov al, 1
			call tty_bit
			shl al, cl
			out dx, al
			call plel_prnReg
			call plel_pushKey
			jmp plel_testReg_end
		
		plel_pushKey:
			push ax
			call beep
			xor ax, ax
			INT 16h
			call smallWait
			pop ax
		ret

;----------------------------------------------------------------------
; 7.2. plel_prnReg

plel_prnReg:
	push ds
	push si
		call ds2data
		mov al, byte ptr ds:[offset tty_row]
		push ax
		mov al, byte ptr ds:[offset tty_col]
		push ax
			mov al, 10
			mov byte ptr ds:[offset tty_row], al
			xor al, al
			mov byte ptr ds:[offset tty_col], al
			push ax
				mov ax, 3
				call tty_clearlines
			pop ax
			mov si, dx				; imprimir nome do registo
			sub si, LPT_BASE		
			cmp si, 0Fh			; tem registo?
			je plel_prnReg_skip

			shl si, 1
			call ds2cs
			mov si, cs:[offset plel_str_regs + si]
			call tty_str
			push ax					; imprimir porta do registo
				call tty_sp
				mov ax, dx
				call tty_Hex16
			pop ax

			call tty_ddot			; ler registo e valor
			in al, dx
			call tty_bin8
			mov al, byte ptr ds:[offset tty_col]
			call tty_crlf
			sub al, cl				; imprimir indicador do bit
			dec al
			mov byte ptr ds:[offset tty_col], al
			mov al, '^'
			call tty_char

		plel_prnReg_skip:
		call ds2data
		pop ax
		mov byte ptr ds:[offset tty_col], al
		pop ax
		mov byte ptr ds:[offset tty_row], al
	pop si
	pop ds
ret
  
;----------------------------------------------------------------------
; 8. do_joy_test

do_joy_test: 
	call tty_crlf
	call tty_saveCur
	call ds2cs
	
	mov cx, UNIT_IN_SEGS_16K
	mov es, cx

	do_joy_test_loop:
	
		call tty_restoreCur

		mov word ptr es:[0], 0
		mov word ptr es:[2], 0
		mov word ptr es:[4], 0
		mov word ptr es:[6], 0

		mov dx, JOY_RESET		; reset aos timers
		out dx, al				
		mov dx, JOY_READ
		mov cx, 50				
								; registar quantas
								; iterações cx demorou
								; o sinal em alto
		do_joy_test_loop1: 
			in al, dx
			xor ah, ah
			and al, 0Fh
			push ax
				and al, 1
				add word ptr es:[0], ax
			pop ax
			push ax
				shr al, 1
				and al, 1
				add word ptr es:[2], ax
			pop ax
			push ax
				shr al, 2
				and al, 1
				add word ptr es:[4], ax
			pop ax
				shr al, 3
				and al, 1
				add word ptr es:[6], ax
		loop do_joy_test_loop1

	
		M_tty_wrStr "DX1:"
		mov ax, word ptr es:[0]
		call tty_U16
		call tty_crlf
		M_tty_wrStr "DY1:"
		mov ax, word ptr es:[2]
		call tty_U16
		call tty_crlf
		M_tty_wrStr "DX2:"
		mov ax, word ptr es:[4]
		call tty_U16
		call tty_crlf
		M_tty_wrStr "DY2:"
		mov ax, word ptr es:[6]
		call tty_U16
		call tty_crlf
		in al, dx
		M_tty_wrStr "Botao 3/2/1:"
		shr al, 4
		call tty_bit
		shr al, 1
		call tty_bit
		shr al, 1
		call tty_bit

	jmp do_joy_test_loop


	jmp do_joy_test
	
  
;----------------------------------------------------------------------
; 9. do_io_test

do_io_test:
	sti
	M_IRQ_ON KEYB_IRQ
	call tty_crlf
	mov cx, UNIT_IN_SEGS_16K
	mov es, cx
	xor di, di

	M_tty_wrStr "*** ESC: novo endereco, ENTER: repetir"
	
	call tty_crlf
	M_tty_wrStr "Porta:"
	call tty_inputHex
	mov dx, ax
	call tty_sp
	call tty_Hex16
		
	call tty_crlf
	M_tty_wrStr "Lido:"
	in al, dx
	call tty_Hex8

	do_io_test_values:
	call tty_crlf
	call tty_crlf
	M_tty_wrStr "A escrever:"
	call tty_inputHex
	call tty_sp
	call tty_Hex8
		
	out dx, al

	call tty_crlf
	M_tty_wrStr "Lido:"
	in al, dx
	call tty_Hex8
	call tty_crlf
		
	call beep
	xor ax, ax
	INT 16h
	cmp ah, KEY_SCANCODE_ESC
	jne do_io_test_values
		
	call beep
	jmp do_io_test


;----------------------------------------------------------------------
; 10. p_rs232_io

IF BUILD_BIOS_SERIALINT EQ 0
	p_rs232_io:
		M_ENTER_INT 14h, "SERIAL"
		xor ah, ah
	iret
ELSE
A1	LABEL	WORD			; TABLE OF INIT VALUE
	DW	1047			; 110 BAUD
	DW	768			; 150
	DW	384			; 300
	DW	192			; 600
	DW	96			; 1200
	DW	48			; 2400
	DW	24			; 4800
	DW	12			; 9600

p_rs232_io:
assume CS:CODE
assume DS:DATA
;----- VECTOR TO APPROPRIATE ROUTINE

	STI				; INTERRUPTS BACK ON
	PUSH	DS			; SAVE SEGMENT
	PUSH	DX
	PUSH	SI
	PUSH	DI
	PUSH	CX
	PUSH	BX
	MOV	SI,DX			; RS232 VALUE TO SI
	MOV	DI,DX
	SHL	SI,1			; WORD OFFSET
	CALL	ds2data
	MOV	DX, RS232_BASE[SI]	; GET BASE ADDRESS
	OR	DX,DX			; TEST FOR 0 BASE ADDRESS
	JZ	A3			; RETURN
	OR	AH,AH			; TEST FOR (AH)=0
	JZ	A4			; COMMUN INIT
	DEC	AH			; TEST FOR (AH)=1
	JZ	A5			; SEND AL
	DEC	AH			; TEST FOR (AH)=2
	JZ	A12			; RECEIVE INTO AL
A2:
	DEC	AH			; TEST FOR (AH)=3
	JNZ	A3
	JMP	A18			; COMMUNICATION STATUS
A3:					; RETURN FROM RS232
	POP	BX
	POP	CX
	POP	DI
	POP	SI
	POP	DX
	POP	DS
	IRET				; RETURN TO CALLER, NO ACTION

;----- INITIALIZE THE COMMUNICATIONS PORT

A4:
	MOV	AH,AL			; SAVE INIT PARMS IN AH
	ADD	DX,3			; POINT TO 8250 CONTROL REGISTER
	MOV	AL,80H
	OUT	DX,AL			; SET DLAB=1

;----- DETERMINE BAUD RATE DIVISOR

	MOV	DL,AH			; GET PARMS TO DL
	MOV	CL,4
	ROL	DL,CL
	AND	DX,0EH			; ISOLATE THEM
	MOV	DI,OFFSET A1		; BASE OF TABLE
	ADD	DI,DX			; PUT INTO INDEX REGISTER
	MOV	DX,RS232_BASE[SI]	; POINT TO HIGH ORDER OF DIVISOR
	INC	DX
	MOV	AL,CS:[DI]+1		; GET HIGH ORDER OF DIVISOR
	OUT	DX,AL			; SET MS OF DIV TO 0
	DEC	DX
	MOV	AL,CS:[DI]		; GET LOW ORDER OF DIVISOR
	OUT	DX,AL			; SET LOW OF DIVISOR
	ADD	DX,3
	MOV	AL,AH			; GET PARMS BACK
	AND	AL,01FH 		; STRIP OFF THE BAUD BITS
	OUT	DX,AL			; LINE CONTROL TO 8 BITS
	DEC	DX
	DEC	DX
	MOV	AL,0
	OUT	DX,AL			; INTERRUPT ENABLES ALL OFF
	JMP	SHORT A18		; COM_STATUS

;----- SEND CHARACTER IN (AL) OVER COMMO LINE

A5:
	PUSH	AX			; SAVE CHAR TO SEND
	ADD	DX,4			; MODEM CONTROL REGISTER
	MOV	AL,3			; DTR AND RTS
	OUT	DX,AL			; DATA TERMINAL READY, REQUEST TO SEND
	INC	DX				; MODEM STATUS REGISTER
	INC	DX
	MOV	BH,30H			; DATA SET READY & CLEAR TO SEND
	CALL	WAIT_FOR_STATUS 	; ARE BOTH TRUE
	JE	A9			; YES, READY TO TRANSMIT CHAR
A7:
	POP	CX
	MOV	AL,CL			; RELOAD DATA BYTE
A8:
	OR	AH,80H			; INDICATE TIME OUT
	JMP	A3			; RETURN
A9:					; CLEAR_TO_SEND
	DEC	DX			; LINE STATUS REGISTER
A10:					; WAIT_SEND
	MOV	BH,20H			; IS TRANSMITTER READY
	CALL	WAIT_FOR_STATUS 	; TEST FOR TRANSMITTER READY
	JNZ	A7			; RETURN WITH TIME OUT SET
A11:					; OUT_CHAR
	SUB	DX,5			; DATA PORT
	POP	CX			; RECOVER IN CX TEMPORARILY
	MOV	AL,CL			; MOVE CHAR TO AL FOR OUT, STATUS IN AH
	OUT	DX,AL			; OUTPUT CHARACTER
	JMP	A3			; RETURN

;----- RECEIVE CHARACTER FROM COMMO LINE

A12:
	ADD	DX,4			; MODEM CONTROL REGISTER
	MOV	AL,1			; DATA TERMINAL READY
	OUT	DX,AL
	INC	DX			; MODEM STATUS REGISTER
	INC	DX
A13:					; WAIT_DSR
	MOV	BH,20H			; DATA SET READY
	CALL	WAIT_FOR_STATUS 	; TEST FOR DSR
	JNZ	A8			; RETURN WITH ERROR
A15:					; WAIT_DSR_END
	DEC	DX			; LINE STATUS REGISTER
A16:					; WAIT_RECV
	MOV	BH,1			; RECEIVE BUFFER FULL
	CALL	WAIT_FOR_STATUS 	; TEST FOR REC. BUFF. FULL
	JNZ	A8			; SET TIME OUT ERROR
A17:					; GET_CHAR
	AND	AH,00011110B		; TEST FOR ERROR CONDITIONS ON RECV CHAR
	MOV	DX,RS232_BASE[SI]	; DATA PORT
	IN	AL,DX			; GET CHARACTER FROM LINE
	JMP	A3			; RETURN

;----- COMMO PORT STATUS ROUTINE

A18:
	MOV	DX,RS232_BASE[SI]
	ADD	DX,5			; CONTROL PORT
	IN	AL,DX			; GET LINE CONTROL STATUS
	MOV	AH,AL			; PUT IN AH FOR RETURN
	INC	DX			; POINT TO MODEM STATUS REGISTER
	IN	AL,DX			; GET MODEM CONTROL STATUS
	JMP	A3			; RETURN
;----------------------------------------
; WAIT FOR STATUS ROUTINE		:
;					:
; ENTRY:				:
;	BH=STATUS BIT(S) TO LOOK FOR,	:
;	DX=ADDR. OF STATUS REG		:
; EXIT: 				:
;	ZERO FLAG ON  = STATUS FOUND	:
;	ZERO FLAG OFF = TIMEOUT.	:
;	AH=LAST STATUS READ		:
;----------------------------------------
WAIT_FOR_STATUS:
	MOV	BL,RS232_TIM_OUT[DI]	; LOAD OUTER LOOP COUNT
WFS0:
	SUB	CX,CX
WFS1:
	IN	AL,DX			; GET STATUS
	MOV	AH,AL			; MOVE TO AH
	AND	AL,BH			; ISOLATE BITS TO TEST
	CMP	AL,BH			; EXACTLY = TO MASK
	JE	WFS_END 		; RETURN WITH ZERO FLAG ON
	LOOP	WFS1			; TRY AGAIN
	DEC	BL
	JNZ	WFS0

	OR	BH,BH			; SET ZERO FLAG OFF
WFS_END:
	RET
ENDIF
;----------------------------------------------------------------------
; 11. p_printer_io
IF BUILD_BIOS_PLELINT EQ 0
	p_printer_io:
		M_ENTER_INT 17h, "LPT"
		xor ah, ah
	iret
ELSE


ENDIF
 
;----------------------------------------------------------------------
; 12. p_keyb_int
IF BUILD_BIOS_KEYB EQ 0
ASSUME DS:DATA
p_keyb_int:
	iret
		
	;DUMMY_KEY EQU 3920h ; SPACE
	DUMMY_KEY EQU 0B920h ; SPACE

	p_keyb_io:
		cmp ah, 0
		je p_keys_waitstroke
		cmp ah, 1
		je p_keys_getstroke
		cmp ah, 2
		je p_keys_getshift
		iret

		p_keys_waitstroke:
			call makeShortBeep
			wait
			call smallWait
			push ds
				call ds2data
				mov kb_buffer, 1
			pop ds
			mov ax, DUMMY_KEY
			iret

		p_keys_getstroke:
			push ds
				call ds2data
				mov ax, kb_buffer
			pop ds
			or ax, ax
			jz p_keys_nokey

				M_x86_disableFlags x86_ZF
				mov ax, DUMMY_KEY
				iret

			p_keys_nokey:
				M_x86_enableFlags x86_ZF
				iret

		p_keys_getshift:
			xor ax, ax	; sem teclas especiais
			iret			
ASSUME DS:CODE
ENDIF


;----------------------------------------------------------------------
; 13. p_services_io
p_services_io:
	M_ENTER_INT 15h, "SERV"
	stc
	mov ah, 86h			; XT 
	M_retFarWithout 2

