; formato ACOMP header mestre
ACP_SIGNATURE		EQU 0
ACP_TOTALSIZE_L		EQU 3
ACP_TOTALSIZE_H		EQU 5
ACP_SAMPRATE_L		EQU 7
ACP_SAMPRATE_H		EQU 9
ACP_TOTALHEADERS	EQU 11
ACP_HEADERS_START	EQU 13

; formato dos headers
ACP_HEADER_SIZE		EQU 0
ACP_HEADER_OPTIONS	EQU 2
ACP_HEADER_NEXT		EQU 3

ACP_DEBUG			EQU 0
ACP_DEBUG_PARAMS	EQU 0
ACP_DEBUG_KEYB		EQU 0
ACP_DEBUG_BYTES		EQU 0

acp_check_sign:
	mov si, ACP_SIGNATURE
	cmp byte ptr ds:[si+0], 'A'
	jne acp_check_sign1
	cmp byte ptr ds:[si+1], 'C'
	jne acp_check_sign1
	cmp byte ptr ds:[si+2], 'P'
	jne acp_check_sign1
		clc
		ret
	acp_check_sign1:
		call ds2cs
		M_tty_wrStr "ACP: erro assinatura"
		stc
		ret
;--------------------------------------------------------------------------
	; ds:[0] - ficheiro em memoria comprimido
	; es:[0] - memoria para descompressao
acp_uncompress:		
	cld
	call acp_check_sign
	jnc acp_uncompressCont
		ret
	acp_uncompressCont:	
	mov si, ACP_HEADERS_START
	xor di, di
	mov dx, 0FFFFh

	acp_loopHeaders:
		inc dx
		mov cx, word ptr ds:[si+ACP_HEADER_SIZE]	; tamanho
		mov al, byte ptr ds:[si+ACP_HEADER_OPTIONS] ; opções 
		add si, ACP_HEADER_NEXT
		
		IF ACP_DEBUG EQ 1
			call acp_debug1
		ENDIF

		or cx, cx		; se tamanho 0 acabou
		jz acp_loopHeaders_Finish

		test cx, 8000h
		jz acp_loopHeaders_Constant
			and cx, 07FFFh	; retirar bit alto
			push dx
				call acp_Data
			pop dx
			jmp acp_loopHeaders
		
		acp_loopHeaders_Constant:
			and cx, 07FFFh	; retirar bit alto
			push dx
				call acp_Constant
			pop dx
			jmp acp_loopHeaders

	acp_loopHeaders_Finish:
		IF ACP_DEBUG EQ 1
			call ds2cs
			call tty_crlf
			M_tty_wrStr "acp terminado"	
		ENDIF
ret
;--------------------------------------------------------------------------
	; es:[di] localização
	; al = opções ( bits<<4 | mult), <sendo mul-1>
	; cx = tamanho apartir de ds:[si] - 1 byte (compacto)
