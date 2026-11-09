TTY_COLS			 EQU 40+(TTY_MODE*40)
TTY_ROWS			 EQU 25

ds2data:
	push ax
	mov ax, DATA
	mov ds, ax
	pop ax
ret

es2data:
	push ax
	mov ax, DATA
	mov es, ax
	pop ax
ret

ds2cs:
	push ax
	mov ax, BIOS_SEG
	mov ds, ax
	pop ax
ret

CGA_TEST_STRING:
	db "ola 0123456789 grande placa OLA!!!", 0

CGA_VIDEO_MODES:
	db  00101000b			  ;CGA 40x25
	db  00101001b			  ;CGA 80x25
	db  00101010b			  ;CGA Video

CGA_VIDEO_PARAMS:
	db	56,40,45,10,31,6,25		; CGA 40x25
	db	28,2,7,6,7	
	db	0,0,0,0
	;----
	db	113,80,90,10,31,6,25	; CGA 80x25
	db	28,2,7,6,7           
	db	0,0,0,0
	;----			
	db	56,40,45,10,127,6,100  ; CGA Video
	db	112,2,1,6,7
	db	0,0,0,0

CGA_DRAW_JUMPERRECTS: 
	dw 81 , 106, 30, 25
	dw 81 , 135, 30, 25
	dw 81,  164, 31, 25
	dw 148, 172, 46, 17
	dw 0,0,0,0
	dw 201, 154, 29, 9
	dw 205, 128, 46, 17
	dw 223, 106, 28, 17


;------------------------------------------------------------------------------
; cga_testVideoMem: testa as 40x25
cga_testVideoMem:
	mov ax, 0B800h
	mov ds, ax
	mov es, ax

	xor di, di
	mov cx, UNIT_16K
	test_vidwrit:			; testar escrita
		mov al, cl
		stosb				; es:[di++] = ax
		loop test_vidwrit

	xor si, si
	mov cx, UNIT_16K		; testar leitura
	test_vidread:
		lodsb				; ax = ds:[si++]
		cmp al, cl
		je test_vidread_ok
		test_vidread_error:
			M_DispDigits16 si, bx
			M_errorBeep 01000h
			;M_Countdown dx, 02FFFh
		test_vidread_ok:
		loop test_vidread

ret

cga_testVideoPixel:
mov al, 0
	M_fakeCall cga_clear
	mov al, 2							; config. modo de video
	M_fakeCall cga_setVideo
	mov ax, 0B800h
	mov es, ax
	xor di, di
	pxloop:
		xor al, al
		M_Countdown dx, 0FFFh
		mov al, 11000000b
		mov es:[di], al
		M_Countdown dx, 0FFFh
		mov al, 11110000b
		mov es:[di], al
		M_Countdown dx, 0FFFh
		mov al, 11111100b
		mov es:[di], al
		M_Countdown dx, 0FFFh
		mov al, 11111111b
		mov es:[di], al
		M_Countdown dx, 0FFFh
		;wait	
		M_Countdown dx, 0FFFFh
		inc di
	jmp pxloop
ret
;------------------------------------------------------------------------------
; cga_testVideoMem: testa as 40x25
cga_testVideoByteMem:
	mov ax, 0B800h
	mov es, ax

	xor di, di
	mov cx, UNIT_16K
	mov di, UNIT_16K
	dec di
	mov bl, 05Ah
	test_videoWR:			; testar escrita
		
		mov si, 2
		test_videoWR_loop1:

			mov es:[di], bl
			xor al, al
			mov al, es:[di]
				
			mov dx, DISP_REG_0	; valor lido (parte alta)
			out dx, al
			mov dx, DISP_REG_1
			shr al, 4
			out dx, al
			;------------------
			mov al, bl			; valor escrito (parte alta)
			mov dx, DISP_REG_2
			out dx, al
			mov dx, DISP_REG_3
			shr al, 4
			out dx, al
			;------------------
			wait
			M_Countdown ax, 0FFFh
			M_beep

			not bl
			dec si
			jnz test_videoWR_loop1

		dec di
		loop test_videoWR

ret
;------------------------------------------------------------------------------
; cga_setVideo: muda o modo de video
cga_setVideo:
	push ax
	push bx
	push cx
	push dx
	push si
	xor ah, ah
	mov bx, ax
		
	;---------------------------------------------
	; definir modo MCRT
	mov si, offset CGA_VIDEO_PARAMS
	mov cl, 16
	mul cl
	add si, ax
	xor ah, ah ; indice MCRT = 0
	cga_setvideo_loop:
		mov dx, MCRT_INDEXREG	; meter indice (ah)	
		mov al, ah
		out dx, al
		;---------------------------------------------
		mov dx, MCRT_DATAREG
		mov al, cs:[si]
		out dx, al				; meter valor (al)
		inc ah					; proximo indice
		inc si
		cmp ah, 16
		jne cga_setvideo_loop

	;---------------------------------------------
	; definir modo cga
	mov al, cs:[offset CGA_VIDEO_MODES+bx]
	mov dx, CGA_MODE
	out dx, al

	pop si
	pop dx
	pop cx
	pop bx
	pop ax
