; Algumas routinas para desenho em CGA
;
;  1.         cga_pixel: pinta um pixel CGA em X0,Y0 com a cor DL
;  2.          cga_rect: desenha retangulo [X0,X0+DX[,[Y0,Y0+DY[ (não usa STACK)
;  3.      cga_fillRect: desenha retangulo [X0,X0+DX[,[Y0,Y0+DY[ (não usa STACK)
;  4.       cga_invRect: inverte o retangulo [X0,X0+DX[,[Y0,Y0+DY[ (não usa STACK)
;  5.          cga_line: desenha um segmento de reta CGA sem thickness
;  6.     cga_lineThick: desenha um segmento de reta CGA com thickness (chama a várias cga_line)
;  7.       cga_fillScr: enche o ecrã com byte AL
;  8.    cga_copyToVRAM: copia de DS:BX para B8000h, e de DS:BX+8KB para BA000h
;  9.         cga_clock: desenha clock em BOOTSCR_CLOCK_X/Y
; 10.         getVector: a componente dum "vetor tabelado"/BL na tabela SI
; 11.      getSinVector: a componente seno dum "vetor tabelado"/BL
; 12.      getCosVector: a componente coseno dum "vetor tabelado"/BL

DEBUG_BRESENHAM EQU 0
DEBUG_CLOCK		EQU 0

;------------------------------------------------------------------------------
; 1.A. M_cga_pixelloc: sai a localização dum pixel X0, Y0 em ES:DI + CL
; entra: AX, BX: X0, Y0
; sai: ES:DI localização do BYTE, CL posição dos 2bits
M_cga_pixelloc MACRO
	push bx				
	push ax				
		mov ax, bx
		and ax, 1		; MASCARAR PARIDADE Y0
		shl ax, 9		; MOVER PARA O BIT 9
		mov cx, 0B800h	; PARA COM OR,
		or  ch, ah		; DEFINIR O SEG. DO PLANO
		mov es, cx		; EM ES

		mov ax, bx		
		shr ax, 1		; AX=Y0/2
		mov cl, 320/4	; AX=(Y0/2)*COLSBYTES
		mul cl
		mov di, ax		; ES:DI localização na linha
	pop ax				 ;AX=X0
	push ax
		mov ch, al		 ;CH=X0
		and ch, 3		 ;CH=BITS RESTO DA DIVISAO POR 4
		shl ch, 1		 ;   * 2 PORQUE TEM DE DESLOCAR	
		mov cl, 6
		sub cl, ch
		shr ax, 2		 ;AX=X0/4
		add di, ax		 ;ES:DI localização no byte
	pop ax
	pop bx
ENDM

;------------------------------------------------------------------------------
; 1. cga_pixel: pinta um pixel CGA em X0,Y0 com a cor DL
; AX, BX: X0, Y0
; DL: COR
cga_pixel:
push dx
push cx
push di
push es
	M_cga_pixelloc
	push ax
	push bx
		mov al, byte ptr es:[di]	; LER BYTE
		mov bl, 11b
		and dl, bl					; MASCARA DE COR					
		shl bl, cl					; 00110000				
		not bl						; MASCARA DE BITS
		and al, bl					; APAGAR BITS
		shl dl, cl					; SHIFTAR COR E
		or  al, dl					; ADICIONAR COR
		mov byte ptr es:[di], al    ; GUARDAR BYTE
	pop bx
	pop ax
pop es
pop di
pop cx
pop dx
ret


cgaRect_maskL db    0, 03Fh, 0Fh,  03h
cgaRect_maskR db    0C0h, 0F0h, 0FCh , 0FFh

;------------------------------------------------------------------------------
; 3. cga_invRect: inverte o retangulo [X0,X0+DX[,[Y0,Y0+DY[
; nao usa STACK, inverte rectangulo
; AX, BX: X0, Y0
; CX, DX: DX, DY
cga_rect_end:
	pop dx
	pop si
	pop di
	pop es
	ret

cga_rect:
CGA_RECT_COLOR           EQU bp+20
CGA_RECT_RECTFUNCTION    EQU bp+18
;					RA	;bp+16
push es				 	;bp+14
push di					;bp+12
push si					;bp+10
push dx   				;bp+8
	or dx, dx
	jz cga_rect_end
push cx   				;bp+6
push bx   				;bp+4
push ax					;bp+2
push bp				    ;bp+0
mov bp, sp
push word ptr 0 
CGA_RECT_MASKFRONT		 EQU bp-2
push word ptr 0   
CGA_RECT_MASKEND		 EQU  bp-4
push cx 
CGA_RECT_BITS_DELTAX	 EQU bp-6

	jz cga_rect_end
		
	; fazer as mascaras para L e R
	push ax	
		and al, 11b
		xor ah, ah
		mov si, ax
		mov al, byte ptr cs:[offset cgaRect_maskL+si] 
		mov ah, al
		not ah
		mov ss:[CGA_RECT_MASKFRONT], ax
	pop ax
	push cx
		add cx, ax
		and cl, 11b
		xor ch, ch
		mov si, cx
		mov cl, byte ptr cs:[offset cgaRect_maskR+si] 
		mov ch, cl
		not ch
		mov ss:[CGA_RECT_MASKEND], cx
	pop cx

	push ax
	push cx
	M_cga_pixelloc	; PixelLoc devolve ES:DI
	pop cx			
	pop ax			
	shr cx, 2

	loop_fill:				    ; preencher em altura

			push cx		; mascarar primeiros 4 pixeis
				mov bx, ss:[CGA_RECT_MASKFRONT]
				mov al, es:[di]	; LER BYTE E EXEC FUNÇÃO
				call ss:[CGA_RECT_RECTFUNCTION]
				inc di
				dec cx
				or cx, cx
				jz loop_fill1

						; preencher outros pixeis
				mov bx, 000FFh
				loop_fill1:
					mov al, es:[di] ; LER BYTE E EXEC FUNÇÃO
					call ss:[CGA_RECT_RECTFUNCTION]
					inc di
				loop loop_fill1

				loop_nomore:
			pop cx
						; mascarar ultimos 4 pixeis
				mov bx, ss:[CGA_RECT_MASKEND]
				mov al, es:[di]		; LER BYTE E EXEC FUNÇÃO
				call ss:[CGA_RECT_RECTFUNCTION]

			loop_fill_noLast:
				sub di, cx
				mov ax, es	; mudar o segmento
				xor ah, 2
				mov es, ax
				test ah, 2			; se proximo for PAR
			jnz loop_fill_isEven	; adicionar linha
				add di, 320/4
			loop_fill_isEven: 

		dec dx
		jnz loop_fill


mov sp, bp
pop bp
pop ax
pop bx
pop cx
pop dx  
pop si
pop di
pop es
ret

;------------------------------------------------------------------------------
; funções de retangulo - AL=ES:[DI]
cga_invRect_:
	xor al, bl      ; A MASCARA EM BL INVERTE
	mov es:[di], al ; OS BITS EM AL
ret
cga_solidRect_:
	mov al, ss:[CGA_RECT_COLOR]
	cga_solidRect2_:
	and es:[di], bh
	and al, bl
	or  es:[di], al 
ret
cga_pattRect1_:
	mov ah, ss:[CGA_RECT_COLOR]
	and al, NOT(00110011b)
	test dx, 1
	jz cga_pattRect_even
		mov al, NOT(11001100b)
	cga_pattRect_even:
	and al, es:[di]
	jmp cga_solidRect2_

cga_pattRect2_:
	mov ah, ss:[CGA_RECT_COLOR]
	and al, NOT(00000011b)
	test dx, 1
	jz cga_pattRect_even2
		mov al, NOT(00110000b)
	cga_pattRect_even2:
	and al, es:[di]
	jmp cga_solidRect2_

cga_pattRect3_:
	mov ah, ss:[CGA_RECT_COLOR]
	and al, NOT(00000011b)
	test dx, 1
	jz cga_pattRect_even3
		ret
	test cx, 1
	jz cga_pattRect_even3
		mov ah, ss:[CGA_RECT_COLOR]
		and al, NOT(00110000b)
	cga_pattRect_even3:
	and al, es:[di]
	jmp cga_solidRect2_

;------------------------------------------------------------------------------
; 5. cga_line: desenha um segmento de reta CGA sem thickness
; AX, BX: X0, Y0
; CX, DX: X1, Y1
; STACK: [THICK|COR]
cga_line:
	CGA_BRES_OPTION EQU +6
	CGA_BRES_RA     EQU +4
	push ax		   ;EQU +2
	push bp		   ;EQU +0
	mov bp, sp
	push 0	
	CGA_BRES_AX   EQU -2
	push 0
	CGA_BRES_XERR EQU -4
	push 0
	CGA_BRES_YERR EQU -6
	push 0
	CGA_BRES_XINC EQU -8
	push 0
	CGA_BRES_YINC EQU -10
	push cx
	CGA_BRES_X    EQU -12
	push 0
	CGA_BRES_Y    EQU -14
	push 0

	push bx
	push cx
	push dx
	push si
	push di

		mov word ptr ss:[bp+CGA_BRES_X], ax
		mov word ptr ss:[bp+CGA_BRES_Y], bx 
		sub cx, ax	; CX=VX
		sub dx, bx  ; DX=VY
		M_abs16 cx, si
		M_abs16 dx, di 
		cmp si, di
			mov si, CGA_BRES_Y
			mov di, CGA_BRES_X
		jle	cga_line_noswap  
			xchg cx, dx
			xchg si, di
			IF DEBUG_BRESENHAM EQ 1
				call tty_crlf
				M_tty_wrStr "swap"
			ENDIF
		cga_line_noswap:
			M_abs16 cx, ax 
			mov word ptr ss:[bp+CGA_BRES_XERR], ax
			M_abs16 dx, ax
			mov word ptr ss:[bp+CGA_BRES_YERR], ax	
			M_sign16ToInc dx, ax
			mov word ptr ss:[bp+CGA_BRES_XINC], ax
			M_sign16ToInc cx, ax
			mov word ptr ss:[bp+CGA_BRES_YINC], ax
 
	mov dx, word ptr ss:[bp+CGA_BRES_OPTION] ; COR 
	mov bx, 0								 ; ERRO
	mov cx, word ptr ss:[bp+CGA_BRES_YERR]	 ; CONTADOR
	inc cx

	cga_line_looppx:
		IF DEBUG_BRESENHAM EQ 0
			push bx
				mov ax,  word ptr ss:[bp+CGA_BRES_X]
				mov bx,  word ptr ss:[bp+CGA_BRES_Y]
				call cga_pixel
			pop bx
		ELSE
			call debug2
		ENDIF
		mov ax, word ptr ss:[bp+CGA_BRES_XINC] ;   x+=XINC
		add word ptr ss:[bp+si], ax
		add bx, word ptr ss:[bp+CGA_BRES_XERR] ; err+=XERR
		IF DEBUG_BRESENHAM EQ 1
			call debug1
		ENDIF
		cmp bx, word ptr ss:[bp+CGA_BRES_YERR] ; err==YERR?
		jl cga_line_looppx_cont

			mov ax, word ptr ss:[bp+CGA_BRES_YINC]
			add word ptr ss:[bp+di], ax			   ; y+=YINC
			sub bx, word ptr ss:[bp+CGA_BRES_YERR] ; err-=YERR
				
			IF DEBUG_BRESENHAM EQ 1
				push ax
					mov al, '*'
					call tty_char
				pop ax
				call debug1
				push ax
					mov al, '*'
					call tty_char
				pop ax
			ENDIF
		cga_line_looppx_cont:

	loop cga_line_looppx
		
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	mov sp, bp
	pop bp
	pop ax
ret
debug2: 
		push ax
			mov ax, word ptr ss:[bp+CGA_BRES_X]
			call tty_crlf
			call tty_U16
			mov ax, word ptr ss:[bp+CGA_BRES_Y]
			call tty_sp
			call tty_U16
		pop ax 
ret

;------------------------------------------------------------------------------
; 6. cga_lineThick: desenha um segmento de reta CGA com thickness (chama a várias cga_line_)
; AX, BX: X0, Y0
; CX, DX: X1, Y1
; STACK: [THICK|COR]
cga_lineThick: 
	CGA_BRES_OPT EQU +4
	push bp
	mov bp, sp

	cmp word ptr ss:[bp+CGA_BRES_OPT], 1
	ja cga_line_havethick
		pop bp
		jmp cga_line

	cga_line_havethick:
		;--------------------
		push 0
		CGA_BRES_THICK  EQU -2
		push 0
		CGA_BRES_CNTX   EQU -4
		push 0
		CGA_BRES_CNTY   EQU -6
		push 0

		push si
		push ax	
		push bx	
		push cx
		push dx	
			mov dx, word ptr ss:[bp+CGA_BRES_OPT]
			xchg dh, dl			; DL = THICK
			xor dh, dh			; DH = 0
			dec dl
			mov word ptr ss:[bp+CGA_BRES_THICK], dx 
			shl dx, 1		; thick*2+1 
			inc dl  
			mov word ptr ss:[bp+CGA_BRES_CNTX], dx
			mov word ptr ss:[bp+CGA_BRES_CNTY], dx
		pop dx
		push dx

		; descontar thick
		sub ax, word ptr ss:[bp+CGA_BRES_THICK]	
		sub bx, word ptr ss:[bp+CGA_BRES_THICK]
		sub cx, word ptr ss:[bp+CGA_BRES_THICK]
		sub dx, word ptr ss:[bp+CGA_BRES_THICK]
		
		cga_line_thickY:

					; guardar contador X
			mov si, word ptr ss:[bp+CGA_BRES_CNTX]
			push si
			push ax	; guardar coordenada X
			push cx
					; push à opção
			mov si, word ptr ss:[bp+CGA_BRES_OPT]
			push si

			cga_line_thickX:
				call cga_line 
				inc ax
				inc cx
				dec word ptr ss:[bp+CGA_BRES_CNTX]
				jnz cga_line_thickX
			pop si
			pop cx
			pop ax
			pop si
			mov word ptr ss:[bp+CGA_BRES_CNTX], si

				; incrementar coordenada Y
		inc bx
		inc dx
		dec word ptr ss:[bp+CGA_BRES_CNTY]
		jnz cga_line_thickY
	pop dx
	pop cx
	pop bx
	pop ax 
	pop si

	mov sp, bp
	pop bp
ret

IF DEBUG_BRESENHAM EQ 1
	debug1:
	push ax
		call tty_crlf
		M_tty_wrstr "err:"
		mov ax, bx
		call tty_U16 
		M_tty_wrstr " yerr:"
		mov ax, word ptr ss:[bp+CGA_BRES_YERR] 
		call tty_U16 
		M_tty_wrstr " c:"
		mov ax, cx
		call tty_U16 
		xor ax, ax
		INT 16h
	pop ax
	ret
ENDIF

;------------------------------------------------------------------------------
; 7. cga_fillScr: enche o ecrã com byte AL
cga_fill:
push ax
push cx
push di
push es
	mov cx, 0B800h
	mov es, cx
	mov cx, UNIT_8K
	xor di, di
	rep stosb
	mov cx, 0BA00h
	mov es, cx
	mov cx, UNIT_8K
	xor di, di
	rep stosb
pop es
pop di
pop cx
pop ax
ret

;------------------------------------------------------------------------------
; 8. cga_copyToVRAM: copia de DS:BX para B8000h, e de DS:BX+8KB para BA000h
cga_copyToVRAM:
	cld
	mov ds, ax
	mov si, bx
	mov cx, (320*100)/4
	
	mov ax, 0B800h				; [es:di] = pagina 1
	mov es, ax
	xor di, di
	rep movsb					; [es:di] = [ds:si]

	mov si, bx
	add si, 02000h
	mov cx, (320*100)/4
	mov ax, 0BA00h				; [es:di] = pagina 2
	mov es, ax
	xor di, di
	rep movsb					; [es:di] = [ds:si]
ret

	
;------------------------------------------------------------------------------
; 9.A. M_drawClockNeedle: desenha uma agulha do relógio centrada em BOOTSCR_CLOCK_X/Y
M_drawClockNeedle MACRO RTC_NORM_, SIZE_, THICK_, COLOR_
LOCAL @@loop1
	
	push ax
	xor ah, ah
	xor dx, dx		
	mov bx, 359
	mul bx			; AX = A*359
	
	call angleCorrect
	mov bx, RTC_NORM_
	div bx			; AX = (A*359)/60
	
	
	
	mov bl, SIZE_
	push ax
	call getCosVector
	mov cx, ax

	pop ax
	call getSinVector
	mov dx, ax

	; desenhar vetor desde (AX,BX)
	mov ax, BOOTSCR_CLOCK_X
	mov bx, BOOTSCR_CLOCK_Y
	add cx, BOOTSCR_CLOCK_X
	add dx, BOOTSCR_CLOCK_Y
	push word ptr ((THICK_) SHL 8) OR COLOR_
	call cga_lineThick
	pop ax
	pop ax
ENDM

;------------------------------------------------------------------------------
; 9. draw_clock: desenha clock em BOOTSCR_CLOCK_X/Y
cga_clock:
	mov cx, 0
	mov bl, 0
	loopClock:
	
		M_RTC_Read_ RTC_SEC		; ler RTC
		call BCDtoU8
		cmp bl, al				; BL=CÓPIA DOS SEGUNDOS
		jne loopClockNotSame	; se diferir mostrar ponteiros
	
	jmp loopClock

		loopClockNotSame:
			push cx
			push bx
			push ax			
				xchg al, ah
				mov al, bl
				M_drawClockNeedle 60, 6, 1, BOOTSCR_CLOCK_BKCOLOR


				xchg al, ah
				M_drawClockNeedle 60, 6, 1, 0
					
				M_RTC_Read_ RTC_MIN
				call BCDtoU8	;norm div thick cor
				M_drawClockNeedle 60, 6, 2, 0
				M_drawClockNeedle 60, 6, 1, 2
		
		
				M_RTC_Read_ RTC_HOUR
				call BCDtoU8
				cmp al, 11
				jle loopClock1
					sub al, 12
				loopClock1:
				M_drawClockNeedle 12, 7, 3, 0
				M_drawClockNeedle 12, 7, 2, 1
			
				call makeShortBeep
				M_Countdown dx, 0FFFh
			pop ax
			pop bx
			pop cx

			inc cx
			cmp cx, 2
			jne skipAnounce
				push ax
				push bx
				push cx
					;call speak_time
					;call speaker_end
					mov al, bl
					M_drawClockNeedle 60, 6, 1, BOOTSCR_CLOCK_BKCOLOR
				pop cx
				pop bx
				pop ax
			skipAnounce:
			mov bl, al

			cmp cx, BOOTSCR_CLOCK_ITS
			je skipClock
				db 0EAh
				dw loopClock
				dw BIOS_SEG
			skipClock:
ret


;------------------------------------------------------------------------------
; 15. getVector: a componente dum "vetor tabelado"/BL na tabela SI

getVector:
push dx
push si
	add si, ax				; SI = AX (OFFSET NA TABELA)
	mov al, byte ptr cs:[si]
	cbw						; AX = R * SIN(DEG)
	idiv bl
	cbw
pop si
pop dx
ret
    
;------------------------------------------------------------------------------
; 15. getSinVector: a componente seno dum "vetor tabelado"/BL
; ax=0..359
getSinVector:
	mov si, offset sinTab
	cmp ax, 360
	jle getSinVector_
		sub ax, 360
	getSinVector_:
	cmp ax, 90
	jle getSVQ1
	cmp ax, 180
	jle getSVQ2
	cmp ax, 270
	jle getSVQ3
	jmp getSVQ4
	getSVQ1:
		jmp getVectorEnd	; Q1: sin(ax)
	getSVQ2:
		M_MinusAX	 180	; Q2: sin(180-ax)
		jmp getVectorEnd
	getSVQ3:
		M_AXMinus	 180	; Q3: -sin(ax-180)
		jmp getInvertVectorEnd
	getSVQ4:
		M_MinusAX	 360	; Q4: -sin(360-ax)
		jmp getInvertVectorEnd
		
getVectorEnd:
	call getVector
	ret
getInvertVectorEnd:
	call getVector
	M_InvertSign
	ret
	
getCosVectorEnd:
	push bx 
		mov bx, 90
		sub bx, ax
		mov ax, bx
	pop bx
	call getVector
	ret
getCosInvertVectorEnd:
	push bx 
		mov bx, 90
		sub bx, ax
		mov ax, bx
	pop bx
	call getVector
	M_InvertSign
	ret
;------------------------------------------------------------------------------
; 16. getCosVector: a componente coseno dum "vetor tabelado"/BL
; ax=0..359
getCosVector:
	mov si, offset sinTab
	cmp ax, 360
	jle getCosVector_
		sub ax, 360
	getCosVector_:
	cmp ax, 90
	jle getCVQ1
	cmp ax, 180
	jle getCVQ2
	cmp ax, 270
	jle getCVQ3
	jmp getCVQ4
	getCVQ1:				; Q1: cos(ax)
		jmp getCosVectorEnd
	getCVQ2:				; Q2: -cos(180-ax)
		M_MinusAX 180		
		jmp getCosInvertVectorEnd
	getCVQ3:				; Q3: -cos(ax-180)
		M_AXMinus 180	
		jmp getCosInvertVectorEnd
	getCVQ4:				; Q4: cos(360-ax)
		M_MinusAX 360
		jmp getCosVectorEnd
                                                                                                                         

sinTab:

 db 0
 db 2
 db 4
 db 6
 db 8
 db 10
 db 13
 db 15
 db 17
 db 19
 db 21
 db 23
 db 25
 db 27
 db 29
 db 31
 db 33
 db 35
 db 37
 db 39
 db 41
 db 43
 db 45
 db 47
 db 49
 db 51
 db 53
 db 54
 db 56
 db 58
 db 60
 db 62
 db 64
 db 65
 db 67
 db 69
 db 71
 db 72
 db 74
 db 76
 db 77
 db 79
 db 80
 db 82
 db 83
 db 85
 db 86
 db 88
 db 89
 db 91
 db 92
 db 93
 db 95
 db 96
 db 97
 db 98
 db 99
 db 101
 db 102
 db 103
 db 104
 db 105
 db 106
 db 107
 db 108
 db 109
 db 110
 db 110
 db 111
 db 112
 db 113
 db 113
 db 114
 db 115
 db 115
 db 116
 db 116
 db 117
 db 117
 db 118
 db 118
 db 119
 db 119
 db 119
 db 119
 db 120
 db 120
 db 120
 db 120
 db 120
 db 120
 db 120
 db 120