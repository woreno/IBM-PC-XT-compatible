
M_IDE_EXIT_WAIT MACRO
	;M_Countdown cx, 0FFh
ENDM

M_IDE_rdReg MACRO REG_			; registo 8-bit
	mov dx, REG_
	in al, dx 
ENDM
M_IDE_wrReg MACRO REG_, VALUE_	; registo 8-bit
	mov al, VALUE_
	mov dx, REG_
	out dx, al
ENDM
M_IDE_wrAlToReg MACRO REG_		; registo 8-bit
	mov dx, REG_
	out dx, al
ENDM

M_IDE_DATAREAD_TO_AX MACRO
	IF IDE_CHUCKMODE EQ 1
		in ax, dx
	ELSE
		in al, dx
		xchg al, ah
		xor dl, 1
		in al, dx
		xchg al, ah
		xor dl, 1
	ENDIF
ENDM

M_IDE_DATAWRITE_FROM_AX MACRO
	xchg al, ah
	xor dl, 1
	out dx, al
	xchg al, ah
	xor dl, 1
	out dx, al
ENDM

M_INT13_ENTER MACRO
	IF INT13_LOG EQ 1
	push ax					; +6
	push ds					; +4

		mov ax, ss
		cmp ax, STACKSEG		; SE É STACK DA BIOS, SALTAR
		je isAlreadyBIOS

		assume DS:DATA			; SE ESTÁ LIGADO SSTEP, SALTAR
		mov ax, DATA
		mov ds, ax
		mov al, SINGLE_STEP_ON
		or al, al
		jnz isAlreadyBIOS
			
			; imprimir coisas

			push ax			; +2	--------------------
			push bp			; +0
				mov SINGLE_STEP_BP, bp
				mov bp, sp
				mov ax, ss:[bp+4]
				mov SINGLE_STEP_DS, ax
				mov ax, ss:[bp+6]
				mov SINGLE_STEP_AX, ax
				mov ax, ss:[bp+14]
				mov SINGLE_STEP_FLAGS, ax
				mov ax, ss:[bp+12]
				mov SINGLE_STEP_NEXTCS, ax
				mov SINGLE_STEP_CS, ax
				mov ax, ss:[bp+10]
				mov SINGLE_STEP_NEXTIP, ax
				mov SINGLE_STEP_IP, ax
				mov ax, ss
				mov SINGLE_STEP_SS, ax
			pop bp
			pop ax			;       --------------------	
				mov SINGLE_STEP_SP, sp	; SS:SP guardada (SP nao é a do inicio)
				M_ssStackToBIOS
					call int13_debug
				assume DS:DATA
				M_ssRestoreStack
				assume DS:CODE

		isAlreadyBIOS:
	pop ds
	pop ax
	ENDIF
ENDM


;------------------------------------------------------------------------

M_IDE_DISP_SECTOR MACRO 
	IF (BUILD_BIOS_DISPLAY+INT13_DEBUG_VISOR) EQ 2
		mov al, cl
		mov dx, DISP_REG_0
		out dx, al
		shr al, 4
		mov dx, DISP_REG_1
		out dx, al
	ENDIF
ENDM


M_IDE_LEDS_READ MACRO
	IF (BUILD_BIOS_DISPLAY+INT13_DEBUG_VISOR) EQ 2
		push ax
		push dx
			mov dx, LEDS_REG
			mov al, NOT( 1 SHL 7 )
			out dx, al
			xor al, al
			mov dx, DISP_REG_2
			out dx, al
			mov al, 0dh
			mov dx, DISP_REG_3
			out dx, al
		pop dx
		pop ax
	ENDIF
ENDM
M_IDE_LEDS_WRITE MACRO
	IF (BUILD_BIOS_DISPLAY+INT13_DEBUG_VISOR) EQ 2
		push ax
		push dx
			mov dx, LEDS_REG
			mov al, NOT( 1 SHL 6 )
			out dx, al
			xor al, al
			mov dx, DISP_REG_2
			out dx, al
			mov al, 0dh
			mov dx, DISP_REG_3
			out dx, al
		pop dx
		pop ax
	ENDIF