ret
	 

;------------------------------------------------------------------------------
; cga_clearZero: zera a imagem
cga_clearZero:
	push ax
	push cx
	push di
	push es
	push dx
	cmp al, 1
	je cga_clear_vsync2
		M_CGA_clear 0, 0, 0
		M_CGA_clear 1, 0, 0
	jmp cga_clear_done2
	cga_clear_vsync2:
		M_CGA_clear 0, 0, 1
		M_CGA_clear 1, 0, 1
	cga_clear_done2:
	pop dx	
	pop es
	pop di
	pop cx
	pop ax
	ret
	
;------------------------------------------------------------------------------
; cga_clear: apagar com espaços e atributo fundo:0 frente:7
cga_clear:
	push ax
	push cx
	push di
	push es
	push dx
	cmp al, 1
	je cga_clear_vsync
		M_CGA_clear 0, 0720h, 0
		M_CGA_clear 1, 0720h, 0
	jmp cga_clear_done
	cga_clear_vsync:
		M_CGA_clear 0, 0720h, 1
		M_CGA_clear 1, 0720h, 1
	cga_clear_done:
	pop dx	
	pop es
	pop di
	pop cx
	pop ax
ret

;-----------------------------------------------------------------
cvtHex4:					; HEX8
	cmp al, 10				; al --> al 	
	jl cvtHex_		
		add al, -10+'A'-'0'
	cvtHex_:
		add al, '0'
ret
;-----------------------------------------------------------------
cvtHex8_:					; HEX8
	push ax					; al --> es:[di]
		and al, 0Fh
		call cvtHex4
		mov es:[di], al
		dec di
	pop ax
	push ax
		shr al, 4
		call cvtHex4
		mov es:[di], al
		dec di
	pop ax
ret 
;-----------------------------------------------------------------
cvtU16StrBuffer:		
push ax					; U16
push bx					; ax --> DATA:STRBUFFER
push dx					; si = inicio do buffer
push di
push es
	mov bx, DATA
	mov es, bx
	mov di, offset DATA:tty_strbuffEnd-1
	mov byte ptr es:[di], 0
	mov bx, 10
	cvtU16Str_loop:
		dec di			  ; avança para esq. string
		xor dx, dx		  ; DIVISÃO (LONGA):
		div bx            ;    dx:ax  | bx      |   ( num / 10 )
		                  ;           -------   |   resto  q
		                  ;    r:dx    q:ax     |   resto  q
						  ; 
		add dl, 48        ; imprime o resto na string
		mov es:[di], dl 
		or ax, ax
		jnz cvtU16Str_loop

	mov si, di
pop es
pop di
pop dx
pop bx
pop ax
ret						
;-----------------------------------------------------------------
cvtU8StrBuffer:			
push ax						; U8
	xor ah, ah				; ax --> DATA:STRBUFFER
	call cvtU16StrBuffer	; si = inicio do buffer
pop ax
ret						
;-----------------------------------------------------------------
cvtHex16StrBuffer:			; HEX16
	push ax					; ax --> DATA:STRBUFFER
	push bx
	push es
	push di
		mov bx, DATA
		mov es, bx
		mov byte ptr es:[offset DATA:tty_strbuff+4], 0
		mov di, offset DATA:tty_strbuff+3
		call cvtHex8_
		xchg al, ah
		call cvtHex8_
	pop di
	pop es
	pop bx
	pop ax
ret						

tty_hex16arr:
push cx
push ax
push si
	tty_hex16arr_loop:
		lodsw
		call tty_hex16
		call tty_sp
	loop tty_hex16arr_loop
pop si
pop ax
pop cx
ret



;-----------------------------------------------------------------
tty_hex8:				; al --> print Hex8(al)	
push ds
push ax
push bx
push es
push si
push di
	mov bx, DATA
	mov es, bx
	mov byte ptr es:[offset DATA:tty_strbuff+2], 0
	mov di, offset DATA:tty_strbuff+1
	call cvtHex8_
	mov ds, bx
	mov si, offset DATA:tty_strbuff
	call tty_str
pop di
pop si
pop es
pop bx
pop ax
pop ds
ret
;-----------------------------------------------------------------
tty_hex16:				; ax --> print Hex16(ax)
push ax
push ds
push si
	call cvtHex16StrBuffer
	mov ax, DATA
	mov ds, ax
	mov si, offset tty_strbuff
	call tty_str
pop si
pop ds
pop ax
ret 
;-----------------------------------------------------------------
tty_U8:					; al --> print U8(al)
push ax
push ds
push si
	call cvtU8StrBuffer
	mov ax, DATA
	mov ds, ax
	call tty_str
