;-------------------------------------------------------------
;- Macros
;-------------------------------------------------------------
 
;  1. PPI (programable peripheral interface)
;  2. Contadores 
;  3. Visor de Digitos e LEDs
;  4. Som
;  5. Interface de teste
;  6. Interrupções
;  7. Stack
;  8. Debug (com STACK)
;  9. Utilitários
; 10. Saltos e chamadas
; 11. Operações com registos (M_MinusAX usa STACK)
; 12. Conversões
; 13. CGA (sem STACK)
; 14. Impressão no ecrã  (sem STACK)
; 15. Impressão no ecrã (com STACK)
; 16. Teclado
; 17. RTC (com STACK se RTC_DEBUG)
; 18. Porta Série


;------------------------------------------------------------------------------
; 1. PPI (programable peripheral interface)

M_PPI_config MACRO
 mov al, 099h
 out PPI_REG_CTRL, al 
ENDM

M_PPI_showDip1 MACRO
	in  al, PPI_REG_OPTS  
	or al, 08h
	out PPI_REG_OPTS, al
	nop
ENDM
M_PPI_showDip0 MACRO
	in  al, PPI_REG_OPTS  
	and al, 0F7h
	out PPI_REG_OPTS, al
	nop
ENDM

M_PPI_TestDip MACRO BIT_
	in  al, PPI_REG_INFO  
	not al
	and al, 1 SHL BIT_ 
ENDM

;------------------------------------------------------------------------------
; 2. Contadores 

M_Countdown2 MACRO REG2_, REG_, CNT_
	LOCAL @@loop, @@loop2
	mov REG2_, CNT_
	@@loop2:
		mov REG_, 0h
		@@loop:
			dec REG_
			jnz @@loop
	dec REG2_
	jnz @@loop2
ENDM
M_Countdown MACRO REG_, CNT_
	LOCAL @@loop
	IFB <CNT_>
		mov REG_, 0FFFFh
	ELSE
		mov REG_, CNT_
	ENDIF
	@@loop:
		dec REG_
		jnz @@loop
ENDM


;------------------------------------------------------------------------------
; 3. Visor de Digitos e LEDs

M_CHECKPOINT MACRO NUM_
;	mov al, NOT( 1 SHL NUM_ )
;	mov dx, LEDS_REG
ENDM

M_OpenLeds MACRO 
	IF BUILD_BIOS_DISPLAY EQ 1
	mov dx, LEDS_REG
	mov al, 0FFh
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 00011000b )
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 00111100b )
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 01111110b )
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 11111111b )
	out dx, al
	M_Countdown ax, 0FFFFh 
	mov al, NOT( 00111100b )
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 00111100b )
	out dx, al
	M_Countdown ax, 03FFFh
	mov al, NOT( 00011000b )
	out dx, al
	M_Countdown ax, 02FFFh
	mov al, NOT( 00000000b )
	out dx, al
	M_Countdown ax, 0FFFFh
	ENDIF
ENDM
M_DispLed MACRO VAL_
	IF BUILD_BIOS_DISPLAY EQ 1
	mov al, 1
	IF VAL_ NE 0
		shl al, VAL_
	ENDIF
	not al
	mov dx, LEDS_REG
	out dx, al
	ENDIF
ENDM
M_DispLeds MACRO VAL_
	IF BUILD_BIOS_DISPLAY EQ 1
	mov al, VAL_
	not al
	mov dx, LEDS_REG
	out dx, al
	ENDIF
ENDM

		; BKREG_ nao estraga AX
		; Nao usar os mesmos registos VAL_=!BKREG_
M_DispDigits16 MACRO VAL_, BKREG_
	IF BUILD_BIOS_DISPLAY EQ 1
	mov BKREG_, ax
	mov ax, VAL_
	mov dx, DISP_REG_0
	out dx, al
	mov dx, DISP_REG_1
	shr al, 4
	out dx, al
	mov dx, DISP_REG_2
	xchg al, ah
	out dx, al
	mov dx, DISP_REG_3
	shr al, 4
	out dx, al
	mov ax, BKREG_
	ENDIF
