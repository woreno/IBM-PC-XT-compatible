;-----------------------------------------------------------------
; 1. p_menu_setBP: definir prioridade de arranque
; 2. p_menu_prnBP: imprimir prioridade numa linha
; 3. p_menu_prnBPlist: imprimir prioridade em lista
; 4. p_menu_setBPKeys: scan teclado chamar a def. uma unidade/trocar prioridade

;-----------------------------------------------------------------
; 1. p_menu_setBP: definir prioridade de arranque
p_menu_setBP:
	call p_menu_clear
	mov si, offset bn07+1
	call p_menu_setTitle
	mov si, offset bn07_footer_str
	call p_menu_setFooter
	
	mov dx, 0400h
	call tty_setCur
	call rtc_BootPriorityCheck
	jnc p_menu_sbp_ok
		M_tty_cwrStr "A ordem guardada estava invalida"
		call tty_crlf
		M_tty_cwrStr "reposta ordem normal"
	p_menu_sbp_ok:

	mov dx, 0A1Ch
	call tty_setCur
	M_tty_wrStr "Ordem: "

	mov dx, 0			; selecionado
	p_menu_sbp_loop1:
		mov cx, 0
		push ax
			p_menu_sbp_loop2:
				call p_menu_prnBPlist
				inc cx
				cmp cx, 4
			jne p_menu_sbp_loop2
		pop ax
		call p_menu_setBPKeys
	jmp p_menu_sbp_loop1
ret

;-----------------------------------------------------------------
; 2. p_menu_prnBP: imprimir prioridade numa linha
p_menu_prnBP:
M_push4 ax, bx, cx, di
	call ds2cs
		mov di, offset bmenu_disks_strs
		M_RTC_Read_ RTC_RAM_BOOT_PRI
		mov cx, 4
		p_menu_prnBpLoop:
			mov bl, al
			and bl, 11b
			shl bl, 1
			xor bh, bh
			mov si, cs:[di+bx]
			call tty_str
			shr al, 2
			cmp cx, 1
			je p_menu_prnBpLoop1
			push ax
				mov al, '>'
				call tty_char
			pop ax
			p_menu_prnBpLoop1:
		loop p_menu_prnBpLoop

		call rtc_BootPriorityCheck
		jnc p_menu_prnBP1
			M_tty_wrStr "(invalida!)"
		p_menu_prnBP1:

		M_pop4 ax, bx, cx, di
		ret
		
;-----------------------------------------------------------------
; 3. p_menu_prnBPlist: imprimir prioridade em lista
p_menu_prnBPlist: 
	mov ah, al			; ah = byte
	and ah, 11b			; ah = selecao
	push ax
		push dx
			mov dx, 0A25h
			add dh, cl
			call tty_setCur
		pop dx

		push bx
			mov bl, B_MENU_BACKCOLOR
			call tty_setAtt
		pop bx

		cmp cl, dl
		jne p_menu_sbpShow_blank

		push bx
			mov bl, B_MENU_SELCOLOR
			call tty_setAtt
		pop bx

		p_menu_sbpShow_blank:
		push ax
			mov al, cl
			inc al
			call tty_U8
			mov al, '.'
			call tty_char
			call tty_sp
		pop ax
		xchg al, ah
		xor ah, ah
		shl al, 1
		mov si, offset bmenu_disks_strs
		add si, ax
		mov si, word ptr ds:[si]
		call tty_str
	pop ax
	shr al, 2
ret

;-----------------------------------------------------------------
; 4. p_menu_setBPKeys: scan teclado chamar a def. uma unidade/trocar prioridade
p_menu_setBPKeys:
M_push3 bx, cx, ax
	p_menu_setBPKeysEsc:	
		xor ax, ax
		INT 16h
		mov al, ah
		cmp al, KEY_SCANCODE_ESCAPED
	je p_menu_setBPKeysEsc

	cmp al, KEY_SCANCODE_ESC
	je p_menu_sbpKeysCancel
	cmp al, KEY_SCANCODE_PLUS 
	je p_menu_sbpChgToUp
	cmp al, KEY_SCANCODE_MINUS
	je p_menu_sbpChgToDown
	cmp al, KEY_SCANCODE_UP
	je p_menu_sbpSelUp
	cmp al, KEY_SCANCODE_DOWN
	je p_menu_sbpSelDown
	jmp p_menu_sbpKeysNo

	p_menu_sbpSelDown:
		inc dl
		jmp p_menu_sbpSel
	p_menu_sbpSelUp:
		dec dl
	p_menu_sbpSel:
		and dl, 03h
		clc
		jmp p_menu_sbpKeysOK

	p_menu_sbpKeysNo:
		clc
		M_pop3 bx, cx, ax
		ret
	p_menu_sbpKeysCancel:
		stc
		pop ax
	p_menu_sbpKeysOK:		; aceitar digito em AL
		mov bx, ax
		pop ax
		mov ah, al			; vai para AH
		pop cx
		pop bx
		ret
	p_menu_sbpChgToUp:
		pop ax
		mov dh, dl
		dec dl
		jmp p_menu_sbpChg
	p_menu_sbpChgToDown:
		pop ax
		mov dh, dl
		inc dl
	p_menu_sbpChg:
		and dl, 11b
		call swapTwoBits
		clc
		pop cx
		pop bx
		ret
ret