pop si
pop ds
pop ax
ret 
tty_U8time:
pushf
push ax
	cmp al, 10
	jae tty_U8time_
		mov al, '0'
		call tty_char
		pop ax
		push ax
	tty_U8time_:
	call tty_U8
pop ax
popf
ret

;-----------------------------------------------------------------
tty_U16:				; ax --> print U16(ax)
push ax
push ds
push si
	call cvtU16StrBuffer
	mov ax, DATA
	mov ds, ax
	call tty_str
pop si
pop ds
pop ax
ret

;-----------------------------------------------------------------
tty_S16:				; ax --> print S16(ax)
pushf
	test ax, 8000h
	jz tty_U16z
		push ax
			mov al, '-'
			call tty_char
		pop ax
		push ax
			dec ax
			not ax
			call tty_U16
		pop ax
		jmp tty_U16end
	tty_U16z:
	call tty_U16
	tty_U16end:
popf
ret
;-----------------------------------------------------------------
tty_S8:				; ax --> print S16(ax)
pushf
	test al, 80h
	jz tty_U8z
		push ax
			mov al, '-'
			call tty_char
		pop ax
		push ax
			dec al
			not al
			call tty_U8
		pop ax
		jmp tty_U8end
	tty_U8z:
	call tty_U8
	tty_U8end:
popf
ret
;-----------------------------------------------------------------
tty_bit:				; al --> print LSB(al)
	push ax
	and al, 1
	add al, '0'
	call tty_char
	pop ax
ret
;-----------------------------------------------------------------
tty_bytesFormat:		; ax setores --> print KB
pushf					; 1 setor = 512B
push ax
	call tty_U16
	call tty_Bytes
	test ax, 200h
	pushf
	push ax
		mov al, '('
		call tty_char
	pop ax
	shr ax, 10
	call tty_U16
	popf
	jz tty_bytesFormat1
		call tty_dot
		mov al, '5'
		call tty_char
	tty_bytesFormat1:
		call tty_KB
		
	push ax
		mov al, ')'
		call tty_char
	pop ax

pop ax
popf
ret
;-----------------------------------------------------------------
tty_sectsToKB:			; ax setores --> print KB
pushf					; 1 setor = 512B
push ax
	shr ax, 1
	pushf
	call tty_U16
	popf
	jnc tty_STKB_isNotHalf
		call tty_dot
		mov al, '5'
		call tty_char
	tty_STKB_isNotHalf:
		call tty_KB
pop ax
popf
ret
;-----------------------------------------------------------------
tty_bin8:				; al ---> print binário
push cx
push ax
	mov cx, 8
	tty_bin8_loop:
		rol	al, 1
		call tty_bit
	loop tty_bin8_loop
pop ax
pop cx
ret

;-----------------------------------------------------------------
tty_bin16:				; al ---> print binário
push ax
	xchg al, ah
	call tty_bin8
	xchg al, ah
	call tty_bin8
pop ax
ret
;-------------------------------------------------------------
; puxa as linhas para cima
tty_rollLine:
	push es
	push ds
	push si
	push di
	push ax
	push cx
	push dx
	pushf
		cld
	
		IF	TTY_MAKE_ROLLSOUND EQ 1
			call makeShortBeep
		ENDIF

		mov ax, 0B800h			; es:[di] = B800h:linha cima
		mov ds, ax				; ds:[si] = B800h:linha debaixo
		mov es, ax
		mov di, 0				; di=1ª linha
		mov si, TTY_COLS*2		; si=2ª linha
		mov cx, TTY_ROWS-1		; linhas-1
		
		IF BUILD_BIOS_USEVSYNC EQ 1
		IF (TTY_MODE+TTY_ISCGA) EQ 2
			mov dx, CGA_STATUS
		ENDIF
		ENDIF

		tty_rollLine_loop:
			push cx
				mov cx, TTY_COLS	; copiar a próxima linha es:[di]
				; esperar pelo espaço vertical se modo 80 colunas
				IF BUILD_BIOS_USEVSYNC EQ 1
				IF (TTY_MODE+TTY_ISCGA) EQ 2
					M_CGA_WaitVSync
				ENDIF
				ENDIF

			rep movsw			; para a linha de cima ds:[si]
			pop cx
			loop tty_rollLine_loop

		; apagar ultima linha
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov cx, TTY_COLS			; es:[di] está na ultima linha
		IF BUILD_BIOS_USEVSYNC EQ 1
		IF (TTY_MODE+TTY_ISCGA) EQ 2
			M_CGA_WaitVSync
		ENDIF
		ENDIF
		mov al, ' '
		mov ah, tty_att
		stosw
		assume DS:CODE
	popf
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop ds
	pop es
ret

