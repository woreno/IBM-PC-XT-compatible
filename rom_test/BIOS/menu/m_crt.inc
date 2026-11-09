; chamadas de impressão 
; 1. p_menu_prnBool |
; 2. p_menu_prnEnum > as chamadas para imprimir o valor dum registo
; 3. p_menu_prnCall |
; 4. p_menuShowSign: no arranque, aquela linha de "premir F1" para entrar 
; 5. p_menuShowSign: para esconder a linha anterior
; 6. p_menu_clearRed: por tudo a vermelho, porque algo falhou
; 7. p_menu_showPages: mostra os separadores
; 8. p_menu_dialog: qq. tecla ou ESC, e ESC retorna ao ecra principal
; 9. p_menu_errorDialog: indica erro, uma tecla para retornar ao ecra principal
; 9. limpar o ecra

bmenu_disk0        db "HDD0", 0
bmenu_disk1        db "HDD1", 0
bmenu_disk2        db "FDD0", 0
bmenu_disk3        db "FDD1", 0
bmenu_disks_strs   dw offset bmenu_disk0, offset bmenu_disk1, offset bmenu_disk2, offset bmenu_disk3, 0

bm15_footer_str      db "", 0
bn07_footer_str      db "[ESC: sair] [^V: selecionar] [-: menos prioridade] [+: mais prioridade]", 0
bm01_footer_str      db "[ENTER: aceitar] [ESC: currente]", 0
bmenu_escstr         db "[ESC: sair] [DIGITOS: mudar pagina] [^V: selecionar] [ENTER: modificar]",0
bmenu_title          db "Definir opcoes da BIOS", 0
strPressEnter:       db "<premir alguma tecla quando preparado, ESC para abortar>", 0
strPressErrorEnter:  db "<houveram erros, premir uma tecla para voltar>", 0

;-------------------------------------------------------------------------------------
; 1. p_menu_prnBool: imprime Sim ou Não, SI = no campo "registo RTC", do registo
p_menu_prnBool:
	M_push3 ax, bx,  dx
	call tty_ddot
	mov bl, byte ptr cs:[si] ; BL = REGISTO RTC 
	M_RTC_Read_ bl			 ; AL = OPÇÃO GUARDADA
	test al, 1
	jz p_menu_prnBool1
		M_tty_wrStr "Sim"
		jmp p_menu_prnBool2
	p_menu_prnBool1:
		M_tty_wrStr "Nao"
	p_menu_prnBool2:
	M_pop3 ax, bx,  dx
ret
;-------------------------------------------------------------------------------------
; 2. imprime a string selecionada,  SI = no campo "registo RTC", do registo
p_menu_prnEnum:
	M_push4 ax, bx, cx, dx
	call tty_ddot
	
	mov bl, byte ptr cs:[si] ; BL = REGISTO RTC 
	M_RTC_Read_ bl			 ; AL = OPÇÃO GUARDADA
	inc si					 ;	
	mov cl, byte ptr cs:[si] ; CL = N. MAX DE ELEMENTOS
	inc si					 ; SI = primeira STRING
	
	mov ah, 0				 ; AH = ITERADOR
	xor ch, ch
	cmp al, cl
	jb p_menu_prnEnum1
		mov AL, 0		; AL = 0
		M_RTC_Set_ BL, 0
		M_tty_wrStr "[invalido! reposto a] "
	 
	p_menu_prnEnum1:
		cmp al, ah				; AL==AH=ITERADOR?
		jne p_menu_prnEnum2
			call tty_str
			jmp p_menu_prnEnumEnd	
	
		p_menu_prnEnum2:
			call strlen	
			add si, cx	; SI=0
			inc si		; SI=PROXIMA STRING
			inc ah      ; AH=ITERADOR 
		loop p_menu_prnEnum1
	
	p_menu_prnEnumEnd:
		M_pop4 ax, bx, cx, dx
		ret
;-----------------------------------------------------------------
; 3. p_menu_prnCall: tipo CALL, chama a chamada impressor
p_menu_prnCall:
	M_push4 ax, bx, dx, si	
		mov bx, 0
		cmp cs:[si], bx
		je p_menu_prnCall1
			call tty_ddot
			call cs:[si]
		p_menu_prnCall1:
	M_pop4 ax, bx, dx, si	
ret
 
;-----------------------------------------------------------------
; 4. p_menuShowSign: no arranque, aquela linha de "premir F1" para entrar 

IF TEST_MENU EQ 0
p_menuShowSign:
	call tty_saveCur
	assume DS:DATA
	mov ax, DATA
	mov ds, ax
	mov tty_col, 0
	mov tty_row, 24
	mov tty_att, 1Fh
	assume DS:CODE
	mov ax, 1
	call tty_clearLines
	call ds2cs
	M_tty_wrStr " ENTER: opcoes | Testes M:MEM | V:VIDEO | C:COM1 | L:LPT1 | I:IO | J:Joystick"
	call tty_restoreCur
	
	cli
	ASSUME ds:CODE
	M_IRQ_OFF KEYB_IRQ	  ; desativar teclado
	xor ax, ax
	mov ds, ax
	M_defIRQ KEYB_IRQ,   p_menuKeybInt
	M_IRQ_ON KEYB_IRQ	  ; ativar teclado
	sti
ret

;-----------------------------------------------------------------
; 5. p_menuShowSign: para esconder a linha anterior
p_menuHideSign:

	cli
	M_IRQ_OFF KEYB_IRQ  ; desativar teclado
	xor ax, ax
	mov ds, ax
	M_defIRQ KEYB_IRQ,   p_keyb_int
	M_IRQ_ON KEYB_IRQ	  ; ativar teclado		
	sti

	call tty_saveCur
	assume DS:DATA
	mov ax, DATA
	mov ds, ax
	mov tty_col, 0
	mov tty_row, 24
	mov tty_att, 0Fh
	assume DS:CODE
	mov ax, 1
	call tty_clearLines
	call tty_restoreCur
