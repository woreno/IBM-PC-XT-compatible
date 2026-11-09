
	;; fazer o dump de memoria por porta serie
	;M_fakeCall p_sys_configSerial	; configurar
	;
	;mov cx, 16
	;mov dx, SERIAL_RXTX			; limpar buffer
	;loop_dummy1:
	;	in al, dx
	;	loop loop_dummy1
	;
	;; enviar 512KB memoria (até 8000h)
	;mov cx, 0
	;nmi_mem1:
	;	mov ds, cx
	;	xor si, si
	;	M_DispDigits16 cx, bx		; mostrar o segmento
	;	xor bx, bx					; bx roda inclusive até aos 64KB
	;	nmi_mem2:
	;		M_SERIAL_WAIT_TX		; esperar que o buffer TX livre 
	;		mov al, ds:[bx]
	;		mov dx, SERIAL_RXTX
	;		out dx, al				; enviar byte
	;		xor si, ax				; XOR
	;		inc bx					; próximo byte
	;		jnz nmi_mem2
	;		
	;	M_SERIAL_WAIT_TX			; esperar 
	;	mov dx, SERIAL_RXTX
	;	mov ax, si
	;	out dx, al					; enviar xsum
	;
	;	M_SERIAL_WAIT_RX			; esperar rx
	;	mov dx, SERIAL_RXTX
	;	in al, dx
	;
	;	add cx, 32					; próximos 512bytes
	;	cmp cx, 8000h				; chegou aos 512KB?
	;	jne nmi_mem1
	;	
	;	; enviar 16KB de video
	;	;mov cx, 0B800h
	;	;M_DispDigits16 cx, bx		; mostrar o segmento
	;	;mov cx, UNIT_16K			; 16KB
	;	;xor bx, bx
	;	;nmi_mem3:
	;	;	M_SERIAL_WAIT_TX		; esperar que o buffer TX livre 
	;	;	mov al, ds:[bx]			
	;	;	mov dx, SERIAL_RXTX		; enviar byte
	;	;	out dx, al
	;	;	inc bx					; próximo byte
	;	;	loop nmi_mem3
