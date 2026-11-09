RLE_SIGNATURE   EQU 0
RLE_SIZE	    EQU 3

RLE_DEBUG		EQU 0
RLE_DEBUG_KEYB	EQU 0

rle_check_sign:
	mov al, 2
	mov si, RLE_SIGNATURE
	cmp byte ptr ds:[si+0], 'R'
	jne rle_check_sign1
	cmp byte ptr ds:[si+1], 'L'
	jne rle_check_sign1
	cmp byte ptr ds:[si+2], 'E'
	jne rle_check_sign1
		clc
		ret
	rle_check_sign1:
		M_tty_wrStr "RLE: erro assinatura"
		stc
	ret
	
	; não usa STACK
	; ds:[si] tem a compressão
	; es:[di] tem o local de destino
rle_uncompress:
	cld
	call rle_check_sign
	jnc rle_uncompressCont
		ret
	rle_uncompressCont:

	IF RLE_DEBUG EQ 1
		mov ax, 7C0h    ; se em DEBUG
		mov es, ax    	; descomprimir sempre 
		xor di, di		; para o mm sitio!
	ENDIF
	
	mov si, RLE_SIZE
	lodsw	; AX=tamanho comprimido (4 bytes)
	lodsw	
	mov cx, ax	; CX=tamanho descomprimido (sem header)
	lodsb		; ds:[si++]
	xchg ah, al	; AH="código RLE"

	rle_uncomp_loop:
		lodsb			; AL = byte, AH = "código RLE"
		cmp al, ah		; É código RLE?
		jne rle_uncomp_isNormal

									; Foi código RLE	
			lodsb					; AL = repetições
			or al, al
			jz rle_uncomp_noRep		; são ZERO repetições?
				
							; TEM repetições:
				mov dl, al	; DL=repetições
				lodsb		; AL=byte a repetir
				
				IF RLE_DEBUG EQ 1 
					call rle_debug_reps
				ENDIF

				rle_uncomp_its:
					IF RLE_DEBUG EQ 1 
						inc di
					ELSE
						stosb		; AL = es:[di++]
					ENDIF
					dec cx
					dec dl
				jnz rle_uncomp_its

				or cx, cx			; se prefez tamanho
				jz rle_uncomp_end	; sair
				jmp rle_uncomp_loop	; senao ir para o proximo

		rle_uncomp_noRep:			; REPETIÇÕES=0, ENTÃO POR AL=AH
									; E CONTINUAR como fosse "NORMAL"
			IF RLE_DEBUG EQ 1 
				call rle_debug_iscode
			ENDIF
			mov al, ah

		rle_uncomp_isNormal:		; se não código guardar byte
			IF RLE_DEBUG EQ 1
				call rle_debug_isbyte
				inc di
			ELSE 
				mov es:[di], al
				inc di				; AL = es:[di++]
			ENDIF

		loop rle_uncomp_loop	
		rle_uncomp_end:
			IF RLE_DEBUG EQ 1
				call rle_debug_end
			ENDIF
ret


IF RLE_DEBUG EQ 1
	rle_debug_reps:
	push ds
		call ds2cs
		M_tty_wrStr "rep. - offset:"
		push ax
			mov ax, si
			call tty_Hex16
		pop ax
		call tty_sp
		M_tty_wrStr " size:"
		push ax
			mov al, dl
			call tty_U8
		pop ax
		call tty_sp
		M_tty_wrStr " byte:"
		call tty_Hex8
		call tty_sp
		call tty_crlf
		IF RLE_DEBUG_KEYB EQ 1
			M_debugKeyb
		ENDIF
	pop ds
	ret

	rle_debug_iscode:
	push ds
		call ds2cs
		M_tty_wrStr "rep. - offset:"
		push ax
			mov ax, si
			call tty_Hex16
		pop ax
		call tty_crlf
		IF RLE_DEBUG_KEYB EQ 1
			M_debugKeyb
		ENDIF
	pop ds
	ret

	rle_debug_isbyte:
		call tty_Hex8
		call tty_sp
		IF RLE_DEBUG_KEYB EQ 1
			M_debugKeyb
		ENDIF
	ret

	rle_debug_end:
	push ds
		call ds2cs
		call tty_crlf
		M_tty_wrStr "fim"
	pop ds
	ret
ENDIF