ENDM
M_DispAddress MACRO SEG_, OFF_, BKREG_
	IF BUILD_BIOS_DISPLAY EQ 1
	M_DispDigits16 SEG_, BKREG_
	M_CountDown BKREG_, 0FFFFh
	M_DispDigits16 OFF_, BKREG_
	M_CountDown BKREG_, 0FFFFh
	ENDIF
ENDM

M_DispDigits MACRO A3_, A2_, A1_, A0_
	IF BUILD_BIOS_DISPLAY EQ 1
	mov dx, DISP_REG_0
	mov al, A0_
	out dx, al
	mov dx, DISP_REG_1
	mov al, A1_
	out dx, al
	mov dx, DISP_REG_2
	mov al, A2_
	out dx, al
	mov dx, DISP_REG_3
	mov al, A3_
	out dx, al
	ENDIF
ENDM

M_BlinkeLeds MACRO 
	IF BUILD_BIOS_DISPLAY EQ 1
	mov al, 0FFh
	mov dx, LEDS_REG
	out dx, al
	M_Countdown dx, 02FFFh
	mov al, 0
	mov dx, LEDS_REG
	out dx, al
	M_Countdown dx, 0FFFFh
	mov al, 0FFh
	mov dx, LEDS_REG
	out dx, al
	M_Countdown dx, 04FFFh
	ENDIF
ENDM

M_RotateLeds MACRO INST_ , HASCOUNT_
LOCAL @@led_loop
	IF BUILD_BIOS_DISPLAY EQ 1
	mov al, 0FEh
	mov cl, 8
	@@led_loop:
		mov dx, LEDS_REG
		IF INST_ EQ 0
			ror al, 1
		ELSE
			rol al, 1
		ENDIF
		out dx, al
		IFB <HASCOUNT_>
			M_Countdown dx, 04FFFh
		ELSE
			M_Countdown dx, HASCOUNT_
		ENDIF
		dec cl
		jnz @@led_loop	
	mov al, 0FFh
	out dx, al
	ENDIF
ENDM
 
;------------------------------------------------------------------------------
; 4. Som

; PPI: ativar TIM2GATESPK e SPKDATA
M_SoundOn MACRO 
	in al, PPI_REG_OPTS
	or al, 3
	out PPI_REG_OPTS, al
ENDM
; PPI: desativar TIM2GATESPK e SPKDATA
M_SoundOff MACRO 
	in al, PPI_REG_OPTS
	and al, 0FCh
	out PPI_REG_OPTS, al
ENDM
M_beep MACRO DURATION_
	mov ax, 02A6h
	out PIT_REG_SPK, al
	mov al, ah
	out PIT_REG_SPK, al
	M_SoundOn
	IFB <DURATION_>
		M_Countdown ax, 0FFFFh
	ELSE
		M_Countdown ax, DURATION_
	ENDIF
	M_SoundOff
ENDM
M_errorBeep MACRO DURATION_
	mov ax, 50A6h
	out PIT_REG_SPK, al
	mov al, ah
	out PIT_REG_SPK, al
	M_SoundOn
	IFB <DURATION_>
		M_Countdown ax, 0FFFFh
	ELSE
		M_Countdown ax, DURATION_
	ENDIF
	M_SoundOff
ENDM


;------------------------------------------------------------------------------
; 5. Interface de teste

M_dispERROR MACRO NUM_
	M_DispDigits 0Eh, 0Ah, 0Ah, NUM_ 
	M_errorBeep
	M_BlinkeLeds
ENDM

M_debugKeyb MACRO
	push ax
		xor ax, ax
		INT 16h
	pop ax
ENDM

M_testButton MACRO 
	M_errorBeep  
	M_Countdown ax
	M_Countdown ax
	M_errorBeep  
	wait
	M_Countdown ax
	M_Countdown ax
ENDM


;------------------------------------------------------------------------------
; 6. Interrupções

M_ENTER_INT MACRO INT_, NAME_ 
ENDM
M_EXIT_INT MACRO 
ENDM

