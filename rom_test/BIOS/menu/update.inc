;-----------------------------------------------------------------
; 1.          p_upd_ready: copia o codigo de copia para RAM e salta para la
; 2.  p_upd_verifyFileXor: verifica os dados do ficheiro de update
; 3. p_upd_verifyFileInfo: imprime pacote
; 4.     p_upd_verifyFile: verifica o ficheiro de update
; 5.  p_upd_verifyFileHdr: verifica o header de update
; 6.     p_upd_copyToPROM: codigo a ser executado na RAM para instalar update

UPDATE_BASESEG		   EQU 700h                 ; seg. base
UPDATE_COPYSEG		   EQU (UPDATE_BASESEG-20h)	; seg. código de cópia

updCorrectHdrSign      db "xtupdate"
updCorrectHdrSignSz    EQU $-offset updCorrectHdrSign

; segmento e formato do ficheiro de update
; o ficheiro de update começa assim
UPDATE_SEG SEGMENT AT  (UPDATE_BASESEG+60h)
	updHdrSign		  	db  8 dup(?)
	updHdrDate		  	db  3 dup(?)
	updHdrTime		  	db  3 dup(?)
	updHdrXor		  	db  ?
	updHdrNumPackets	db	?
	updHdrTotalBytes    dw	?
	updHdrTotalBytesHi	dw	?
	updHdrData			db  ?
	updHdrSz			EQU 20 ;($-... nao funciona)
UPDATE_SEG	ENDS
; depois vem pacotes se updPackSign='D' indica dados
; se e updPackSign='E' indica fim
	updPackSign		  	EQU 0
	updPackOffset		EQU 1
	updPackSegment		EQU 3
	updPackTotalBytes   EQU 5
	updPackTotalBytesHi	EQU 7
	updPackXor			EQU 9
	updPackData			EQU 10

;-----------------------------------------------------------------
; 1. p_upd_ready: copia o codigo de copia para RAM e salta para la
p_upd_ready:
	; copiar codigo de copia de DS:SI para ES:DI
	mov ax, offset p_upd_copyToPROM
	shr ax, 4
	mov bx, cs
	add ax, bx
	mov ds, ax
	xor si, si
	mov cx, p_upd_copyToPROMSz
	;
	mov ax, UPDATE_COPYSEG
	mov es, ax
	xor di, di
	rep movsb	; es:[di++]=ds:[si++]

	call p_menu_clearScreen
	M_tty_cwrStr "Os dados estao preparados na RAM para serem escritos na PROM."
	M_tty_cwrStr "Um codigo de copia foi preparado na RAM."
	call tty_crlf
	M_tty_wrStr "              - este codigo tem o tamanho de "
	mov ax, p_upd_copyToPROMSz
	call tty_bytesFormat
	call tty_crlf
	call tty_crlf
	M_tty_cwrStr "O SWITCH acima da PROM impede de ser escrita"
	M_tty_cwrStr "e deve ser deslocado no sentido para o INTERIOR."
	M_tty_cwrStr "Depois de reprogramar a PROM o sistema vai"
	M_tty_cwrStr "lancar 3 beeps, e imprimir se sucesso."
	M_tty_cwrStr "Nesta altura deve apagar-se o sistema"
	M_tty_cwrStr "e repor-se o SWITCH para a sua posicao anterior"
	M_tty_cwrStr "de nao-escrita."
	call p_menu_dialog

	; saltar para a RAM
	db 0EAh
	dw 0
	dw UPDATE_COPYSEG


;-----------------------------------------------------------------
; 2. p_upd_verifyFileXor: verifica os dados do ficheiro de update
	; es:di, cx=tamanho
p_upd_verifyFileXor:
	push cx
	push di
		xor ax, ax
		p_menu_vufx:
			xor al, es:[di]
			inc di
		loop p_menu_vufx
		or al, al
	pop di
	pop cx
ret


;-----------------------------------------------------------------
; 3. p_upd_verifyFileInfo: imprime pacote
p_upd_verifyFileInfo:
		M_tty_wrStr " destino="
	mov bx, es:[di+updPackSegment]
	mov ax, bx
	call tty_Hex16
	M_tty_wrStr ":"

	mov ax, es:[di+updPackOffset]	
	call tty_Hex16

	M_tty_wrStr " xor="
	mov al, es:[di+updPackXor]	
	call tty_Hex8
	
	M_tty_wrStr " tamanho="
	mov cx, es:[di+updPackTotalBytes]
	mov ax, cx
	call tty_U16
	call tty_bytes
	
ret
;-----------------------------------------------------------------
; 4. p_upd_verifyFile: verifica o ficheiro de update
p_upd_verifyFile:
	call p_menu_clearScreen
	M_tty_cwrStr "A verificar..."
	call tty_crlf
	mov ax, UPDATE_SEG  ; es:di = atualizacao
	mov es, ax
	mov di, offset updHdrData
	mov dx, 0

	loopPackets:
		call tty_crlf
		M_tty_wrStr " - Pacote "
		mov al, es:[di+updPackSign]
		call tty_char
		inc dx
		 
		cmp al, 'E'
		je loopPacketsEnd
		cmp al, 'D'
		je loopPacketsNext
		jmp loopPacketsError1
		
		loopPacketsNext:
			call p_upd_verifyFileInfo

			call p_upd_verifyFileXor
			pushf
				M_tty_wrStr " xsum= "
				call tty_Hex8
			popf
			jnz loopPacketsError2
			call tty_sp
			call tty_ok

		add di, cx
	jmp loopPackets
	loopPacketsEnd:
		call tty_sp
		call tty_ok
		
	call tty_crlf
	call tty_crlf
	M_tty_wrStr "   esperados: "
	mov al, es:[updHdrNumPackets]
	mov cl, al
	xor ch, ch
	call tty_U8 
	M_tty_wrStr " lidos: "
	mov al, dl
	call tty_U8 
	cmp cx, dx 
	jne loopPacketsError3
	
	call p_menu_dialog
	clc