;-------------------------------------------------------------
; compara se a linha chegou à última; se sim rola o ecrã
; e atualiza o valor tty_row
; entra:
;    ds: DATA
tty_checkRow_:
	push bx
		mov bl, ds:[tty_row]
		cmp bl, (TTY_ROWS-1)
		jne tty_checkRowSkip_
			call tty_rollLine
			dec byte ptr ds:[tty_row]
	tty_checkRowSkip_:
	pop bx
ret

;-------------------------------------------------------------
; nova linha
tty_crlf:
	push ds
	push di
	push bx
		assume DS:DATA
		mov bx, DATA						; ds=DATA
		mov ds, bx
		xor bl, bl				
		mov tty_col, bl		; X=0									
		inc tty_row			; inc Y
		call tty_checkRow_					; verificar se bateu fundo
		assume DS:CODE
	pop bx
	pop di
	pop ds
ret
;-------------------------------------------------------------
; imprime espaço
tty_Sp:
	push ax
		mov al, ' '
		call tty_char
	pop ax
ret
tty_Err:
	push ax
		mov al, 'E'
		call tty_char
		mov al, 'R'
		call tty_char
		mov al, 'R'
		call tty_char
	pop ax
ret
tty_Ok:
	push ax
		mov al, 'O'
		call tty_char
		mov al, 'k'
		call tty_char
	pop ax
ret
tty_Bytes:
	push ax
		mov al, 'b'
		call tty_char
		mov al, 'y'
		call tty_char
		mov al, 't'
		call tty_char
		mov al, 'e'
		call tty_char
		mov al, 's'
		call tty_char
	pop ax
ret
tty_KB:
	push ax
		mov al, 'K'
		call tty_char
		mov al, 'B'
		call tty_char
	pop ax
ret
tty_dot:
	push ax
		mov al, '.'
		call tty_char
	pop ax
ret
tty_ddot:
	push ax
		mov al, ':'
		call tty_char 
	pop ax
ret
;-------------------------------------------------------------
; imprime caracter
;    al: o caractere a imprimir
tty_chars:
	push cx
	tty_chars_:
	 call tty_char
	loop tty_chars_
	pop cx
	ret 
;-------------------------------------------------------------
; imprime caracter
;    al: o caractere a imprimir
tty_char:
	pushf
		cmp al, 10						; é LF/CR?
		je tty_charExit					; ...sim ir para outra linha
		cmp al, 13						; é LF/CR?
		jne tty_charSt					; ...sim ir para outra linha
			call tty_crlf
		tty_charExit:
	popf
	ret 

	; sai em AX a posição do cursor
tty_getCursorLineAddr:
push bx
push ds
	ASSUME DS:DATA
	call ds2data
	xor bh, bh
	mov bl, TTY_COLS*2				; bl = total colunas * 2
	xor ah, ah
	mov al, tty_row					; al = total linhas
	mul bl							; ax = total linhas*(nºcols*2)	
	ASSUME DS:CODE	
pop ds
pop bx
ret

	; sai em AX a posição do cursor
tty_getCursorAddr:
push bx
push ds
	ASSUME DS:DATA
	call ds2data
	xor bh, bh
	mov bl, TTY_COLS*2				; bl = total colunas * 2
	xor ah, ah
	mov al, tty_row					; al = total linhas
	mul bl							; ax = total linhas*(nºcols*2)	
	mov bl, tty_col					; bl = cols	
	xor bh, bh
	shl bx, 1						; bx = cols*2
	add ax, bx						; ax = cols*2+total linhas*(nºcols*2)
	ASSUME DS:CODE	
pop ds
pop bx
ret


;-------------------------------------------------------------
tty_cleanLineFromCur:
pushf
push ax
push di
	call tty_getCursorAddr
	mov di, ax
push cx
push dx
push es
push ds
	ASSUME DS:DATA
	cld
	mov ax, 0B800h
	mov es, ax
	mov cl, TTY_COLS
	mov ch, tty_col
	sub cl, ch				; 80-coluna currente
	xor ch, ch

	IF BUILD_BIOS_USEVSYNC EQ 1 
	IF (TTY_MODE+TTY_ISCGA) EQ 2
		mov dx, CGA_STATUS
		M_CGA_WaitVSync
	ENDIF
	ENDIF

	call ds2data
	mov ah, tty_att
	mov al, 20h
	rep stosw
	ASSUME DS:CODE
pop ds
pop es
pop dx
pop cx
pop di
pop ax
popf
ret

tty_saveCurToBDA:
push ax
push ds
	ASSUME DS:DATA
	call ds2data
	mov al, tty_col
	mov ah, tty_row
	mov cursor_posn, ax
	ASSUME DS:CODE
pop ds
pop ax
ret