acp_Data:
	push ax
	push 0 
	ACPSTK_BYTESUNC EQU 10 ; bytes descomprimidos
	push 0 
	ACPSTK_SIGNMASK EQU 8 ;máscara sinal
	push 0 
	ACPSTK_BITMASK EQU 6  ;máscara do valor
	push 0 
	ACPSTK_MULT EQU 4     ;multiplicador
	push 0	
	ACPSTK_BITS EQU 2     ;nº de bits
	push bp
	ACPSTK_0 EQU 0        
	mov bp, sp 
	
	dec cx	; descontar 1 byte ao nº total de bytes

	mov word ptr ss:[bp + ACPSTK_BYTESUNC], 0

	push cx					; preencher opções na STACK	
		mov ch, al
		mov cl, al
		shr cl, 4			; cl = bits
		and ch, 0Fh			; bl = multiplicador (+1)
		inc ch
		mov byte ptr ss:[bp+ACPSTK_BITS], cl
		mov byte ptr ss:[bp+ACPSTK_MULT], ch

		; criar as máscaras
		dec cl			
		mov ax, 1         ; 1<<(bits-1)
		shl ax, cl
		mov byte ptr ss:[bp+ACPSTK_SIGNMASK], al
		dec ax			 ; 1<<(bits-1)-1 	
		mov byte ptr ss:[bp+ACPSTK_BITMASK], al
	pop cx
	
	IF ACP_DEBUG_PARAMS EQ 1
		call acp_debugparams
	ENDIF

							;---------------------------------------
							; AL é o BYTE CODIFICADO
							; BH é o ULTIMO VALOR DESCOBERTO
							;---------------------------------------

	mov al, ds:[si]
	mov bh, al				; BH = (PRIMEIRO VALOR=PRIMEIRO BYTE)
	movsb					; GUARDAR es[di++]=ds:[si++]
	call checkEsDiPointer
	call checkDsSiPointer
	
	IF ACP_DEBUG_BYTES EQ 1
		call tty_Hex8	
		call tty_sp
	ENDIF

	inc word ptr ss:[bp + ACPSTK_BYTESUNC]
	or cx, cx
	jz acp_Data_End


	acp_Data_Loop:
		lodsb				; AL = ds:[si++] (BYTE=VALORES CODIFICADOS)
		call checkDsSiPointer

		push cx
			mov cl, byte ptr ss:[bp+ACPSTK_BITS]		; cl=nº de bits
			mov bl, byte ptr ss:[bp+ACPSTK_BITMASK]     ; bl=máscara de N
			mov ch, 0											; ch=contador de conjuntos
																;    incrementa pelo
																;    nº de bits
		acp_Data_LoopDigits:
			; aplicar mascara de bits
			push ax
				and al, bl		; mascarar AL, (BYTE CODIFICADO)
				xor ah, ah		; com a MASCARA DE BITS
				inc al			; ADICIONAR 1
				
				IF ACP_DEBUG_BYTES EQ 1
					push ax
						mov al, '('
						call tty_char
					pop ax
						call tty_Hex8
				ENDIF

				mov dl, byte ptr ss:[bp+ACPSTK_MULT]
				mul dl 
				mov dl, al		; DL= mul*n
								; DL tem o resultado da multiplicação
								
				IF ACP_DEBUG_BYTES EQ 1
					push ax
						mov al, '>'
						call tty_char
					pop ax
						call tty_Hex8
					push ax
						mov al, ')'
						call tty_char
					pop ax
				ENDIF

			pop ax	
									
								; mascarar AL, (BYTE CODIFICADO)
								; com a MASCARA DE SINAL se POSITIVO
			push cx				; TORNAR DL NEGATIVO
				mov cl, byte ptr ss:[bp+ACPSTK_SIGNMASK]
				test al, cl
				jz acp_Data_LoopNoSign
					not dl				; COMPLEMENTAR 
					inc dl
				acp_Data_LoopNoSign:
					add bh, dl			; ADICIONAR DL a BH (NOVO VALOR)

			pop cx

			mov es:[di], bh		; E GUARDAR NOVO VALOR

			IF ACP_DEBUG_BYTES EQ 1
				push ax
					mov al, bh
					call tty_Hex8
					call tty_sp
					xor ax, ax
					INT 16h
				pop ax	
			ENDIF

			inc di
			call checkEsDiPointer

			shr al, cl			; DESLOCAR AL (BYTE CODIFICADO)
								; APARTIR DAQUI BH é USADO como 
								; VALOR ANTERIOR

								; INCREMENTAR CONTADOR DE BYTES ESCRITOS
			inc word ptr ss:[bp + ACPSTK_BYTESUNC]

			add ch, cl			; ADICIONAR Nº DE BITS a CH
			cmp ch, 8			; SE ACABOU O BYTE CODIFICADO, PROXIMO
			jne acp_Data_LoopDigits
		pop cx
	loop acp_Data_Loop			; SE PREFEZ CX, SAIR
	acp_Data_End:

		IF ACP_DEBUG EQ 1
			push ds
				call ds2cs
				M_tty_wrStr " bytes decomp:"
				mov ax, word ptr ss:[bp + ACPSTK_BYTESUNC]
				call tty_U16
			pop ds
		ENDIF

	pop bp
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
	pop ax