M_IRQ_ON MACRO IRQ_
	IF IRQ_ LE 7
		in al, PIC0_IMR
		and al, NOT(1 SHL (IRQ_)) 
		out PIC0_IMR, al
	ELSE
		in al, PIC1_IMR
		and al, NOT(1 SHL (IRQ_-8)) 
		out PIC1_IMR, al
	ENDIF 
ENDM

M_IRQ_OFF MACRO IRQ_
	IF IRQ_ LE 7
		in al, PIC0_IMR
		or al, 1 SHL (IRQ_)
		out PIC0_IMR, al 
	ELSE
		in al, PIC1_IMR
		or al, 1 SHL (IRQ_-8)
		out PIC1_IMR, al 
	ENDIF
ENDM

M_defINT MACRO INT_, FUNC_
	mov word ptr ds:[INT_*4], offset FUNC_
	mov word ptr ds:[INT_*4+2], BIOS_SEG
ENDM

M_defIRQ MACRO IRQ_, FUNC_
	IF IRQ_ LE 7
		mov bx, (IRQ_*4) + 8*4
	ELSE
		mov bx, ((IRQ_-8)*4) + 070h*4
	ENDIF
		mov word ptr ds:[bx+2], BIOS_SEG
		mov word ptr ds:[bx], offset FUNC_
ENDM

M_saveINT MACRO INT_, OFFSET_
	mov ax, word ptr ds:[INT_*4]
	mov word ptr es:[offset OFFSET_], ax
	mov ax, word ptr ds:[INT_*4+2]
	mov word ptr es:[offset OFFSET_+2], ax
ENDM

M_restoreINT MACRO INT_, OFFSET_
	mov ax, es:[offset OFFSET_]
	mov word ptr es:[INT_*4], ax
	mov ax, es:[offset OFFSET_+2]
	mov word ptr es:[INT_*4+2], ax
ENDM

;------------------------------------------------------------------------------
; 7. Stack

M_ssStackToBIOS MACRO
	mov ax, STACKSEG				; BX, AX, DS estragados
	mov ss, ax						; STACK DA BIOS
	mov sp, STACKTOP
ENDM
M_ssRestoreStack MACRO
	mov ax, DATA					; DS=DATA
	mov ds, ax
	mov ax, SINGLE_STEP_SS			; REPOR STACK DE ENTRADA
	mov ss, ax						; "PUSH E POPS PROIBIDOS
	mov sp, SINGLE_STEP_SP			;  APARTIR DAQUI"
ENDM

M_push2 MACRO A1_, A2_
	push A1_
	push A2_
ENDM

M_pop2 MACRO A1_, A2_
	pop A2_
	pop A1_
ENDM
M_push3 MACRO A1_, A2_, A3_
	push A1_
	push A2_
	push A3_
ENDM

M_pop3 MACRO A1_, A2_, A3_
	pop A3_
	pop A2_
	pop A1_
ENDM

M_push4 MACRO A1_, A2_, A3_, A4_
	push A1_
	push A2_
	push A3_
	push A4_
ENDM
M_pop4 MACRO A1_, A2_, A3_, A4_
	pop A4_
	pop A3_
	pop A2_
	pop A1_
ENDM

M_push5 MACRO A1_, A2_, A3_, A4_, A5_
	push A1_
	push A2_
	push A3_
	push A4_
	push A5_
ENDM
M_pop5 MACRO A1_, A2_, A3_, A4_, A5_
	pop A5_
	pop A4_
	pop A3_
	pop A2_
	pop A1_
ENDM

M_push6 MACRO A1_, A2_, A3_, A4_, A5_, A6_
	push A1_
	push A2_
	push A3_
	push A4_
	push A5_
	push A6_
ENDM
M_pop6 MACRO A1_, A2_, A3_, A4_, A5_, A6_
	pop A6_
	pop A5_
	pop A4_
	pop A3_
	pop A2_
	pop A1_
ENDM

M_push7 MACRO A1_, A2_, A3_, A4_, A5_, A6_, A7_
	push A1_
	push A2_
	push A3_
	push A4_
	push A5_
	push A6_
	push A7_
