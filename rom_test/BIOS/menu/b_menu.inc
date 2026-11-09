; 1. p_menuShow: mostra as páginas e as opções
; 2. p_menu_displayOption: mostrar a opção
; 3. p_menu_ENTER: encima duma opção e se CARREGA EM ENTER

;-----------------------------------------------------------------
			; se B_MENU_DEBUG=1 usa uma array para
			; ler/gravar em vez do RTC
B_MENU_DEBUG          EQU   0
include m_def.inc
include m_ops.inc
include m_crt.inc
include m_clk.inc
include m_sbp.inc
include m_tests.inc
;include m_wifi.inc
include update.inc
include upd_file.inc
include upd_com.inc
;-----------------------------------------------------------------
; 1. p_menuShow: mostra as páginas e as opções
p_menuShow:
	IF TEST_MENU					; DOSBOX
		mov ax, offset sstack
		shr ax, 4
	ELSE
		mov ax, STACKSEG
	ENDIF
	mov ss, ax
	mov sp, STACKTOP

	call beep
	call tty_clear
	
	call p_menu_clear
	call ds2cs
	mov si, offset bmenu_title
	call p_menu_setTitle
	mov si, offset bmenu_escstr
	call p_menu_setFooter 
	
	xor cx, cx						; CL=PAGINA=0
	xor bx, bx
	xor dx, dx

	p_menuShow_start0:			
		xor ax, ax						; AH=OPÇÃO=0
		call p_menu_showPages 			; CL: PÁGINA - mostra separadores
		call p_menu_showAllOps			; mostra opcoes
		
		p_menuShow_update:	
			push ax
				call p_menuShow_keys			; esperar tecla
			pop dx
				jc p_menuShow_start0
			push ax
				mov al, dh	 ; desenhar anterior selecionada
				mov ah, 0FFh ; sem selecao
				call p_menu_displayOption		; mostrar nova opcao selecionada
			pop ax
			mov al, ah
			call p_menu_displayOption		

		jmp p_menuShow_update


p_menu_showAllOps:						; mostrar todas as opções
		mov al, 0						; rodar indices AL
		
		p_menu_showAllOps1:
			call p_menu_displayOption
			jc p_menu_showAllOpsExit			

		inc al
		jmp p_menu_showAllOps1

	p_menu_showAllOpsExit:
		ret


;-----------------------------------------------------------------
; 2. p_menu_displayOption: mostrar a opção
; entra
;			DI = PAGINA
;			AL = OPCAO NA PAGINA A MOSTRAR
;			AH = OPCAO SELECIONADA CURRENTEMENTE
;
p_menu_displayOption:
	M_push6 ax,bx,cx,dx,si,di
	push ax
		mov dh, 5				 
		add dh, al
		xor ah, ah
		shl ax, 1
		add di, ax
		mov si, word ptr cs:[di]
	pop ax
	
		;  SI  = um dos offsets em bk_ops, etc (o ultimo a ZERO)
		; [SI] = Tipo de Opção, string da Opção

	or si, si					
	jnz p_menu_displayOptionCont

		M_pop6 ax,bx,cx,dx,si,di
		stc
		ret 

	p_menu_displayOptionCont:

	mov bl, byte ptr cs:[si]	; BL=TIPO DE OPÇÃO
								; SE OPÇÃO IGUAL AO INDICE
								; ILUMINAR
	push bx
								; APAGAR LINHA TOTALMENTE
		mov bl, B_MENU_BACKCOLOR
		call tty_setAtt
		call tty_setCur
		push ax
			mov ax, 1
			call tty_clearlines
		pop ax
								; MUDAR ATRIBUTO SE SELECIONADA
		cmp al, ah
		jne p_menu_displayOption2
			mov bl, B_MENU_SELCOLOR
			p_menu_displayOption2:
		call tty_setAtt
	pop bx

	inc si						; SI=STRING
	M_push3 ax, cx, di
		call strlen 
		cmp bh, B_OPTION_CALL
		jne p_menu_displayNoSp	
			mov di, si
			add di, cx			; há calls que não tem print
			inc di				; estas ficam centradas
			mov ax, cs:[di]
			or ax, ax
			jnz p_menu_displayNoSp
				mov dl, 80
				sub dl, cl
				shr dl, 1
				jmp p_menu_displayNoSp1
		p_menu_displayNoSp:		; todas as restantes ficam
		mov dl, 40				; alinhadas + o seu print
		sub dl, cl
		p_menu_displayNoSp1:
		call tty_setCur
		call tty_Str
		add si, cx				; AVANCAR ZERO
		inc si					; SI=PROXIMA POSIÇÃO
		or bl, bl				; BL=TIPO DE OPÇÃO
		shl bx, 1				; CHAMAR FUNCAO DE PRINT
	M_pop3 ax, cx, di
	call word ptr cs:[p_menu_prnCalls+bx]
	M_pop6 ax,bx,cx,dx,si,di
		
	clc
ret

;-----------------------------------------------------------------
; 3. p_menu_ENTER: encima duma opção e se CARREGA EM ENTER
p_menu_ENTER:

	M_push4 ax, cx, bx, dx
	mov bh, cs:[si]				; SI = uma lista
	and bh, 07Fh 				; BH = TIPO DE MENU
	inc si
	push cx	
		call strlen
		add si, cx
		inc si					; SI = PROXIMA POSICAO
	pop cx

	cmp bh, B_OPTION_CALL
	jne p_menu_action_Enum
								; se for call
		M_push4 ax, bx, cx, dx
			call word ptr cs:[si+2]	; SI = chamada	
			; regressar para p_menuShow <------------------------------------
			mov bp, sp
			mov ss:[bp], offset p_menuShow
			ret near
				
	p_menu_action_Enum:
		mov bl, byte ptr cs:[si]	; REGISTO RTC
		M_RTC_Read_ bl				; AL=VALOR
		inc al						; AL++
		
		cmp bh, B_OPTION_ENUM		; ERA BH=ENUMERAÇÃO?
		je p_menu_action_Enum2
			and al, 1				; NÃO, BOOL, fazer AND
			M_RTC_Set_ bl, al		; DEFINIR NO RTC
			jmp p_menu_action_End
	
	p_menu_action_Enum2:
		inc si						; BH=É ENUMERAÇÃO
		M_RTC_Set_ bl, al			; AL=VALOR
		mov ah, cs:[si]				; AH=Nº de OPCOES	
		cmp al, ah
		jb p_menu_action_End		; CHEGOU AO LIMITE, ZERAR
		M_RTC_Set_ bl, 0 

	p_menu_action_End:
	M_pop4 ax, bx, cx, dx
ret
 

reset:
	call p_menu_clear
	mov dx, 0800h
	call tty_setCur
	mov si, offset logo1
	call tty_centeredStr
	call tty_crlf
	mov si, offset logo2
	call tty_centeredStr
	call tty_crlf
	mov si, offset logo3
	call tty_centeredStr
	call tty_crlf
	mov si, offset logo4
	call tty_centeredStr
	call tty_crlf
	mov si, offset logo5
	call tty_centeredStr
	call tty_crlf
	call LonglongWait
	call LonglongWait
	call LonglongWait
	call LonglongWait
	call LonglongWait
	call LonglongWait
	M_resetJump
ret
