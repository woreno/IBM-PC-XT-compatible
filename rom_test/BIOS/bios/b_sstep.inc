; GUARDA E RESTAURA os valores de ENTRADA

p_singleStep_intCode:
	;  FLAGS +4 |
	;  CS    +2  > valores antes da interrupção
	;  IP    +0 | <-- SS:SP
		mov word ptr cs:[offset DATA:SINGLE_STEP_AX], ax	; guardar AX
		mov word ptr cs:[offset DATA:SINGLE_STEP_DS], ds	; guardar DS
		db 0EAh
		dw offset p_singleStep
		dw BIOS_SEG
	p_ss_intCodeExit:
		mov ds, word ptr cs:[offset DATA:SINGLE_STEP_DS]	
		mov ax, word ptr cs:[offset DATA:SINGLE_STEP_AX]	; restaurar AX
	iret 
p_breakpoint_intCode:
		mov word ptr cs:[offset DATA:SINGLE_STEP_AX], ax	; guardar AX
		mov word ptr cs:[offset DATA:SINGLE_STEP_DS], ds	; guardar DS
		db 0EAh
		dw offset p_breakpoint
		dw BIOS_SEG

p_singleStep_intCodeLength EQU $-p_singleStep_intCode

;------------------------------------------------------------------------
;------------------------------------------------------------------------
;------------------------------------------------------------------------
p_breakpoint:
	;  FLAGS +4 |
	;  CS    +2  > valores antes da interrupção
	;  IP    +0 | <-- SS:SP
	assume ds:DATA
	mov ax, DATA					; DS = DATA
	mov ds, ax			
	mov SINGLE_STEP_SI, si
	
				; restaurar opcode  (usa ax, ds, si)
	mov al, SINGLE_STEP_NEXTOP
	mov si, SINGLE_STEP_NEXTIP
	mov ds, SINGLE_STEP_NEXTCS		; DS:SI = próxima instrução
	mov ds:[si], al

				; restaurar registos modificados
	mov ax, DATA					; DS = DATA
	mov ds, ax			
	mov ax, SINGLE_STEP_AX
	mov si, SINGLE_STEP_SI
	mov ds, SINGLE_STEP_DS


;------------------------------------------------------------------------
;------------------------------------------------------------------------
;------------------------------------------------------------------------

p_singleStep:
	;  FLAGS +4 |
	;  CS    +2  > valores antes da interrupção
	;  IP    +0 | <-- SS:SP

		; ATENÇÃO: NÃO USAR STACK!!!!!!!

	assume ds:DATA
	mov ax, DATA					; DS = DATA
	mov ds, ax			
	mov SINGLE_STEP_BX, bx			; guardar valores de entrada 
	mov SINGLE_STEP_SI, si
	mov SINGLE_STEP_SS, ss
	mov SINGLE_STEP_SP, sp
	mov SINGLE_STEP_BP, bp 
	mov bp, sp						; BP=SP aponta para IP/CS/FLAGS
	mov ax, ss:[bp+4]
	mov SINGLE_STEP_FLAGS,	 ax		; guardar FLAGS

		; SE A STACK JÁ FOR A DA BIOS, SAIR
	mov ax, ss							; AX = SS atual
	cmp ax, STACKSEG
	je p_singleStepExit

		; SE CS ATUAL for da BIOS SAIR
	mov ax, SINGLE_STEP_CS				; AX = CS atual
	cmp ax, BIOS_SEG
	je p_singleStepExit
	 
		; SE CS ATUAL != PRÓXIMO, IMPRIMIR 
	mov bx, ss:[bp+2]					; BX = CS próximo
	cmp ax, bx							; AX = CS atual
	jne p_singleStepPrn

	mov bx, ss:[bp]					; bx = IP seguinte

	; passar à frente da vetorização da INT1 do DOS
	IF BUILD_SSTEP_SKIP_DOS EQ 1
		cmp ax, 070h					; se CS!=70h proximo teste
		jne p_singleStepNotDosInit		

		mov al, dos_crash				; se flag já escrita
		or al, al						; passar à frente
		jnz p_singleStepNotDosInit
		
			cmp bx, 1965h				; bx seguinte é o do CRASH?
			jne p_singleStepNotDosInit
				mov dos_crash, 1		; escrever flag	
				mov ss:[bp], 1968h		; saltar em frente
		p_singleStepNotDosInit:
	ENDIF
									; bx = IP seguinte
	mov ax, SINGLE_STEP_IP			; ax = IP atual
	cmp ax, bx	
	je p_singleStepExit				; são iguais? sair
	

	p_singleStepPrn:
	
		M_ssStackToBIOS
		call p_singleStepPrint			; CHAMAR IMPRESSÃO
		M_ssRestoreStack

	p_singleStepExit:
	; colocar de novo bit de SINGLESTEP
	; porque esta int. a desliga
	; vai ter efeito após IRET
		mov ax, ss:[bp+4]
		or ax, x86_TF
		mov ss:[bp+4], ax

	; GUARDAR PRÓXIMO CS:IP (PRÓXIMA INSTRUÇÃO) e AS FLAGS
	; para IMPRESSÃO e DA PROXIMA VEZ SABER SE É O MESMA
		mov ax, ss:[bp]
		mov SINGLE_STEP_IP, ax
		mov ax, ss:[bp+2]
		mov SINGLE_STEP_CS, ax
		
		; repor BX e BP os unicos modificados aqui
		; AX e DS são respostos depois
	mov si, SINGLE_STEP_SI
	mov bx, SINGLE_STEP_BX
	mov bp, SINGLE_STEP_BP
	mov sp, SINGLE_STEP_SP

		; SALTO "FAR" PARA O FIM DO TRATADOR ORIGINAL, 
		; QUE VAI REPOR AX E DS
	assume ds:CODE
	db 0EAh
	dw offset DATA:SINGLE_STEP_CODEIP + (offset p_ss_intCodeExit - offset p_singleStep_intCode)
	dw DATA

;------------------------------------------------------------------------
;------------------------------------------------------------------------
;------------------------------------------------------------------------
; Imprime as coisas passo-a-passo

p_singleStepPrint:				; entra DS=DATA
	cld							; estraga AX, BX, DS
	assume ds:DATA

	push dx
		IF TTY_MODE EQ 0
			mov al, 00101000b			
		ELSE
			mov al, 00101001b			
		ENDIF
		mov dx, CGA_MODE
		out dx, al
		;M_dispDigits 0Fh, 0, 0Dh, 0Ah
	pop dx

								; apagar linhas
	IF BUILD_SSTEP_ONELINE EQ 1	; se ONELINE senão dar CRLF
		call tty_restoreCur
		mov ax, 3+(1-TTY_MODE)*2
		call tty_clearLines
	ELSE
		call tty_crlf
		call tty_crlf
	ENDIF

	call printOpcodes

	assume ds:DATA
	mov ax, DATA
	mov ds, ax

	inc int13_accesses

								; imprimir valores
	M_tty_ChgAtt 047h
	M_tty_wrFieldSegReg 'C','S','I','P', SINGLE_STEP_CS, SINGLE_STEP_IP
	M_tty_ChgAtt 007h
	M_tty_wrFieldReg ' ','F',SINGLE_STEP_FLAGS
	M_tty_wrFieldReg 'S','S',SINGLE_STEP_SS
	IF TTY_MODE EQ 0
		call tty_crlf
	ENDIF
	M_tty_wrFieldReg 'S','P',SINGLE_STEP_SP
	M_tty_wrFieldReg 'B','P',SINGLE_STEP_BP
	M_tty_wrFieldReg 'C','#',int13_accesses


	mov ax, SINGLE_STEP_AX		; verificar se é INT
	call checkInt				; SAI AL="Nº INT" e AH="AH" FUNCAO
	jc p_ssPrn_notInt
	
	call printInt				; Imprimir informação da INT

	cmp al, 10h
	je skipwait

	cmp al, 13h
	jne skipothers
								; Imprimir CHS do disco, etc
		mov ax, DATA
		mov ds, ax
		mov ax, SINGLE_STEP_AX
		mov bx, SINGLE_STEP_BX
		call tty_crlf
		call tty_CHSfromRegs

	skipothers:
								; se INT ver DIP e 
								; mostrar registos, BEEP e pausa
		call p_singleStepCheckButton
		jc p_ssPrn_skipRegs	
								; se já mostrou regs nao mostrar
								; outra vez
	skipbutton:
		call smallWait
	skipwait:
			
	p_ssPrn_notInt:
		M_PPI_TestDip 3
		jz p_ssPrn_skipRegs
			mov ax, DATA
			mov ds, ax
			mov ax,  SINGLE_STEP_AX
			mov bx,  SINGLE_STEP_BX
			mov ds,  SINGLE_STEP_DS
			call tty_regs
			call p_singleStepPrintStack
	p_ssPrn_skipRegs:

	M_PPI_TestDip 1
	jz p_ssPrn_noWait
		wait 
		call longWait
	p_ssPrn_noWait:

  assume ds:CODE
ret

p_singleStepPrintStack:
assume DS:DATA
push ds
push si
push ax
push cx
	mov cx, 8

	call tty_crlf
	mov ax, DATA
	mov ds, ax
	mov si, SINGLE_STEP_SP
	mov ds, SINGLE_STEP_SS
	call tty_hex16arr
	
	call tty_crlf
	mov ax, DATA
	mov ds, ax
	mov si, SINGLE_STEP_BP
	mov ds, SINGLE_STEP_SS
	call tty_hex16arr
pop cx
pop ax
pop si
pop ds
assume DS:CODE
ret

;------------------------------------------------------------
;------------------------------------------------------------
;------------------------------------------------------------
; SE INT, verificar o SWITCH 2, se ON IMPRIMIR REGISTOS e PAUSA

p_singleStepCheckButton:
	assume DS:DATA
	
	M_PPI_TestDip 2
	jz p_ss_CheckButtonExit
	
	mov ax, DATA
	mov ds, ax
	mov ax, SINGLE_STEP_AX
	mov bx, SINGLE_STEP_BX
	mov ds, SINGLE_STEP_DS
	call tty_regs			; imprimir registos
	call p_singleStepPrintStack
	call makeShortBeep
	wait
	call longWait
	stc
	ret
p_ss_CheckButtonExit:
	clc
	ret
	assume DS:CODE


checkSingleStepInstall:
push ax
push cx
push si
push di
push es
push ds

	mov ax, BIOS_SEG
	mov ds, ax
	mov si, offset p_singleStep_intCode
	mov ax, DATA
	mov es, ax
	mov di, offset SINGLE_STEP_CODEIP
	mov cx, (p_singleStep_intCodeLength+1)/2
	repe CMPSW

	cmp cx, 0
	je checkSingleStepInstallEnd
		call ds2cs
		call tty_clear
		M_tty_wrStr "Memoria corrupta"
		call errorBeep
		wait
	checkSingleStepInstallEnd:
		
pop ds
pop es
pop di
pop si
pop cx
pop ax
ret