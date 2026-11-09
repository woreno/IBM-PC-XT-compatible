
p_hdd_prnCHSfromRegs:
push ax
push cx
push dx

	M_tty_ChgAtt 047h
	push ax
		mov al,'D'
		call tty_char
		mov al,'R'
		call tty_char
		mov al,'V'
		call tty_char
		mov al,'='
		call tty_char
		mov al, dl
		call tty_Hex8
	pop ax
	M_tty_ChgAtt 007h
	call tty_crlf	
	
	cmp ah, 00			; p_hdd_reset
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 01			; p_hdd_status
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 09			; p_hdd_init
	je p_hdd_prnCHSfRegsSkip
	cmp ah, 08			; p_hdd_params
	je p_hdd_prnCHSfRegsSkip

	jmp p_hdd_prnCHSfRegsCont

	p_hdd_prnCHSfRegsSkip:
		pop dx
		pop cx
		pop ax
		ret

	p_hdd_prnCHSfRegsCont:

	;-------------------------
	push cx
		shr cl, 6
		xchg cl, ch
		M_tty_wrFieldChar16 'C', cx
	pop cx

	;-------------------------
	M_tty_wrFieldChar8 'H', dh
	;-------------------------
	and cl, 03Fh
	M_tty_wrFieldChar8 'S', cl
	;-------------------------
	M_tty_wrFieldChar '#'
	call tty_Hex8
	call tty_sp
	xor ah, ah
	call tty_sectsToKB
	IF TTY_MODE EQ 0
		call tty_crlf
	ENDIF
	;-------------------------
	call tty_sp
	mov al, '>'
	call tty_char
	mov al, '>'
	call tty_char
	M_tty_ChgAtt 047h
	mov ax, es
	call tty_Hex16
	mov al, ':'
	call tty_char
	mov ax, bx
	call tty_Hex16
	M_tty_ChgAtt 007h
	call tty_sp
	call tty_sp 
pop dx
pop cx
pop ax
ret