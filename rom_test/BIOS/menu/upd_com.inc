; 1. serial_init: inicializa por INT14h a porta serie
; 2. serial_tx: transmite ds:si, tamanho cx
; 3. serial_rx: recebe em es:di, tamanho cx
; 4. serial_txAck: manda a string deste cliente (helloClientStr)
; 5. serial_rxAck: espera pela string do servidor (helloServerStr)
; 6. p_upd_serial: atualizacao

INT14_INIT    EQU 0
INT14_TX      EQU 1
INT14_RX      EQU 2
INT14_STATUS  EQU 3
;-----------------------------------------------------------------
RXTEMP_SEG SEGMENT AT 7C0h
	rxBytes				dw	?
	rxAckString			db	?
RXTEMP_SEG ENDS
;-----------------------------------------------------------------
helloClientStr: db "moreno-xtrom-client"
helloClientStrSz EQU $-offset helloClientStr
helloServerStr: db "moreno-xtrom-server"
helloServerStrSz EQU $-offset helloServerStr
;-----------------------------------------------------------------
; 1. serial_init: inicializa por INT14h a porta serie
serial_init:
	xor dx, dx
	mov al, 11100011b
	mov ah, INT14_INIT
	INT 14h	
	
	; limpar se houver para receber
	; antes de começar
	serial_init1:
		mov ah, INT14_STATUS
		INT 14h	
		test ah, 1
		jz serial_initEnd
		mov ah, INT14_RX
		INT 14h	
	jmp serial_init1
    serial_initEnd:
ret
;-----------------------------------------------------------------
; 2. serial_tx: transmite ds:si, tamanho cx
serial_tx:					; al = ds:[si++]
push si
push cx
	xor dx, dx
	serial_txWait:
		mov ah, INT14_STATUS
		INT 14h	
		test ah, 80h
		jnz serial_txError
		test ah, 20h
		jz serial_txWait
		lodsb				
		mov ah, INT14_TX
		INT 14h
		loop serial_txWait
		pop cx
		pop si
		clc
		ret
	serial_txError:
		pop cx
		pop si
		stc
		ret
;-----------------------------------------------------------------
; 3. serial_rx: recebe em es:di, tamanho cx
serial_rx:					; es:[di++] = al
push di
push cx
	xor dx, dx
	serial_rxWait:
		mov ah, INT14_STATUS
		INT 14h	
		test ah, 80h
		jnz serial_rxError
		test ah, 1b
		jz serial_rxWait
		mov ah, INT14_RX
		INT 14h
		stosb				 
	loop serial_rxWait
		pop cx
		pop di
		clc
		ret
	serial_rxError:
		pop cx
		pop di
		stc
		ret 
;-----------------------------------------------------------------
; 4. serial_txAck: manda a string deste cliente
serial_txAck:
	push ds
		mov ax, cs
		mov ds, ax
		mov si, offset helloClientStr
		mov cx, helloClientStrSz
		call serial_tx
	pop ds
ret
;-----------------------------------------------------------------
; 5. serial_rxAck: espera pela string do servidor
serial_rxAck:
	mov ax, RXTEMP_SEG
	mov es, ax
	mov di, offset rxAckString
	mov cx, helloServerStrSz
	call serial_rx
	jc serial_rxAckError
	xor cx, cx
	mov ax, cs
	mov ds, ax
	mov si, offset helloServerStr
	cld
	repe cmpsb
	jnz serial_rxAckError
		clc
		ret
	serial_rxAckError:
		stc
		ret

;-----------------------------------------------------------------
; 6. p_upd_serial: update por COM1 
p_upd_serial:
	call p_menu_clear
	mov si, offset bk04+1
	call p_menu_setTitle
	mov si, offset bm15_footer_str
	call p_menu_setFooter
	
	call p_menu_clearScreen

	call tty_crlf
	M_tty_cwrStr "Execute a aplicacao no PC servidor"
	M_tty_cwrStr "ligada `a COM1"
	
	call p_menu_dialog
	call serial_init

p_menu_upd_connect1:
	M_tty_cwrStr "A tentar ligacao `a aplicacao servidor..."
	call ds2cs
	call serial_txAck ;-----1--------
	jnc p_menu_upd_connect2
		p_update_timeout:
		call ds2cs
		M_tty_cwrStr "erro ao enviar ID"
		call p_menu_errorDialog
	
p_menu_upd_connect2:
	call serial_rxAck ;-----2--------
	jnc p_menu_upd_connect3
		
	M_tty_cwrStr "erro ao receber ID"
	call p_menu_errorDialog

p_menu_upd_connect3:
	call ds2cs
	call tty_crlf
	M_tty_cwrStr "ligacao OK"
	call beep 
	call beep 
	call p_menu_clearScreen

	call serial_txAck	;-----3--------
	mov cx, updHdrSz 	; receber preambulo
	mov ax, UPDATE_SEG
	mov es, ax
	xor di, di
	call serial_rx	 ;-----4--------
	jnc p_menu_upd_connect4
			
p_menu_upd_connect4:
	call p_upd_verifyFileHdr

	M_IRQ_OFF 0
		
	M_tty_cwrStr "Os dados estao a ser recebidos..."
	call tty_crlf
	
			; ------------- barra de progresso---		
	mov dx, 0B14h
	call tty_setCur
	mov al, 38
	call tty_progbarInit
			; -----------------------------------		

	call serial_txAck ;-----5--------

	mov di, offset updHdrData

	p_upd_rxloop:

		; receber o tamanho parcial, se zero sair ----------------
		push di
			mov ax, RXTEMP_SEG			  ; es:di = temporario
			mov es, ax
			mov di, offset rxBytes		  ; receber tamanho parcial
			mov cx, 2
			call serial_rx				  ;-----6--------	
			mov cx, word ptr es:[rxBytes] ; se zero acabou
			or cx, cx
		pop di
		jz p_upd_rxloopEnd

		; receber os dados --------------------------------------
		mov ax, UPDATE_SEG				  ; es:di = atualizacao
		mov es, ax				
		call serial_rx					  ;-----7--------	
		add di, cx
			
		; ------------- barra de progresso---
			mov al, 38
			mov dx, word ptr es:[updHdrTotalBytes]
			mov cx, di
			sub cx, offset updHdrData
			call tty_progbar
		; -----------------------------------

		call serial_txAck	;-----8--------	
	jmp p_upd_rxloop
	p_upd_rxloopEnd:
	M_IRQ_ON KEYB_IRQ		; ativar teclado
	call p_upd_verifyFile
	call p_upd_ready
ret