ENDM

M_pop7 MACRO A1_, A2_, A3_, A4_, A5_, A6_, A7_
	pop A7_
	pop A6_
	pop A5_
	pop A4_
	pop A3_
	pop A2_
	pop A1_
ENDM

M_pushall MACRO
	PUSH	AX
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	SI
	PUSH	DI
	PUSH	DS
	PUSH	ES
ENDM

M_popall MACRO
	POP	ES
	POP	DS
	POP	DI
	POP	SI
	POP	DX
	POP	CX
	POP	BX
	POP	AX
ENDM


;------------------------------------------------------------------------------
; 8. Debug (com STACK)

M_SETINT_TRAP MACRO
LOCAL @@p_int1_skip, @@p_int1_end
	push ds
	push ax
	pushf

		pushf				; 
		pop ax				; AX=FLAGS
		test ax, x86_TF		; SE FLAG TF JÁ POSTA, SALTAR
		jnz @@p_int1_skip

		mov ax, DATA		; se já ATIVO SALTAR
		mov ds, ax
		mov al, SINGLE_STEP_ON
		or al, al
		jnz @@p_int1_skip

		M_PPI_showDip1		 
		M_PPI_TestDip 1		; se DIP NÃO ATIVO SALTAR
		jz @@p_int1_skip

						; REINSTALAR O PONTEIRO DO
						; TRATADOR
			mov ax, 0			
			mov ds, ax
			mov ax, offset DATA:SINGLE_STEP_CODEIP
			mov word ptr ds:[01h*4], ax
			mov ax, DATA  
			mov word ptr ds:[01h*4+2], ax
			mov ds, ax
			mov SINGLE_STEP_ON, 1

			call tty_clear
			popf			; restaurar flags porcausa das 
							; comparações!

			M_PPI_showDip0
			M_x86_enableFlags x86_TF
			jmp @@p_int1_end

	@@p_int1_skip:
		M_PPI_showDip0	; restaurar flags porcausa das 
		popf			; comparações!
	@@p_int1_end:
		pop ax
		pop ds
ENDM

;------------------------------------------------------------------------------
; 9. Utilitários

M_movOff MACRO SEG_, SEG2_, REG_
	IFNB <REG_>
		mov REG_, offset SEG2_
		mov SEG_, REG_
	ELSE
		mov ax, offset SEG2_
		mov SEG_, ax
	ENDIF
ENDM
M_mov MACRO SEG_, SEG2_, REG_
	IFNB <REG_>
		mov REG_, SEG2_
		mov SEG_, REG_
	ELSE
		mov ax, SEG2_
		mov SEG_, ax
	ENDIF
ENDM

; isto é para satisfazer os tempos 
; entre OUTs que o 8237 requer
M_WaitDMA MACRO 
	push AX
ENDM

;------------------------------------------------------------------------------
; 10. Saltos e chamadas

hlt_ MACRO 
local @@loopi
	@@loopi: jmp @@loopi
ENDM

emergency MACRO 
	call emergency_
ENDM

M_ResetJump MACRO
	db 0EAh 
	dw BIOS_START_OFFSET
	dw BIOS_SEG
ENDM

	; NAO USAR PUSHF/POPF! nestas chamadas "FAKE"
	; ao por qualquer coisa nas FLAGS
	; pode ativar por exemplo TRAP e chamar
	; a INT1 sem estar definida!!!!

M_fakeCall MACRO LOCATION_
	LOCAL @@returnCall_, @@returnPtr_
	mov sp, BIOS_SEG
	mov ss, sp
	mov sp, offset @@returnPtr_	; a pilha fica no código
	jmp LOCATION_				; em returnPtr_, que tem 
	@@returnPtr_:				; RA
		dw offset @@returnCall_	
	@@returnCall_:
ENDM
M_FarJump MACRO SEGMENT_, OFFSET_
	db 0EAh 
	dw offset OFFSET_
	dw SEGMENT_
ENDM