ENDM
M_IDE_LEDS_OFF MACRO
	IF (BUILD_BIOS_DISPLAY+INT13_DEBUG_VISOR) EQ 2
		mov dx, LEDS_REG
		mov al, NOT( 0 )
		out dx, al
		xor al, al
		mov dx, DISP_REG_0
		out dx, al
		mov dx, DISP_REG_1
		out dx, al
		mov dx, DISP_REG_3
		out dx, al
	ENDIF
ENDM

p_hdd_tty_error:

	; nao fatais
	cmp bx, ide_error_busy
	je p_hddtty_skip1
	cmp bx, ide_error_dataready
	je p_hddtty_skip1

		IF INT13_ERRORS_ARE_FATAL EQ 1
			call tty_clear
		ENDIF

	p_hddtty_skip1:
	push si
		call ds2cs
		call tty_crlf
		mov si, offset str_ide_error
		call tty_str
		call tty_ddot
		call tty_sp
		push ax
			mov ax, bx
			call tty_hex8
		pop ax
		call tty_sp
		mov si, offset ide_error_strs
		shl bx, 1
		mov si, ds:[si+bx]	; ptr da string
		call tty_str
	pop si
	call beep

	; nao fatais
	cmp bx, ide_error_busy
	je p_hddtty_skip2
	cmp bx, ide_error_dataready
	je p_hddtty_skip2

	IF INT13_ERRORS_ARE_FATAL EQ 1
		pop bx
		call tty_crlf
		call tty_regs
		hlt_
	ENDIF

	p_hddtty_skip2:
ret

M_IDE_SetError MACRO ERROR_ID_
	push bx
	mov bx, ERROR_ID_
	call p_hdd_tty_error
	pop bx
ENDM

;------------------------------------------------------------------------

tty_regsFromDS:
push ax
	assume DS:DATA
	M_tty_ChgAtt 047h
	M_tty_wrFieldSegReg 'C','S','I','P', SINGLE_STEP_CS, SINGLE_STEP_IP
	M_tty_ChgAtt 007h
	M_tty_wrFieldReg ' ','F',SINGLE_STEP_FLAGS
	M_tty_wrFieldReg 'D','S',SINGLE_STEP_DS
	IF TTY_MODE EQ 0
		call tty_crlf
	ENDIF
	M_tty_wrFieldReg 'S','S',SINGLE_STEP_SS
	M_tty_wrFieldReg 'S','P',SINGLE_STEP_SP
	M_tty_wrFieldReg 'B','P',SINGLE_STEP_BP
	assume DS:CODE
pop ax
	call tty_regs1
ret
;------------------------------------------------------------------------

IF BUILD_SSTEP EQ 1
int13_installBreakpoint:
ASSUME ds:DATA
push ds
push si
	mov ax, DATA
	mov ds, ax
					
	mov si, SINGLE_STEP_NEXTIP
	mov SINGLE_STEP_IP, si
	mov ax, SINGLE_STEP_NEXTCS
	mov SINGLE_STEP_CS, ax
	mov ds, ax			; DS:SI (próxima instrução)
	mov al, ds:[si]		; AL = OPCODE

	mov byte ptr ds:[si], 0CCh	; substituir

	push ax				; guardar AL=OPCODE
		mov ax, DATA	
		mov ds, ax		; mudar DS=DATA
	pop ax
	mov SINGLE_STEP_NEXTOP, al

						; instalar ptr tratadores
	xor ax, ax
	mov ds, ax			; DS=0

	mov ax, offset SINGLE_STEP_CODEIP
	mov word ptr ds:[01h*4], ax
	mov ax, offset SINGLE_STEP_CODEIP + (offset p_breakpoint_intCode - offset p_singleStep_intCode)
	mov word ptr ds:[03h*4], ax
	
	mov ax, DATA
	mov word ptr ds:[01h*4+2], ax
	mov word ptr ds:[03h*4+2], ax

	call checkSingleStepInstall

