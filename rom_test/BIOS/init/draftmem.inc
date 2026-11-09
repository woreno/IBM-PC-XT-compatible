

	; desenha os 256KB no ecra
	; cada 4 pixeis = 32 bytes
	;  - se o conteudo dos 32 bytes for igual
	;    ao 1º byte fica igual
	
;IF ((TEST_RAM_ON_SCREEN+INT13_DRAWMEM) NE 0)
;ram_drawMemory:
;	push ax
;	push bx
;	push cx
;	push ds
;	push es
;	push si
;	push di
;	
;	mov ax, 01010101b
;	mov ah, al
;					; ES:DI = video
;					; apagar com COR 1
;	mov cx, 0BA00h
;	mov es, cx
;	xor di, di			
;	mov cx, UNIT_8K/2
;	rep stosw
;	mov cx, 0B800h
;	mov es, cx
;	xor di, di			
;	mov cx, UNIT_8K/2
;	rep stosw
;	
;	xor di, di
;	mov bx, 0		
;	mov si, 0		
;	mov ds, si		; a DRAM se usa ds:si
;
;	ram_drawMemoryLoop:
;		; XOR ao bloco de RAM de 32bytes em DS:SI
;		xor si, si 
;		mov cx, 31
;		mov al, byte ptr ds:[si]
;		loopXOR:
;			inc si
;			xor al, byte ptr ds:[si]
;			loop loopXOR
;			
;		xor si, si
;		; se existir diferenças -> COR 2
;		; se não por com a cor original
;		or al, al
;		jz nodiffs
;			mov al, 10101010b
;			jmp done
;		nodiffs:
;			mov al, ds:[si]
;		done:
;			mov es:[di], al
;
;		; aumentar ds (32 bytes)
;		mov cx, ds
;		add cx, 2
;		mov ds, cx
;
;		inc di					; próximos 1 byte CGA (4 pixeis)
;		mov cx, di
;		sub cx, bx				; retira o inicio da linha
;		test cx, 64				; e testa se chegou ao pixel 256 (256/4=64)
;	jz ram_drawMemoryLoop	
;		
;	mov cx, es
;	xor cx, UNIT_IN_SEGS_8K		; trocar o endereço da linha (página)
;	mov es, cx					; voltar ao inicio da linha
;	cmp cx, 0BA00h
;	je isSecondPage
;		add di, 80-64			; se agora é a vez da 1ª página
;								; tirar os 64 percorridos, 
;								; e incrementar 80 (próxima linha)
;		cmp di, (80*64)
;		je ram_drawMemoryEnd	
;		mov bx, di
;		jmp ram_drawMemoryLoop	
;	isSecondPage:				; se é a vez da 2ª página
;		sub di, 64				; tirar os 64 percorridos
;		mov bx, di
;		jmp ram_drawMemoryLoop	
;	ram_drawMemoryEnd:
;	pop di
;	pop si
;	pop es
;	pop ds
;	pop cx	
;	pop bx		
;	pop ax
;ret
;ENDIF

;IF (TEST_WRITERAM_FF+TEST_RAM_ON_SCREEN) NE 0
;ram_fill256K:
;	cld
;	xor dx, dx
;	mov ax, 0FFFFh
;	rBlankFill1:
;		xor di, di
;		mov es, dx
;		mov cx, 0FFFFh
;		rep stosw 
;		stosw 
;		add dx, UNIT_IN_SEGS_64K
;		cmp dx, UNIT_IN_SEGS_256K
;		jne rBlankFill1
;ret
;ENDIF