M_retFarWithout MACRO N_
	db 0CAh        ; return FAR descontando 2 bytes 
	dw N_		   ; se usar RET sem PROC FAR, ele
					; provavelmente vai gerar um NEAR
ENDM

;------------------------------------------------------------------------------
; 11. Operações com registos (M_MinusAX usa STACK)

M_abs16 MACRO X_, Y_
LOCAL @@jump1
	mov Y_, X_
	test Y_, 8000h
	jz @@jump1
	not Y_
	inc Y_
	@@jump1:
ENDM

M_sign16ToInc MACRO X_, Y_
LOCAL @@jump1
	mov Y_, +1
	test X_,  8000h
	jz @@jump1
		mov Y_, 0FFFFh
	@@jump1:
ENDM

M_InvertSign MACRO 
	not ax
	inc ax
ENDM

M_AXMinus MACRO M_
	sub ax, M_
ENDM
M_MinusAX MACRO M_
	push bx
		mov bx, M_
		sub bx, ax
		mov ax, bx
	pop bx
ENDM
 
M_32bitShr MACRO REG1_, REG2_, VAL_, TEMP_
	shr REG2_, VAL_
	mov TEMP_, REG2_
	shl REG1_, 16-VAL_
	or  REG2_, REG1_
	shr REG2_, TEMP_
	shr REG2_, VAL_
ENDM

;------------------------------------------------------------------------------
; 12. Conversões

M_encodeCHS MACRO C_, H_, S_
	mov dh, H_
	mov cx, C_
	xchg cl, ch
	shl cl, 5
	mov al, S_
	and al, 1Fh
	or  cl, al
ENDM


;------------------------------------------------------------------------------
; 13. CGA (sem STACK)

	; VSYNC só está ligado se VIDEOENABLE
M_CGA_clear MACRO PAGE_, WORD_, VSYNC_
	cld
	mov ax, 0B800h + (PAGE_*UNIT_IN_SEGS_8K)	;+8KB
	mov es, ax
	xor di, di
	mov cx, (UNIT_8K/2)
	IF VSYNC_ EQ 1
		mov dx, CGA_STATUS
		M_CGA_WaitVSync
	ENDIF
	mov ax, WORD_
	rep stosw
ENDM

M_CGA_testTextMode MACRO MODE_
LOCAL @@loop1, @@loop2, @@fillskip, @@fill
	mov al, MODE_	
	M_fakeCall cga_setVideo
	
	mov ax, 0B800h
	mov es, ax
	mov cx, ((MODE_+1)*40)*25		; preencher o ecrã com !!!
	mov ah, 070h
	mov al, 030h	
	xor di, di
	@@fill:
		stosw			; es:di = [ val | att ]
		inc al
		cmp al, 03Ah
		jne @@fillskip
			mov al, 030h
		@@fillskip:
		M_Countdown dx, 0FFh
	loop @@fill

	mov ax, 0			; linha coluna
	mov bl, 0Fh
	;mov si, offset cga_test_string

	mov ax, 0B800h
	mov es, ax
	mov di, (2*(MODE_+1)*40)*3
	mov cx, 16	  
	xor ax, ax				  ; escrever 256 caracteres 		
	@@loop1:				  ; ASCII nas próximas linhas
		mov dx, 16			  ; por 16 colunas 
		@@loop2:
			stosw			  ; guardar caractere
			inc al			  ; próximo caractere
			inc ah			  ; incrementar atributo
			dec dl
			jnz @@loop2
		add di, (2*(MODE_+1)*40-32)		; reposicionar na proxima linha
	loop @@loop1
ENDM

;------------------------------------------------------------------------------
; 14. Impressão no ecrã  (sem STACK)
 
M_text_clearScreen MACRO
	mov bx, 0B800h
	mov es, bx
	xor di, di
	mov cx, 80*25
	rep stosw
ENDM

M_text_wrStr MACRO LINE_, STR_
LOCAL @@next, @@done
	mov ax, 0B800h
	mov es, ax
	IF TTY_MODE EQ 0
		mov di, LINE_*40
	ELSE
		mov di, LINE_*80
	ENDIF
	@@next:
		mov al, ds:[si]
		or al, al
		jz @@done
		movsb 
		inc di
	jmp @@next
	@@done:
ENDM

M_tty_wrFieldChar MACRO CHAR_
	call tty_Sp
	push ax
		mov al, CHAR_
		call tty_char
		mov al, '='
		call tty_char
	pop ax
ENDM 

M_tty_ChgAtt MACRO ATT_
	push bx
		mov bl, ATT_
		call tty_setAtt
	pop bx
ENDM

M_CGA_WaitVSync MACRO 
LOCAL @@loop
	IF TTY_MODE EQ 1
	@@loop:
		in  al, dx
		test al, 8
		jz @@loop
	ENDIF
ENDM


;------------------------------------------------------------------------------
; 15. Impressão no ecrã (com STACK)

M_tty_wrFieldChar16 MACRO CHAR_, VAL_
	M_tty_wrFieldChar CHAR_
	push ax
		mov ax, VAL_
		call tty_Hex16
	pop ax
ENDM
M_tty_wrFieldChar8 MACRO CHAR_, VAL_
	M_tty_wrFieldChar CHAR_
	push ax
		mov al, VAL_
		call tty_Hex8
	pop ax
ENDM

M_tty_wrFieldSegReg MACRO STR_0, STR_1, STR_2, STR_3, REG0_, REG1_
	call tty_Sp
	push ax
		mov al, STR_0
		call tty_char
		mov al, STR_1
		call tty_char
		mov al, ':'
		call tty_char
		mov al, STR_2
		call tty_char
		mov al, STR_3
		call tty_char
		mov al, '='
		call tty_char
		mov ax, REG0_
		call tty_Hex16
		mov al, ':'
		call tty_char
		mov ax, REG1_
		call tty_Hex16
	pop ax
ENDM 

M_tty_wrFieldReg MACRO STR_0, STR_1, REG_
	call tty_Sp
	push ax
		mov al, STR_0
		call tty_char
		mov al, STR_1
		call tty_char
		mov al, '='
		call tty_char
	pop ax
	push ax
		mov ax, REG_
		call tty_Hex16
	pop ax
ENDM

M_tty_wrField MACRO FIELD_STR_
	push si
	call tty_crlf
	mov si, offset FIELD_STR_
	call tty_str			
	push ax
		mov al, '='
		call tty_char
	pop ax
	pop si
ENDM
M_tty_wrFieldStr MACRO STRING_
LOCAL @@skipStr, @@theString
	jmp @@skipStr
		@@theString: db STRING_ , 0
	@@skipStr:
	;call tty_crlf
	push si
	push ax
		call tty_sp
		mov si, offset @@theString
		call tty_str			
		mov al, '='
		call tty_char
	pop ax
	pop si
ENDM

	; USA AX, ES, SI, DI
M_dispERRORStr MACRO LINENUM_, STR_
LOCAL @@skipStr, @@strlabel,  @@loopN, @@loopNEnd
	jmp @@skipStr
	@@strlabel: 
		db  &STR_ , 0
	@@skipStr:
		mov ax, 0B800h 
		mov es, ax
		mov di, LINENUM_*(80*2)
		mov si, offset @@strlabel
		mov ah, 4Fh
		@@loopN:
			mov al, cs:[si]
			or al, al
			jz @@loopNEnd
			mov es:[di], al
			inc di
			mov es:[di], ah
			inc di
			inc si
			jmp @@loopN
		@@loopNEnd:
ENDM

M_tty_wrESFieldStr MACRO NAME_, NUM_, W16_
LOCAL @@loopNum, @@skipStr, @@strlabel
	call tty_crlf
	jmp @@skipStr
	@@strlabel:
		db " &NAME_ ", 0
	@@skipStr:
		; imprimir offset
	mov ax, offset NAME_
	call tty_Hex16 
	call tty_sp
		; imprimir nome
	mov si, offset @@strlabel
	call tty_str
	mov al, '='
	call tty_char
		; imprimir valores
	IFB <NUM_>
		mov cx, 1
	ELSE
		mov cx, NUM_
	ENDIF
	mov di, offset NAME_
	IFB <W16_>
		@@loopNum:
			mov al, es:[di]
			call tty_Hex8
			call tty_sp
			add di, 2
		loop @@loopNum
	ELSE
		@@loopNum:
			mov ax, es:[di]
			call tty_Hex16
			call tty_sp
			inc di
		loop @@loopNum
	ENDIF
