	mov al, TTY_MODE
	M_fakeCall cga_setVideo
	mov al, 0
	
	; colocar stack na memoria video aos 8KB
	mov ax, 0B800h
	mov ss, ax
	mov sp, UNIT_8K 

	; testar a stack na memória video
	mov cx, 96
	loopstack1:
		push word ptr 0AA55h
	loop loopstack1
	mov cx, 96
	loopstack2:
		pop ax
		cmp ax, 0AA55h
		je loopstack2_cont
			call noBootScreen
		loopstack2_cont:
	loop loopstack2
	

	; Descomprimir 2 planos CGA comprimidos com RLE 
	; para a memória CGA

	mov al, 2
	M_fakeCall cga_setVideo
	
	mov dx, CGA_PALETTE
	mov al, 10h
	out dx, al 

	mov al, RES_ID_CGA0_DRAW
    call dsToResource
	xor si, si
	mov ax, 0B800h
	mov es, ax
	xor di, di
	call rle_uncompress
	
	mov al, RES_ID_CGA1_DRAW
    call dsToResource
	xor si, si
	mov ax, 0BA00h
	mov es, ax
	xor di, di
	call rle_uncompress

	; desenhar estado dos DIPs
	CGA_DIP_ST_Y   EQU 74	; em px	
	CGA_DIP_ST_X   EQU 124	; em px
	CGA_DIP_ST_DYE EQU 5	; em linhas pares
	CGA_DIP_ST_DX  EQU 12	; em px

	mov ax, 0B800h
	mov es, ax
	mov ax, 0BA00h
	mov ds, ax
	mov di, (CGA_DIP_ST_X/4)+((CGA_DIP_ST_Y/2)*(320/4))
	mov si, di
	
	call getDipState
	mov al, 10101101b  ;  A TIRAR
	mov cx, 0
	cgaDips_loop:
		test al, 1
		jnz cgaDips_ON
			 add di, CGA_DIP_ST_DYE*(320/4) 
			 M_push5 ax, bx, cx, dx, si
			 	mov si, offset CGA_DRAW_JUMPERRECTS
			 	mov ax, cx
			 	mov bl, 4*2
			 	xor ah, ah
			 	mul bl
			 	add si, ax
			 	mov ax, word ptr cs:[si]
			 	mov bx, word ptr cs:[si+2]
			 	mov cx, word ptr cs:[si+4]
			 	mov dx, word ptr cs:[si+6]
			    push 0
			    push offset cga_pattRect2_
			    call cga_rect
			    add sp, 4
			 M_pop5 ax, bx, cx, dx, si
		cgaDips_ON:
			mov byte ptr es:[di], 0
			mov byte ptr ds:[di], 0
			mov byte ptr es:[di+320/4], 0
			mov byte ptr ds:[di+320/4], 0
		shr al, 1
		add si, (CGA_DIP_ST_DX/4)
		mov di, si
		inc cx
		cmp cx, 8
		jne cgaDips_loop 

	
	IF BUILD_BOOTSCR_CLOCK EQ 1  

		; fazer a animacao do relogio
		M_RTC_Read_ RTC_STATUS_D
		test al, 80h
		jz cga_dontdrawclock
				call cga_clock
		cga_dontdrawclock:
 
	ENDIF

	IF BUILD_BOOTSCR_ANIM EQ 1
		mov ax, 0
		mov bx, 0
		mov cx, 320
		mov dx, 200
		push 0
		push offset cga_pattRect3_
		call cga_rect
		add sp, 4
		push 0
		push offset cga_pattRect2_
		call cga_rect
		add sp, 4
		
		mov ax, 0B800h
		mov es, ax
		mov ax, 0BA00h
		mov ds, ax
		CGA_ANIM_BLK_W	EQU (320/4)
		CGA_ANIM_BLK_H	EQU 30
			xor di, di
			xor ax, ax
			mov bx, 01FFFh
			cga_anim_loop:

				mov si, di					; guardar di
				mov dx, (CGA_ANIM_BLK_H/2)	; nº de linhas para cada plano
				cga_anim_rows:
					mov cx, (CGA_ANIM_BLK_W/4) ; nº de bytes a preencher
					cga_anim_block:
						mov byte ptr es:[di], 0		  
						mov byte ptr ds:[di], 0         ; preencher
						inc di
						loop cga_anim_block
			
					add di, (320/4)-(CGA_ANIM_BLK_W/4) 
					dec dx
				jnz cga_anim_rows

				mov di, si					   ; voltar atrás
				add di, (CGA_ANIM_BLK_W/4)	   ; adicionar bloco
				add ax, (CGA_ANIM_BLK_W/4)
				cmp ax, 320/4				  
				jne cga_anim_skip
									 ; se o bloco bateu no fim
									 ; adicionar as linhas
					add di, ((CGA_ANIM_BLK_H/2)-1)*(320/4)
					mov ax, 0
				cga_anim_skip:
				M_Countdown cx, bx
				sub bx, 10

			cmp di, 8000
			jb cga_anim_loop

		ENDIF 
 
noBootScreen:
