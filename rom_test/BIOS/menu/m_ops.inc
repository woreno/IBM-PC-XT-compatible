;--------------------------------------------------------------------------------------
; 1. p_menuIndexPageString: entra CL=PAGINA sai SI=VETOR 
; 2. p_menuIndexRecord: entra CL=PAGINA, AH=REGISTO sai SI=VETOR 
; 3. p_menuShow_keys: scan ao teclado e muda AH (registo atual) ou pagina (CL)
; 4. p_menuShow_exit: ve se o RTC funciona e RESET

;--------------------------------------------------------------------------------------
; 1. p_menuIndexPageString entra CL=PAGINA sai SI=VETOR 
p_menuIndexPageString:
	mov si, offset bpagesStrs
	or cl, cl
	jz p_menuIndexPageStringEnd
		p_mips_loop:
			push cx
				call strlen
				add si, cx
				inc si
			pop cx
		loop p_mips_loop
p_menuIndexPageStringEnd:
ret
;--------------------------------------------------------------------------------------
; 2. p_menuIndexRecord entra CL=PAGINA, AH=REGISTO sai SI=VETOR 
p_menuIndexRecord:
	push ax			
	push cx
		xor ch, ch				
		shl cl, 2				
		mov si, cx
		mov si, word ptr cs:[bpages+si]	;bn_ops	
		xor al, al				
		xchg ah, al
		shl al, 1
		add si, ax				
		pop cx
	pop ax
	mov si, cs:[si]
ret

p_menu_noPrn:
ret

;-----------------------------------------------------------------
; 3. p_menuShow_keys: teclado muda AH (registo atual) ou pagina (CL)
p_menuShow_keys:
	push bx

	p_menuShow_keysEscaped:
		push ax
			xor ax, ax
			INT 16h
			mov bx, ax
			mov bl, bh
		pop ax
		
		cmp bh, KEY_SCANCODE_ESCAPED
		je p_menuShow_keysEscaped

	cmp bh, KEY_SCANCODE_ESC
	jne p_menuShow_keys_cont	; se nao esc, não sair
		call p_menuShow_exit

	p_menuShow_keys_cont:
		
		cmp bl, KEY_SCANCODE_ENTER
		je p_menuShow_keyEnter
		cmp bl, KEY_SCANCODE_DOWN
		je p_menuShow_keysDown
		cmp bl, KEY_SCANCODE_UP
		je p_menuShow_keysUp
		

		push ax					; guardar "valor" 
		mov al, bl				; se scancode numero
		call keyb2digit
		jc p_menuShow_no

			or al, al			; se zero voltar
			jz p_menuShow_no
			dec al				
			mov cl, al			; CL=nova página
			pop ax
			pop bx
			mov ah, 0		; reset ao indice
			stc
			ret

		p_menuShow_no:
			pop ax
			pop bx
			clc
			ret

		p_menuShow_keysDown:
			push cx
				mov bl, cl
				xor bh, bh
				shl bl, 2
				mov bl, byte ptr cs:[bpages+bx+2]
				dec bl
			pop cx
			cmp ah, bl
			je p_menuShow_keysDone
			inc ah
		jmp p_menuShow_keysDone

		p_menuShow_keysUp:
			dec ah
			cmp ah, 0FFh
			jne p_menuShow_keysDone
				mov ah, 0
			jmp p_menuShow_keysDone

		p_menuShow_keyEnter:
			call p_menuIndexRecord  
			pop bx
			push bx 
			push cx
			call p_menu_ENTER 
			pop cx

		p_menuShow_keysDone:
			pop bx
			clc
			ret

;-----------------------------------------------------------------
; 4. p_menuShow_exit: ve se o RTC funciona e RESET
p_menuShow_exit:
	call rtc_saveCookie
	call rtc_cfg_isOK
	jc p_menuShow_exitAbnormal
		p_menuShow_exitOK:
		call tty_clear
		mov dx, 0900h
		call tty_setCur
		call ds2cs
		M_tty_cwrStr "A reiniciar..."
		M_ResetJump
	p_menuShow_exitAbnormal:
		call p_menu_clearRed
		call ds2cs
		mov dx, 0800h
		call tty_setCur
		M_tty_cwrStr "Os dados no RTC nao foram bem gravados!"
		inc dh
		call tty_setCur
		M_tty_cwrStr "Ou nao existe RTC, ou sua memoria falhou, ou a bateria esta' fraca."
		inc dh
		call tty_setCur
		M_tty_cwrStr "Premir uma tecla para continuar..."
		xor ax, ax
		INT 16h
		jmp p_menuShow_exitOK