ENDM


M_tty_cwrStr MACRO STRING_
LOCAL @@skipStr, @@theString
	jmp @@skipStr
		@@theString: db STRING_ , 0
	@@skipStr:
	push si
		mov si, offset @@theString
		call tty_centeredStr			
	pop si
	call tty_crlf
ENDM

M_tty_wrStr MACRO STRING_
LOCAL @@skipStr, @@theString
	jmp @@skipStr
		@@theString: db STRING_ , 0
	@@skipStr:
	push si
		mov si, offset @@theString
		call tty_str			
	pop si
ENDM


;------------------------------------------------------------------------------
; 16. Teclado

M_KeybOn MACRO 
	in al, PPI_REG_OPTS
	and al, 7Fh
	or  al, 40h
	out PPI_REG_OPTS, al
ENDM
M_KeybReset MACRO 
	in al, PPI_REG_OPTS
	or al, 080h
	out PPI_REG_OPTS, al
	and al, 07Fh
	out PPI_REG_OPTS, al
ENDM
M_KeybOff MACRO 
	in al, PPI_REG_OPTS
	and al, 0BFh
	out PPI_REG_OPTS, al
ENDM

;------------------------------------------------------------------------------
; 16. Porta Série

M_SERIAL_WAIT_RX MACRO 
LOCAL @@looprx
	push dx
	add dx, SERIAL_STATUS
	@@looprx:   ; esperar que o buffer de RX
		in al, dx
		test al, 1
		jz @@looprx 
	pop dx
ENDM
M_SERIAL_WAIT_TX MACRO 
LOCAL @@looptx
	push dx
	add dx, SERIAL_STATUS
	@@looptx:   ; esperar que o buffer de TX
		in al, dx
		test al, 20h
		jz @@looptx 
	pop dx
ENDM

;------------------------------------------------------------------------------
; 17. RTC (com STACK se RTC_DEBUG)

M_RTC_Read MACRO REGNAME_
	mov dx, RTC_ADDR 
	mov al, REGNAME_
	out dx, al       ; definir endereço registo
	xor al, al
	mov dx, RTC_DATA 
	in  al, dx       ; apanhar valor do registo
ENDM


M_RTC_Set MACRO REGNAME_, REGDATA_
	push ax
	mov dx, RTC_ADDR
	mov al, REGNAME_
	out dx, al        ; definir endereço registo
	pop ax
	nop
	nop
	push ax
	mov dx, RTC_DATA
	mov al, REGDATA_
	out dx, al        ; definir valor registo
	pop ax
ENDM

IF RTC_DEBUG      EQ  1
	M_RTC_Read_ MACRO REG_
	push bx
		mov bl, REG_
		xor bh, bh
		mov al, byte ptr cs:[offset rtcTestArr+bx]
	pop bx
	ENDM
	M_RTC_Set_ MACRO REG_, VAL_
	push bx
		mov bl, REG_
		xor bh, bh
		mov byte ptr cs:[offset rtcTestArr+bx], VAL_
	pop bx
	ENDM
ELSE
	M_RTC_Read_ MACRO REG_
		M_RTC_Read REG_
	ENDM
	M_RTC_Set_ MACRO REG_, VAL_
		M_RTC_Set REG_, VAL_
	ENDM
ENDIF
M_RTC_DebugRead MACRO REGNAME_
LOCAL @@strlabel,@@nxt
	jmp @@nxt
	@@strlabel:
		db " &REGNAME_ ", 0
	@@nxt:
	mov si, offset @@strlabel
	call tty_crlf
	call tty_str
	push ax
		mov al, '='
		call tty_char
	pop ax
	M_RTC_Read REGNAME_
	call tty_Hex8
ENDM