tty_charSt:
push ds
push di
push es
push dx
push bx
push ax
		assume DS:DATA
		mov bx, DATA							; ds=DATA
		mov ds, bx
											; é caracter imprimivel
											; escrever caractere
		mov bx, 0B800h						; calcular a posição e 
		mov es, bx							; escrever caractere
		
		call tty_getCursorAddr
		mov di, ax
		; esperar pelo espaço vertical se modo 80 colunas
		
		IF BUILD_BIOS_USEVSYNC EQ 1 
		IF (TTY_MODE+TTY_ISCGA) EQ 2
			mov dx, CGA_STATUS
			M_CGA_WaitVSync
		ENDIF
		ENDIF
		pop ax		; restaurar o caractere
		push ax								
		
		mov ah, tty_att						; es:[di]	
		stosw
											; avançar posição
		inc tty_col							; inc X
		mov bl, tty_col
		cmp bl, (TTY_COLS)				; bateu na direita?
		jne tty_charEnd

		tty_charNewLine:
			call tty_crlf					; ...sim ir para outra linha

assume DS:CODE
tty_charEnd:
pop ax
pop bx
pop dx
pop es
pop di
pop ds
popf
ret
;-------------------------------------------------------------
; CRLF + imprime string em DS:SI 
tty_line:
	call tty_crlf
	call tty_str
ret
;-------------------------------------------------------------
; imprime string em DS:SI
;    ds:si a string a imprimir
tty_str:
push ax
push si
	tty_strloop:	
		mov al, ds:[si]		; terminou string?
		or al, al
		jz tty_strExit
		call tty_char
		inc si
	jmp tty_strloop
tty_strExit:
pop si
pop ax
ret

tty_IP:
push si
	mov al, ds:[si]
	call tty_U8
	call tty_dot
	mov al, ds:[si+1]
	call tty_U8
	call tty_dot
	mov al, ds:[si+2]
	call tty_U8
	call tty_dot
	mov al, ds:[si+3]
	call tty_U8
	call tty_dot
pop si
ret


tty_strn:
push cx
push si
	tty_strloop2:	
		mov al, ds:[si]		
		call tty_char
		inc si
	loop tty_strloop2
pop si
pop cx
ret 


tty_atts:
push ax
push bx
push cx
push si
	call tty_getCursorAddr
	mov si, ax
	mov ax, 0B800h
	mov ds, ax
	ASSUME DS:DATA
		call ds2data
		mov bh, tty_col
	ASSUME DS:CODE
	tty_strloop1:	
		mov ds:[si+1], bh
	loop tty_strloop1
pop si
pop cx
pop bx
pop ax
ret
;-------------------------------------------------------------
; imprime string em DS:SI
;    ds:si a string a imprimir
tty_centeredStr:
push cx
push dx
	call strlen
	call tty_getCur
	mov dl, 80/(2-TTY_MODE)
	sub dl, cl
	shr dl, 1
	call tty_setCur
	call tty_str
pop dx
pop cx
ret

;-------------------------------------------------------------
; al: tamanho, dx: bytes total, cx: bytes curr
tty_progbarInit:
M_push6 ax, ds, es, di, dx, cx 
	xor ch, ch
	mov cl, al					;  cx=tamanho
	mov ax, 0B800h
	mov es, ax
	call tty_getCursorAddr
	mov di, ax
	assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov ah, ds:[tty_att]
	assume DS:CODE
	mov al, 176
	rep stosw
M_pop6 ax, ds, es, di, dx, cx
ret
;-------------------------------------------------------------
; al: tamanho, dx: bytes total, cx: bytes curr
tty_progBar:
M_push7 di, bx, ax, ds, es, dx, cx 
	 
	xor ah, ah
	mov bx, dx						; BX=DX tem o total
	push ax
		
		xor dx, dx					; [AL=colunas] * [curr=CX] 
		mul cx						;	    [total=BX]
		div bx		
		mov cx, ax					; [CX=Nº de colunas a pintar]
	
	mov ax, 0B800h					; apanhar o local do cursor
	mov es, ax						; para es:di 
	call tty_getCursorAddr
	mov di, ax
	
	assume DS:DATA					; apanhar o atributo
		mov ax, DATA				; currente
		mov ds, ax
		mov ah, ds:[tty_att]		; ah=atributo
	assume DS:CODE
	
	push di							; pintar CX blocos 
		mov al, 219					; es:[di++]=ax
		rep	stosw					
	pop di
	
	pop dx							; DX = total colunas 
	push dx
		shl dx, 1
		add di, dx					; pintar branco a seguir
		mov cx, 20					 
		mov al, 20h
		rep stosw
	pop ax						; restaurar o nº de colunas
			
			
	assume DS:DATA				; guardar em di, e logo
		mov bx, DATA			; mudar posição do cursor
		mov ds, bx
		mov di, word ptr ds:[tty_col]
		add ds:[tty_col], al 
	assume DS:CODE

	pop cx
	pop dx
	push dx
	push cx

		mov bx, dx					; BX=DX tem o total
		mov ax, 100	
		xor dx, dx					; [100] * [curr=CX] 
		mul cx						;	     [total=BX]
		div bx		
		mov cx, ax					; [AX=percent]

		call tty_sp
		call tty_U16
		mov al, '%'
		call tty_char
	
	assume DS:DATA					; restaurar cursor
		mov ax, di
		mov ds:[tty_col], al
	assume DS:CODE