pop si
pop ds
ASSUME ds:CODE
ret
ENDIF
;------------------------------------------------------------------------

IF INT13_LOG EQ 1
int13_debug:		;  ds=DATA
	assume DS:DATA
	mov ax, SINGLE_STEP_AX
	push ax
		call tty_restoreCur
		mov al, 13h
		call printInt
	pop ax
	
	call tty_sp
	push ax
		mov ax, DATA
		mov ds, ax
		inc int13_accesses
		mov ax, int13_accesses
		call tty_U16 
	pop ax

	call tty_crlf
	call printOpcodes
	call tty_crlf
	call tty_regsFromDS
	call tty_crlf
	call p_hdd_prnCHSfromRegs
	;call makeShortBeep
	wait
	call smallWait

	IF BUILD_SSTEP EQ 1
		push ax
		push ds
			mov ax, DATA
			mov ds, ax
			mov ax, int13_accesses
			cmp ax, INT13_STEP_AT
			jne int13_debug_skip
				mov int13_accesses, 0
				call tty_clear
				call int13_installBreakpoint 
				call longwait
			int13_debug_skip:
		pop ds
		pop ax
	ENDIF
	
	IF INT13_STEP_WAIT EQ 1
		wait
	ENDIF
	;M_SETINT_TRAP
	assume DS:CODE
ret
ENDIF
;------------------------------------------------------------------------

p_hdd_prnCHSfromRegs:
push ax
push cx
push dx

	M_tty_ChgAtt 047h
	push ax
		mov al,'D'
		call tty_char
		mov al,'R'
		call tty_char
		mov al,'V'
		call tty_char
		mov al,'='
		call tty_char
		mov al, dl
		call tty_Hex8
	pop ax
	M_tty_ChgAtt 007h
	call tty_crlf	
	
	cmp ah, 00			; p_hdd_reset
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 01			; p_hdd_status
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 09			; p_hdd_init
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 08			; p_hdd_params
	je p_hdd_prnCHSfRegsSkip

	jmp p_hdd_prnCHSfRegsCont

	p_hdd_prnCHSfRegsSkip:
		pop dx
		pop cx
		pop ax
		ret

	p_hdd_prnCHSfRegsCont:
	;-------------------------
	push cx
		shr cl, 6
		xchg cl, ch
		M_tty_wrFieldChar16 'C', cx
	pop cx
	;-------------------------
	M_tty_wrFieldChar8 'H', dh
	;-------------------------
	and cl, 03Fh
	M_tty_wrFieldChar8 'S', cl
	;-------------------------
	M_tty_wrFieldChar '#'
	call tty_Hex8
	call tty_sp
	xor ah, ah
	call tty_sectsToKB
	IF TTY_MODE EQ 0
		call tty_crlf
	ENDIF
	;-------------------------
	call tty_sp
	mov al, '>'
	call tty_char
	mov al, '>'
	call tty_char
	M_tty_ChgAtt 047h
	mov ax, es
	call tty_Hex16
	mov al, ':'
	call tty_char
	mov ax, bx
	call tty_Hex16
	M_tty_ChgAtt 007h
	call tty_sp
	call tty_sp 
