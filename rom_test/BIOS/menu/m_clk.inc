; relógio 
; 1. p_menu_setClock: definir data/hora
; 2. p_menu_inNumber: entrada de numero (2 digitos)

;-----------------------------------------------------------------
; 1. p_menu_setClock: definir data/hora
p_menu_setClock:
	call p_menu_clear

	mov si, offset bm01+1
	call p_menu_setTitle
	mov si, offset bm01_footer_str
	call p_menu_setFooter 

	mov dx, 0812h
	call tty_setCur
	M_tty_wrStr "A data e': "
		call rtc_printDate
	mov dx, 0912h
	call tty_setCur
	M_tty_wrStr "A hora e': "
		call rtc_printTime
	mov dx, 0B12h
	call tty_setCur
	M_tty_wrStr "Definir nova data: "
	
		; o bit SET é necessário para parar a atualização
		; das horas e poder redefinir os registos
		M_RTC_Set_ RTC_STATUS_B, 80h+RTC_STATUS_B_CFG
		
		M_RTC_Read_ RTC_MDAY
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_MDAY, al
		mov al, '/'
		call tty_char
		
		M_RTC_Read_ RTC_MONTH
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_MONTH, al
		mov al, '/'
		call tty_char
		
		M_RTC_Read_ RTC_YEAR
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_YEAR, al
	
	mov dx, 0C12h
	call tty_setCur
	M_tty_wrStr "Definir nova hora: "
	
		M_RTC_Read_ RTC_HOUR
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_HOUR, al
		mov al, ':'
		call tty_char
		
		M_RTC_Read_ RTC_MIN
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_MIN, al
		mov al, ':'
		call tty_char
		
		M_RTC_Read_ RTC_SEC
		call BCDtoU8
		call p_menu_inNumber
		call U8toBCD
		M_RTC_Set_ RTC_SEC, al


		M_RTC_Set_ RTC_STATUS_B, RTC_STATUS_B_CFG

		call longWait
		call longWait
ret


;-----------------------------------------------------------------
; 2. p_menu_inNumber: entrada de numero (2 digitos)
p_menu_inNumber:
push bx
	call tty_saveCur
	mov bl, B_MENU_INPUTCOLOR
	call tty_setAtt
pop bx
push ax
	mov al, 20h
	call tty_char
	call tty_char
pop ax
	call tty_restoreCur
push bx
	mov bl, B_MENU_INPUTCOLOR
	call tty_setAtt
pop bx
push bx
push cx
push dx
push ax
	mov bx, 0
	mov dl, 0
	call tty_saveCur

	p_menu_inNumberEnter:
		xor ax, ax
		INT 16h
		xchg ah, al
		cmp al, KEY_SCANCODE_ESC
		je p_menu_inNumberCancel
		cmp al, KEY_SCANCODE_ENTER
		je p_menu_inNumberAccept
		call keyb2Digit
		jc p_menu_inNumberCancel

			inc dl
			push ax
				mov ax, bx		; AX=BX=ANTERIOR
				mov cl, 10
				mul cl			; BX=AX*10+DIGITO
				mov bx, ax
			pop ax
			xor ah, ah			; AL=DIGITO
			add bx, ax			; AX= BX+DIGITO
			mov ax, bx			
			call tty_restoreCur
			call tty_U16
			cmp dl, 2
			jz p_menu_inNumberAccept
			jmp p_menu_inNumberEnter

p_menu_inNumberAccept:
	or dl, dl
	jz p_menu_inNumberCancel
	mov bx, B_MENU_BACKCOLOR
	call tty_setAtt
	add sp, 2
	clc
	jmp p_menu_inNumberExit
p_menu_inNumberCancel:
	mov bx, B_MENU_BACKCOLOR
	call tty_setAtt
	pop ax
	call tty_restoreCur
	call tty_U16
	stc
p_menu_inNumberExit:
	mov bx, B_MENU_BACKCOLOR
	call tty_setAtt
	pop dx
	pop cx
	pop bx
	ret