M_pop7 di, bx, ax, ds, es, dx, cx
ret
	
	; bl = atributo
tty_coloredLine:
push ax
push cx
push es
push di
	mov ax, 0B800h 
	mov es, ax
	call tty_getCursorLineAddr
	mov cx, TTY_COLS
	mov di, ax
	cl_loop:
		mov es:[di+1], bl
		add di, 2
		loop cl_loop
pop di
pop es
pop cx
pop ax
ret


;-------------------------------------------------------------
; apaga AX linhas apartir do cursor
tty_clearLines:				; AX=nº de linhas a apagar
	push ds
	push ax
	push bx
	push cx
	push dx
	push es
	push di
	pushf
		cld
		IF BUILD_BIOS_USEVSYNC EQ 1 
		IF (TTY_MODE+TTY_ISCGA) EQ 2
			mov dx, CGA_STATUS
		ENDIF
		ENDIF
		assume DS:DATA
		mov bx, DATA
		mov ds, bx				; DS=DATA
		mov bx, 0B800h
		mov es, bx				; ES=B800h
		mov bl, TTY_COLS*2		; BL=total colunas*2
		push ax
			mov al, tty_row	; AL=DS:[tty_row]
			xor ah, ah			;      ----AL----   -----BL---------
			mul bl				; AX= linha atual * (total colunas*2)
			mov di, ax			; ES:DI = cursor na linha atual
		pop ax
			mov bl, TTY_COLS	; BL=total colunas
			xor ah, ah			;      ----AL----------   -----BL-------
			mul bl				; CX = nº linhas apagar * (total colunas)
			mov cx, ax			; CX = nº words a apagar
				
		
		IF BUILD_BIOS_USEVSYNC EQ 1 
		IF (TTY_MODE+TTY_ISCGA) EQ 2
			M_CGA_WaitVSync
		ENDIF
		ENDIF
		mov ah, tty_att
		mov al, 20h
		rep stosw

		assume DS:CODE
	popf
	pop di
	pop es
	pop dx
	pop cx
	pop bx
	pop ax
	pop ds
ret

;-------------------------------------------------------------
; cursor : guardar/restaurar/repor a zero
tty_restoreCur:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov al, tty_lastCol
		mov tty_col, al
		mov al, tty_lastRow
		mov tty_row, al
		mov al, tty_lastAtt
		mov tty_att, al
		assume DS:CODE
	pop ax
	pop ds
	ret
tty_saveCur:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov al, tty_col
		mov tty_lastCol, al
		mov al, tty_row
		mov tty_lastRow, al
		mov al, tty_att
		mov tty_lastAtt, al
		assume DS:CODE
	pop ax
	pop ds
	ret
tty_resetCur:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov tty_lastcol, 0
		mov tty_lastrow, 0
		mov tty_col, 0
		mov tty_row, 0
		mov tty_att, 07h
		assume DS:CODE
	pop ax
	pop ds
	ret

; entra dx linha/coluna
tty_setCur:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov tty_col, dl
		mov tty_row, dh
		assume DS:CODE
	pop ax
	pop ds
	ret
	
; entra dx linha/coluna
tty_getCur:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov dl, tty_col
		mov dh, tty_row
		assume DS:CODE
	pop ax
	pop ds
	ret
;-------------------------------------------------------------
; atributo: repor ao normal/colocar a BL
tty_resetAtt:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov al, 07h
		mov tty_lastatt, al
		mov tty_att, al
		assume DS:CODE
	pop ax
	pop ds
	ret
tty_setAtt:
	push ds
	push ax
		assume DS:DATA
		mov ax, DATA
		mov ds, ax
		mov tty_att, bl
		assume DS:CODE
	pop ax
	pop ds
	ret
	
;-------------------------------------------------------------
; inicializar
tty_init: 
	push ax
	IF BUILD_BIOS_VIDEO EQ 0
		mov al, TTY_MODE
		call cga_setVideo
	ENDIF 
	pop ax
;-------------------------------------------------------------
; apagar as definições
tty_clear:
	push ax
		IF BUILD_BIOS_VIDEO EQ 1
			mov ah, 00  
			IF TTY_MODE EQ 0
				mov al, 01
			ELSE
				mov al, 03
			ENDIF
			INT 10h 
		ELSE
			call cga_clearZero
		ENDIF
		call tty_resetAtt
		call tty_resetCur
	pop ax
	ret
	