ret
	
;--------------------------------------------------------------------------
	; es:[di] localização
	; al = byte a repetir
	; cx = tamanho
acp_Constant:
	stosb					; es:[di++]=AL
	call checkEsDiPointer
	loop acp_Constant
ret
;--------------------------------------------------------------------------

;--------------------------------------------------------------------------

checkEsDiPointer:
	or di, di
	jnz checkEsDiPointerSkip
		mov di, es
		add di, UNIT_IN_SEGS_64K
		mov es, di
		xor di, di
	checkEsDiPointerSkip:
ret

checkDsSiPointer:
	or di, di
	jnz checkDsSiPointerSkip
		mov si, ds
		add si, UNIT_IN_SEGS_64K
		mov ds, si
		xor si, si
	checkDsSiPointerSkip:
ret


	; SE O HEADER MZ DO EXECUTÁVEL, TIVER MAXALLOC=FFFFh
	; (default) não vai ser POSSIVEL ALOCAR!
	; entra em DS:SI, o offset para o audio comprimido
	; sai em AX, o 1º segmento da alocação
dos_alloc_sound:
	;mov ax, word ptr ds:[si + ACP_TOTALSIZE_H]
	;mov dx, ax
	
	mov bx, ax
	mov ax, word ptr ds:[si + ACP_TOTALSIZE_L]
	shr ax, 4			; /16
	test bx, 1111b		; se houver resto
	jz dos_alloc_sound_cont
		 inc ax
	dos_alloc_sound_cont:

	mov bx, ax		; BX  = numero de paragrafos a alocar
	mov ah, 48h		; DOS: alocar paragrafos
	int 21h 
	jc alloc_sound_error
		ret
	alloc_sound_error:
		call ds2cs
		call tty_crlf
		M_tty_wrStr "!!! nao consigo allocar - erro"
		call tty_Hex16
		M_tty_wrStr " maior disp:"
		mov ax, bx
		call tty_Hex16
		hlt


	
IF ACP_DEBUG EQ 1
	acp_debug1:
	push ax
	push ds
		call ds2cs
		call tty_crlf
		push ax
			M_tty_wrStr "quadro:"	
			mov ax, dx
			call tty_U8time
			call tty_sp
			mov ax, cx
			M_tty_wrStr " tipo:"	 	; imprimir tipo
			test ax, 8000h
			jz acp_debug1_1
				mov al, 'D'
				jmp acp_debug1_2
			acp_debug1_1:
				mov al, 'S'
			acp_debug1_2:
			call tty_char
			call tty_sp
		pop ax
			M_tty_wrStr " op:"	
			call tty_Hex8
		push ax
			call tty_sp
			M_tty_wrStr " st:"
			mov ax, si			
			call tty_Hex16
			call tty_sp
			M_tty_wrStr " sz:"	
			mov ax, cx			; imprimir tamanho
			and ax, 7FFFh
			call tty_U16
			
			IF ACP_DEBUG_KEYB EQ 1
				xor ax, ax
				INT 16h
			ENDIF
			call tty_crlf			
		pop ax
	pop ds
	pop ax
ret

acp_debugparams:
	push ax
	push ds
		call ds2cs
		call tty_crlf
		M_tty_wrStr " mult:"
		mov al, byte ptr ss:[bp + ACPSTK_MULT]
		call tty_U8
		M_tty_wrStr " bits:"
		mov al, byte ptr ss:[bp + ACPSTK_BITS]
		call tty_U8
		M_tty_wrStr " bitmask:"
		mov al, byte ptr ss:[bp + ACPSTK_BITMASK]
		call tty_Hex8
		M_tty_wrStr " signmask:"
		mov al, byte ptr ss:[bp + ACPSTK_SIGNMASK]
		call tty_Hex8
	pop ds
	pop ax
ret

ENDIF