;IF ACTION_ EQ TEST_RAM_RW	; escrever e ler
;mov cx, 2
;ram_loop_rw:
;	mov ds:[bx], al		; escrever AL 
;	nop
;	xor ah, ah
;	mov ah, ds:[bx]     ; ler para AH
;	cmp ah, al 
;	je @@ram_loop_skip1
;		call RamError
;	@@ram_loop_skip1:	; sair sem erro
;	not al
;
;	loop ram_loop_rw
;ENDIF
;IF ACTION_ EQ TEST_RAM_DISPLAY
;	mov ah, ds:[bx]     ; ler para AH
;	M_DispDigits16 ax, cx
;	wait
;	M_CountDown cx, 0FFFFh
;ENDIF
;IF ACTION_ EQ TEST_RAM_RLOOP	; ler
;	mov ah, ds:[bx]             
;ENDI
;IF ACTION_ EQ TEST_RAM_RLOOP
;	jmp @@ram_loop_bytes
;ELSE
;	jnz @@ram_loop_bytes
;ENDIF F



	; IF TEST_RAM_ON_SCREEN EQ 1
	; 	mov al, 2
	; 	M_fakeCall cga_setVideo
	; 	mov al, 0
	; 	M_fakeCall cga_clearZero
	; ENDIF

	; IF (TEST_WRITERAM_FF+TEST_RAM_ON_SCREEN) NE 0
	; 	M_fakeCall ram_fill256K
	; ENDIF

	;IF TEST_RAM_ON_SCREEN EQ 1
	;test_ram_onscreen:
	;	; prencher memória toda a FF
	;	; 32 bytes equivalem a 1 bloco (de 4 pixeis)
	;	mov dx, 0	
	;	memloop:	
	;
	;		mov es, dx
	;		mov ax, 0		; por a PRETO UM BLOCO
	;		mov di, 0
	;		mov cx, TEST_RAM_ON_SCR_BYTES/2
	;		rep stosw		; es:[di]=0
	;	
	;		;or dx, dx		; (se o primeiro não fazer)
	;		;jz skipFill		; por A BRANCO DE NOVO o anterior
	;		;	sub dx, TEST_RAM_ON_SCR_BYTES/16
	;		;	mov es, dx
	;		;	mov ax, 0FFFFh
	;		;	mov di, 0
	;		;	mov cx, TEST_RAM_ON_SCR_BYTES/2
	;		;	rep	stosw
	;		;	add dx, TEST_RAM_ON_SCR_BYTES/16
	;		;skipFill:
	;
	;		cmp dx, UNIT_IN_SEGS_256K
	;		jne notTop
	;			mov dx, 0
	;		notTop:
	;
	;		add dx, TEST_RAM_ON_SCR_BYTES/16		; dx=proximo bloco
	;		M_fakeCall ram_drawMemory
	;	
	;		IF TEST_RAM_ON_SCR_BUT EQ 1
	;			wait						; ESPERAR PELO BOTAO
	;		ENDIF
	;
	;	jmp memloop
	;	jmp test_ram_onscreen
	;
	;ENDIF
 
; antes da stack ser válida desenhar memoria
; IF INT13_DRAWMEM EQ 1
; 	M_fakeCall ram_fill256K
; 	;M_fakeCall drawMemory
; 	;wait
; ENDIF


	; LER BYTE A BYTE E ESCREVER NO VISOR
	; IF TEST_RAM_BYTE_BYTE EQ 1
	; 	M_RamTest TEST_RAM_W, RAM_SEGMENTS, RAM_DELTASEGS, TEST_RAM_TOPSEG, 0, 1
	; 	wait                ; parar enquanto TEST
	; 	M_RamTest TEST_RAM_DISPLAY, RAM_SEGMENTS, RAM_DELTASEGS, TEST_RAM_TOPSEG, 0, 1
	; ENDIF




	;
	;; ESCREVER E LER APÓS NA DRAM DA CGA
	;IF TEST_VRAM EQ 1
	;	M_RamTest  TEST_RAM_RW, VRAM_SEGMENTS, VRAM_DELTASEGS, VRAM_TOPSEG, 055h, 3
	;	M_fakeCall rotateLedsLeft
	;	M_fakeCall beep
	;ENDIF


	
;IF INT13_DRAWMEM EQ 1 
;	call drawMemory
;ENDIF
	
 