;-------------------------------------------------------------
tty_buffer:
	push ax
	push bx
	push cx
	push si
	push di
	call tty_crlf		; imprimir ds:si
		mov ax, ds
		call tty_Hex16
		mov al, ':'
		call tty_char
		mov ax, si
		call tty_Hex16

	call tty_crlf			; imprimir matriz 16x16
	mov cx, 16
	loop_rows:
		push cx
		
		mov ax, si
		call tty_Hex16
		mov al, ':'
		call tty_char
		
		mov cx, 16
		mov ax, cx 
		loop_cols:
			lodsb
			mov dl, es:[di]
			cmp al, dl
			je loop_colsEqu
				mov bl, 4Fh
				call tty_setAtt
			loop_colsEqu:
				call tty_Hex8
				call tty_resetAtt
			inc di
			loop loop_cols

		IF TTY_MODE EQ 1
			mov al, ' '
			call tty_char
			mov cx, 16
			sub di, 16
			sub si, 16
			loop_cols2:
				lodsb
				mov dl, es:[di]
				cmp al, dl
				je loop_colsEqu2
					mov bl, 4Fh
					call tty_setAtt
				loop_colsEqu2:
				cmp al, 13
				jne loop_cols2_skip
					mov al, ' '
				loop_cols2_skip:
				call tty_char
				call tty_resetAtt
			inc di
			loop loop_cols2
		ENDIF
		call tty_crlf
		pop cx
		loop loop_rows
	pop di
	pop si
	pop cx
	pop bx
	pop ax
ret
;-------------------------------------------------------------
; imprimir os registos
tty_regs1:
	pushf
	call tty_crlf
	M_tty_wrFieldReg 'A', 'X', AX
	M_tty_wrFieldReg 'B', 'X', BX
	M_tty_wrFieldReg 'C', 'X', CX
	M_tty_wrFieldReg 'D', 'X', DX
	call tty_crlf
	M_tty_wrFieldReg 'D', 'S', DS
	M_tty_wrFieldReg 'S', 'I', SI
	M_tty_wrFieldReg 'E', 'S', ES
	M_tty_wrFieldReg 'D', 'I', DI
	call tty_crlf
	pop ax
	M_tty_wrFieldReg 'F', 'L', AX
ret

tty_regs:
push ax
	call tty_regs1
	M_tty_wrFieldReg 'S', 'S', SS
	mov ax, sp
	add ax, 2
	M_tty_wrFieldReg 'S', 'P', AX
pop ax
ret

tty_prnTime:
	mov al, byte ptr es:[di+0]
	call tty_U8time
	mov al, ':'
	call tty_char
	mov al, byte ptr es:[di+1]
	call tty_U8time
	mov al, ':'
	call tty_char
	mov al, byte ptr es:[di+2]
	call tty_U8time
ret

tty_prnDate:
	mov al, byte ptr es:[di+0]
	call tty_U8time
	mov al, '/'
	call tty_char
	mov al, byte ptr es:[di+1]
	call tty_U8time
	mov al, '/'
	call tty_char
	mov al, byte ptr es:[di+2]
	call tty_U8time
ret

		
		; tamanho em blocos de 512B 
		; multiplicar tudo
;mul dl
;mul cx		 
;shr ax, 1	 ; ax>>1
;mov cx, dx		
;ror cx, 1	; [LSB ..]
;mov dx, cx
;and dx, 07FFFh 
;and cx, 08000h
;or ax, cx
pop ax
ret
 
;tty_U32:
;push ax
;push cx
;push dx
;push si
;call ds2data
;mov si, offset tty_strbuffEnd
;mov ds:[si], 0
;dec si
;
;	mov cx, 0
;	tty_U32_loop:
;		push dx
;		push ax
;			div 10
;			or ax, ax
;			jz tty_U32_loopEnd
;			add dl, '0'
;			mov ds:[si], dl
;			dec si
;		pop dx
;	jnz tty_U32_loop
;
;	tty_U32_loopEnd:
;	inc si
;	call tty_str
;
;pop si
;pop dx
;pop cx
;pop ax

;						scancode 0..9 --> al=0..9
keyb2digit:
	cmp al, KEY_SCANCODE_ZERO
	je keyb2digit0
	cmp al, KEY_SCANCODE_DIGIT
	jb keyb2digitNo
	cmp al, KEY_SCANCODE_DIGIT+9
	ja keyb2digitNo
	jmp keyb2digitOther
keyb2digit0:
	mov al, 0
	clc
	ret
keyb2digitOther:
	sub al, KEY_SCANCODE_DIGIT-1
	clc
	ret
keyb2digitNo:
	stc
	ret


;------------------------------------------------------------------------------
; substitui a INT10 só nas funções de escrita de 1 char ou strings 

IF BUILD_BIOS_VIDEO EQ 0 
	tty_int10h:
		cmp ah, 0Eh
		jne p_int10_next0
			call tty_char 
			iret
			
		p_int10_next0:

		cmp ah, 13h
		jne p_int10_next1
			push ax
			push cx
			push dx 
			push si
				call tty_crlf
				cmp al, 1
				jg p_int10_att
					mov dx, 1
					jmp p_int10_att_strloop
				p_int10_att:
					mov dx, 2
				p_int10_att_strloop:
					mov al, es:[si] 
					call tty_char
					add si, dx
					loop p_int10_att_strloop
			pop si 
			pop dx
			pop cx
			pop ax
		iret
		
		p_int10_next1:
		iret