ret
loopPacketsError1:
	M_tty_wrStr " - Erro de id"
	call p_menu_errorDialog
loopPacketsError2:
	M_tty_wrStr " - Erro de integridade"
	call p_menu_errorDialog
loopPacketsError3:
	call tty_crlf
	M_tty_cwrStr "Numero nao corresponde"
	call p_menu_errorDialog
;-----------------------------------------------------------------
; 5.  p_upd_verifyFileHdr: verifica o header de update
p_upd_verifyFileHdr:
	call ds2cs
	M_tty_cwrStr "Confirme para atualizar..."

	M_mov es, UPDATE_SEG
	mov di, offset updHdrSign
	M_mov ds, cs
	mov si, offset updCorrectHdrSign
	mov cx, updCorrectHdrSignSz
	repe cmpsb
	or cx, cx
	jz p_upd_verifyFileHdr1

		M_tty_cwrStr "Erro de assinatura"
		M_tty_wrStr "obtido: "
			M_mov ds, UPDATE_SEG
			M_mov es, UPDATE_SEG
			xor si, si
			xor di, di
			call tty_buffer
			hlt_
			xor al, al
			mov bx, updCorrectHdrSignSz
			mov ds:[bx], al
			call tty_str
			call ds2cs
			call p_menu_errorDialog 

	p_upd_verifyFileHdr1:

	call ds2cs
	call tty_crlf
	M_tty_wrStr "    - A data da currente BIOS e' de "
	push es 
		IF TEST_MENU EQ 0 ; SE DOSBOX não existe isto
			call p_sys_prn_dateTime 
		ENDIF
	pop es
	call tty_crlf
	M_tty_wrStr "    - A da pretendida BIOS a instalar e' de "
			mov di, offset updHdrDate
			call tty_prnDate
			call tty_sp
			mov di, offset updHdrTime
			call tty_prnTime
			call tty_sp
	call tty_crlf
	M_tty_wrStr "    - Numero de pacotes e' "
		mov al, byte ptr es:[offset updHdrNumPackets]
		call tty_U8
	call tty_crlf
	M_tty_wrStr "    - Total a receber e' "
		mov ax, word ptr es:[offset updHdrTotalBytes]
		call tty_bytesFormat	; <!>
		
	call p_menu_dialog
ret


;-----------------------------------------------------------------
; 6. p_upd_copyToPROM: codigo a ser executado na RAM para instalar update
;                      entra cx o tamanho em blocos de 512b
align 16
p_upd_copyToPROM:
	
	jmp p_upd_copyToPROM1
		; copiar codigo de copia de DS:SI para ES:DI
		upd_strError: db "Erro de assinatura!",0
		upd_strErrorLength EQU  ((offset $ - offset upd_strError)/2)*2
		upd_strOk:    db "Atualizacao ok", 0
		upd_strOkLength   EQU  ((offset $ - offset upd_strOk)/2)*2
	p_upd_copyToPROM1:

	mov ax, UPDATE_SEG		; DS:SI = origem dos dados
	mov ds, ax
	xor si, offset updHdrData
							; LER À MEDIDA E FAZER XOR
	update_loop:
		lodsb
		cmp al, 'E'			; P (pacote) E (fim)
		jmp update_loopGood
		cmp al, 'D'
		jne update_loopError
		lodsw				; ler SI=updPackOffset
		mov di, ax			
		lodsw				; ler ES=updPackSegment
		mov es, ax
		lodsw				; ler CX=updPackTotalBytes
		mov cx, ax
		lodsw				; ler DX=updPackTotalBytesHi
		mov dx, ax
		lodsb				; ler ...=updPackXor
		update_loopDx:
			push cx
				rep movsb	; es:[di++]=ds:[si++]
			pop cx
			; mais de 64K? aumentar por segmentos
				mov ax, es
				add ax, 1000h
				mov es, ax
			dec dx
		jnz update_loopDx
		
	update_loopGood:
		mov si, offset upd_strOk - offset p_upd_copyToPROM
		mov cx, upd_strOkLength
		mov ax, 2020h
		jmp update_finish1
	update_loopError:
		mov si, offset upd_strError - offset p_upd_copyToPROM
		mov cx, upd_strErrorLength
		mov ax, 4F20h
	update_finish1:
		M_RTC_Set_ RTC_RAM_ROMISNEW, 1	; por no RTC que é nova a BIOS
		xchg bp, cx
		M_text_clearScreen 
		xchg bp, cx
		mov ax, cs
		mov ds, ax
		mov di, (11*(80*2))+40*2
		sub di, cx
		M_text_wrStr 2, upd_strOk
		M_beep 0FFFFh
		M_Countdown ax, 0FFFFh
		M_beep 0FFFFh
		M_Countdown ax, 0FFFFh
		M_beep 0FFFFh
		M_Countdown ax, 0FFFFh
		hlt
p_upd_copyToPROMSz EQU $ - offset p_upd_copyToPROM