ret
ENDIF

;-----------------------------------------------------------------
; 6. p_menu_clearRed: por tudo a vermelho, porque algo falhou
p_menu_clearRed:
	assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov tty_row, 0
		mov tty_col, 0
		mov tty_att, 4Fh
	assume DS:CODE
	mov ax, 25
	call tty_clearLines
ret
p_menu_clear:
	assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov tty_row, 0
		mov tty_col, 0
		mov tty_att, B_MENU_BACKCOLOR
	assume DS:CODE
	mov ax, 25
	call tty_clearLines
ret
p_menu_setTitle:
	call ds2cs
	mov bl, B_MENU_TITLECOLOR
	call tty_coloredLine
	call tty_setAtt
	call tty_centeredStr
	mov bl, B_MENU_BACKCOLOR
	call tty_setAtt
ret
p_menu_setFooter:
	call ds2cs
	mov dx, 1800h
	call tty_setCur
	mov bl, B_MENU_TITLECOLOR
	call tty_setAtt
	call tty_coloredLine
	call tty_str	
	mov bl, B_MENU_BACKCOLOR
	call tty_setAtt
ret

;-------------------------------------------------------------
; 7. p_menu_showPages: para mostrar os separadores
p_menu_showPages:
	push cx
	push ax
		mov al, cl
		xor ah, ah
		shl ax, 2					; cl*4
		mov di, offset bpages
		add di, ax
		mov di, word ptr cs:[di]	; indexar página

		;-------------------------- decorar parte de cima com _
		mov dx, 0100h				;cursor linha 1
		call tty_setCur				
		mov bl, B_MENU_TITLECOLOR	
		call tty_setAtt				;atributo branco
		mov ax, 2
		call tty_clearLines			;pintar 2 linhas
		mov dx, 0200h
		call tty_setCur				;cursor linha 2
		mov al, '_'
		push cx
			mov cx, 80				;desenhar _
			call tty_chars
		pop cx
		mov dx, 0115h				;por cursor a meio
		mov dl, (80-(bpagesStrsSz+12))/2
		call tty_setCur
	pop ax
	pop cx

	push ax
	push cx
		push dx
		xor ch, ch
		mov si, offset bpagesStrs
		p_menu_indexPageLoop0:
			
			call tty_sp
			push cx
				;call tty_str
				call strlen
				add si, cx
				inc si
				add cx, 2 
				mov al, '_'
				call tty_chars
			pop cx
			call tty_sp
			call tty_sp
			call tty_sp

		inc ch
		cmp ch, bpagesNum
		jne p_menu_indexPageLoop0
		pop dx
		inc dh
		call tty_setCur

		xor ch, ch
		mov si, offset bpagesStrs
		p_menu_indexPageLoop1:
		
			mov bl, B_MENU_TITLECOLOR
			call tty_setAtt			
			
			cmp cl, ch
			jne p_menu_indexPage0

				mov bl, B_MENU_BACKCOLOR		
				call tty_setAtt			
			
			p_menu_indexPage0:
			mov al, '/'
			call tty_char
			mov al, ch
			inc al
			call tty_U8
			call tty_dot
			call tty_str
			push cx
				call strlen
				add si, cx
				inc si
			pop cx
			mov al, '\'
			call tty_char
			mov bl, B_MENU_TITLECOLOR
			call tty_setAtt			
			mov al, '_'
			call tty_char
			call tty_char

		inc ch
		cmp ch, bpagesNum
		jne p_menu_indexPageLoop1
		
		mov bl, B_MENU_BACKCOLOR		
		call tty_setAtt				; ATIVA
		mov dx, 0440h
		call tty_setCur
		mov ax, 25-5
		call tty_clearLines
	pop cx
	pop ax 
ret


;-----------------------------------------------------------------
; 8. p_menu_dialog: qq. tecla ou ESC, se ESC retorna ao ecra principal
p_menu_dialog:
push ds
	call ds2cs
	mov si, offset strPressEnter
	call tty_crlf
	call tty_crlf
	call tty_crlf
	call tty_centeredStr 
	push ax
	xor ax, ax
	INT 16h
	cmp ah, KEY_SCANCODE_ESC
	jne blabla_ok
		call p_menuShow
	blabla_ok:
	pop ax
	pop ds
	call p_menu_clearScreen
ret
;-----------------------------------------------------------------
; 9. indica erro, uma tecla para retornar ao ecra principal
p_menu_errorDialog:
	mov si, offset strPressErrorEnter
	call tty_crlf
	call tty_crlf
	call tty_centeredStr 
	xor ax, ax
	INT 16h
	call p_menuShow
ret
;-----------------------------------------------------------------
; 10. limpar o ecra
p_menu_clearScreen:
	call ds2cs
	mov dx, 0405h
	call tty_setCur
	mov ax, 24
	call tty_clearLines
ret



logo1 db "    __  ___                            _______  ________",0
logo2 db "   /  |/  /___  ________  ____  ____ /  ___/ _\/ _/_  _/",0
logo3 db "  / /|_/ / __ \/ ___/ _ \/ __ \/ __ \ \_ \/ /// /  / /  ",0
logo4 db " / /  / / /_/ / /  /  __/ / / / /_/ /__/ / /// _/ / /   ",0
logo5 db "/_/  /_/\____/_/   \___/_/ /_/\____/ \__/\__/\/   \/    ",0