ENDIF


			; entra CHS (no formato da BIOS)
tty_CHS:
M_push4 ds, ax, cx, dx
	call ds2cs
	M_tty_wrFieldChar 'C'
	mov ax, cx
	xchg al, ah
	shr ah, 6
	call tty_hex16
	mov al, dh
	and al, 15
	M_tty_wrFieldChar 'H'
	call tty_hex8
pop dx
pop cx
	mov al, cl
	and al, 3Fh
	M_tty_wrFieldChar 'S'
	call tty_hex8
	call tty_sp
pop ax
pop ds
ret

		; entra CHS (no formato da BIOS)
		; cx: cilindros
		; dx: cabeças
		; ax: setores
tty_CHS_normal:
push ds
push ax
	call ds2cs
	M_tty_wrFieldChar 'C'
	mov ax, cx
	call tty_hex16
	M_tty_wrFieldChar 'H'
	mov ax, dx
	call tty_hex8
	M_tty_wrFieldChar 'S'
pop ax
	call tty_hex8
pop ds
ret


tty_prnMemDisp:
	; imprimir
	push ax
	push cx
		call tty_restoreCur
		shr cx, 6
		mov ax, cx
		call tty_U16
		call tty_KB
	pop cx
	pop ax
ret

tty_prnMem:
	call tty_saveCur
	assume ds:CODE
	mov cx, 1000h; começar aos 64KB 
	mov ax, 0AA55h
	mem_loop_test1:
		call tty_prnMemDisp
		mov es, cx			

		mov dx, es:[0]		; ler
		mov es:[0], ax		; escrever 
		mov bx, es:[0]		; ler o q escreveu	
		mov es:[0], dx		; deixar igual ao que estava
		cmp ax, bx			; igual resultado?
		jne mem_loop_exit

		not ax
		cmp ax, 0AA55h
		jne mem_loop_test1	; terminou?

		add cx, 400h			; + 16KB
		cmp cx, 0A000h
		jne mem_loop_test1
								; 640KB completos
	mem_loop_exit:
	
		call tty_prnMemDisp


ret

	; entra es:di localização de memoria
	; sai   cx nº de caracteres
tty_inputStr:
push di
	push ax
	mov cx, 0
	tty_inputStrLoop:
		xor ax, ax
		INT 16h
		cmp ah, KEY_SCANCODE_ESC
		je tty_inputStr_cancel
		cmp ah, KEY_SCANCODE_ENTER
		je tty_inputStr_accept
		stosb 
		call tty_char
		inc cx
		jmp tty_inputStrLoop
	tty_inputStr_accept:
		xor al, al
		mov es:[di], al			; terminar com zero
		clc
		jmp tty_inputStr_exit
	tty_inputStr_cancel:
		stc
	tty_inputStr_exit:
		pop ax
		pop di
		ret

	; entra es:di a localização de mem a usar
	; sai em AX o valor
tty_inputHex:
	M_push4 bx, cx, dx, di
		call tty_inputStr
		jnc tty_inputHexCont

		tty_inputHexErr1:
			M_pop4 bx, cx, dx, di
			stc
			ret
		tty_inputHexCont:
		xor bx, bx
		xor dx, dx
		dec cx
		add di, cx
		inc cx
		jz tty_inputHexErr1
		tty_inputHex8_loop:
			mov al, es:[di]
			call toUpperChar
			;;call tty_crlf
			;;call tty_char
			;;call tty_dot

			sub al, '0'
			cmp al, 9
			jle tty_iH8_skip

			cmp al, ('A'-'0')	; onde começa A
			jl tty_inputHexErr1
			cmp al, ('F'-'0') ; onde começa F
			ja tty_inputHexErr1

			add al, 10-('A'-'0')
			tty_iH8_skip:
			
				xor ah, ah			; AH=0
				;;call tty_Hex8
				;;call tty_dot

				cmp dl, 0
				je tty_iH8_cont
				cmp dl, 1
				je tty_iH8_skip1
				cmp dl, 2
				je tty_iH8_skip2
				cmp dl, 3
				je tty_iH8_skip3
				
				tty_iH8_skip1:
				shl al, 4
				jmp tty_iH8_cont
				tty_iH8_skip2:
				xchg al, ah
				jmp tty_iH8_cont
				tty_iH8_skip3:
				xchg al, ah
				shl ah, 4

				tty_iH8_cont:
				add bx, ax
			inc dl
			dec di
		loop tty_inputHex8_loop
	mov ax, bx
	clc
	jmp tty_inputHexOK
	tty_inputHexErr:
	stc
	tty_inputHexOK:
	M_pop4 bx, cx, dx, di
ret