pop dx
pop cx
pop ax
ret
;------------------------------------------------------------------------
p_hdd_fillRegs:
push ax
	IF INT13_DEBUG_CHS EQ 1
		push ax
			M_PPI_TestDip 0
			jnz p_hdd_fillRegsNoPrn
				pop ax
				push ax
				call tty_crlf
				call p_hdd_prnCHSfromRegs
				call longWait
			p_hdd_fillRegsNoPrn:
		pop ax
	ENDIF

	call p_hdd_waitRegs

	; cilindros (C)
	push ax
	push cx
	push dx							; M_IDE_wrReg estraga dx
		shr cl, 6					; e é necessário a seguir
		xchg cl, ch
		M_IDE_wrReg IDE_CIL_H, ch
		M_IDE_wrReg IDE_CIL_L, cl
	pop dx
	pop cx
	pop ax

	; cabeças (H)
	push ax
						; A BIOS permite 8bits para HEAD, mas IDE
						; apenas 4bits
		mov al, 0A0h	; tirar o bit 7 a dl=80h/81h e por no nibble alto 
		shl dl, 4		;  AL=[Ah + dl| dh]
		or al, dl		
		add al, dh
		M_IDE_wrAlToReg IDE_DRIVEHEAD
	
		; setor (S)
		and cl, 03Fh
		M_IDE_wrReg IDE_SECTOR, cl	; cl (baixo)=setor
	pop ax

	; número de setores (#)
	M_IDE_wrAlToReg IDE_SECTCOUNT			; al=nº setores
pop ax
ret
;------------------------------------------------------------------------

IF INT13_XSUMS EQ 1
p_hdd_checkXSUM:
push ax
push cx
push dx
push si
	M_tty_wrFieldChar16 'X', bx
	 
	; (c_*tH_ + h_)*tS_ + (s_-1)

	mov cx, ss:[bp+10]
	xchg cl, ch			; |
	shr ch, 6			; |
	xor dx, dx			; > c_*tH_
	mov ax, 10h			; |
	mul cx				; |
	
	mov dx, ss:[bp+8]	; |
	xchg dl, dh			; > ...+ h_
	xor dh, dh			; |
	add ax, dx			; |

	xor dx, dx			; |
	mov cx, 03Fh		; > *tS_			
	mul cx				; |
			
	mov cx, ss:[bp]		; |
	dec cl				; > + (s_-1)
	xor ch, ch			; |
	add ax, cx			; |
	
						; imprimir LBA
	M_tty_wrFieldChar16 'L', ax
	
	shl ax, 1
	mov si, ax
	cmp si, hdd_xsumsLength
	jae p_hdd_checkXSUM_ok

	mov cx, cs:[offset hdd_xsums + si]
	
	M_tty_wrFieldChar16 '?', cx

	cmp cx, bx
	je p_hdd_checkXSUM_ok
		call errorBeep
		wait
	p_hdd_checkXSUM_ok:
pop si
pop dx
pop cx
pop ax
ret
ENDIF
;------------------------------------------------------------------------

IF INT13_DEBUG_PARAMS EQ 1
p_debugBootPart:
	push bx
	push cx
	push dx
	push es
		M_tty_wrStr "Debug MBR0"
		call tty_crlf
		call tty_sp
		xor cx, cx
		mov es, cx
		mov di, boot_loc+1BEh	
		mov cx, 16
		debug_part:
			mov al, es:[di]
			call tty_Hex8
			call tty_Sp
			inc di
			loop debug_part
		call tty_crlf
		call p_hdd_debugDBT
		call tty_crlf
		M_tty_wrStr "Debug Params"
		mov dl, 080h
		mov ah, 8
		INT 13h
		call tty_regs
		call tty_crlf
		call p_hdd_prnCHSfromRegs
		call tty_crlf
		call tty_saveCur
		wait
		call longwait
	pop es
	pop dx
	pop bx
	pop cx
ret


p_hdd_debugDBT:
	call tty_crlf
	call ds2cs

	M_tty_wrStr "Debug DBT0"
	call tty_crlf
	mov ax, 0			; es:di = 0:41h*4
	mov es, ax
	mov di, 41h*4
	M_tty_wrFieldStr "INT41h"
	mov ax, es:[di+2]
	call tty_Hex16
	mov al, ':'
	call tty_char
	mov ax, es:[di]
	call tty_Hex16

	M_tty_wrFieldStr "="
	mov ax, offset DATA:hdd_DBT0+11
	call tty_Hex16
	
	call tty_crlf

	mov ax, DATA
	mov es, ax
	mov di, offset hdd_DBT0 

	call ds2cs
	
	M_tty_wrFieldStr "Bytes/sec"
	mov cl, es:[di+BIOS_DBT_BYTESPERSECT]
	mov ax, 80h
	shl ax, cl
	call tty_Hex16

	M_tty_wrFieldStr "Sect/cil"
	mov al, es:[di+BIOS_DBT_SECTPERCIL]
	call tty_Hex8

	M_tty_wrFieldStr "MaxCilinders"
	mov ax, es:[di+BIOS_DBT2_CILS]
	call tty_Hex16
	
	M_tty_wrFieldStr "MaxHeads"
	mov al, es:[di+BIOS_DBT2_HEADS]
	call tty_Hex8

	call tty_crlf
	
	M_tty_wrFieldStr "CilStartRedCurr"
	mov ax, es:[di+BIOS_DBT2_RWC_CIL]
	call tty_Hex16
	
	M_tty_wrFieldStr "CilStartPrecomp"
	mov ax, es:[di+BIOS_DBT2_WPRC_CIL]
	call tty_Hex16
	
	M_tty_wrFieldStr "MaxECCBurst"
	mov al, es:[di+BIOS_DBT2_ECC_MAXBURST]
	call tty_Hex8

	M_tty_wrFieldStr "Control"
	mov al, es:[di+BIOS_DBT2_CTRLBYTE]
	call tty_Hex8
ret
ENDIF

;------------------------------------------------------------------------

IF INT13_PEEKBUFFER EQ 1
p_hdd_peek_buffers:
push ax
push ds
push si
push di
	call beep
	wait
	call tty_clear
	push es
	pop ds
	mov si, bx
	mov di, bx
	call tty_buffer
	wait 
	call longWait
	call tty_clear
	add si, 256
	mov di, si
	call tty_buffer
	wait
pop di
pop si
pop ds
pop ax
ret
ENDIF

p_hdd_prnStatus:
	call ds2cs
	call tty_crlf
	M_IDE_rdReg IDE_STATUS
	IF INT13_VERBOSE_STATS EQ 1
		M_tty_wrFieldStr "erro"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "erro indice"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "erro dados"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "dados prontos"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "repos"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "falha escrita"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "pronto"
		call tty_bit
		shr al, 1
		M_tty_wrFieldStr "ocupado"
		call tty_bit
	ELSE
		M_tty_wrFieldStr "status"
		call tty_bin8
	ENDIF
ret


;------------------------------------------------------------------------

IF INT13_TESTHDD EQ 1

M_setCHS MACRO C_,H_,S_,NUM_
	mov ax, 200h+NUM_
	mov cl, (S_) + (((C_) SHR 8) SHL 6)
	mov ch, C_ AND 0FFh
	mov dh, H_
	mov dl, 80h
ENDM
p_hdd_test:
	mov dx, 0
	call p_hdd_test1
	mov dx, 256
	call p_hdd_test1
	wait
ret
HDD_TEST_NUM   EQU 18h
HDD_TEST_BUF1  EQU 500h; 07C00h
HDD_TEST_BUF2  EQU 0C000h
p_hdd_test1:
	call tty_clear
	call tty_saveCur
	mov cx, 7
	push dx				; dx=offset a testar
	p_hdd_test2:
		call tty_restoreCur
		push cx
		mov ax, cx
		call tty_Hex16
		call tty_crlf

		xor ax, ax
		mov es, ax
		M_setCHS 0,8h,27h,HDD_TEST_NUM
		mov bx, HDD_TEST_BUF1
		push ax
			int 13h
		pop ax
			call longWait
			mov bx, HDD_TEST_BUF2
			int 13h

		xor ax,ax 
		mov ds, ax
		mov es, ax
		mov si, HDD_TEST_BUF1
		mov di, HDD_TEST_BUF2
		pop cx		; rest. contador
		pop dx		; rest. offset
		push dx		; pushar offset
		push cx		; pushar contador
		add di, dx
		add si, dx
		call tty_buffer
		wait
		call longWait
			
	pop cx		   ; rest. contador
	loop p_hdd_test2
	pop dx
ret


ENDIF