

;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
; as seguintes array sao usadas por RamTest com ss:sp
; e tem os segmentos iniciais para cada coluna da DRAM: 
; 0KB, 256KB, 512KB, 576KB, 640KB (com tamanhos 256KB, 256KB, 64KB, 64KB) 

	RAM_SEGMENTS:		; 4 segmentos
	dw   0, 04000h, 08000h, 09000h, 0A000h
	
	VRAM_SEGMENTS:		; 1 segmento toda a VRAM
	dw   0B800h, 0BC00h

	; M_RamTest itera memória
	;	       ACTION_: o que fazer TEST_RAM_..RW/W/R/V/R_LOOP/DISPLAY
	;         ARRSEGS_: array de segmentos
	;       DELTASEGS_: nº de segmentos a incrementar
	;          TOPSEG_: em que segmento parar
	;      VALUESTART_: valor inicial a escrever
	;        VALUEINC_: valor a adicionar ao anterior, byte-a-byte


M_RamTest MACRO ACTION_, ARRSEGS_, DELTASEGS_, TOPSEG_, VALUESTART_, VALUEINC_
	LOCAL @@ram_loop_mem, @@ram_loop_mem_end, @@ram_loop_segs, @@ram_loop_bytes,  @@ram_loop_skip1
	mov cx, BIOS_SEG		; definir stack na ROM
	mov ss, cx
	mov bp, offset ARRSEGS_ ; definir offset
	mov di, bp

	@@ram_loop_mem:
	
	; colocar nos LEDs "o número da coluna"
	; 1 coluna RAM = 1º LED, 2 coluna RAM = 2º RAM
	; IF ACTION_ NE TEST_RAM_DISPLAY
	; 	mov bl, al
	; 		mov cx, bp
	; 		sub cx, cs:[offset ARRSEGS_]
	; 		shr cl, 1
	; 		mov al, 1
	; 		shl al, cl			; 1<<(bp-ARRSEGS/2)
	; 		not al
	; 		mov dx, LEDS_REG
	; 		out dx, al
	; 	mov al, bl
	; ENDIF

	
	mov cx, ss:[bp]					; ds = seg.inicio
	mov ds, cx
	M_DispDigits16 ds, bx			; mostrar o segmento
	mov al, VALUESTART_
	
	@@ram_loop_segs:	; iterar conjunto de segmentos
	
		inc al		; incrementar o valor por 1 a cada conj. de segmentos
		mov dx, 0E0h

		xor bx, bx			; ---------------- bx roda 64K
		@@ram_loop_bytes:
		
			IF ACTION_ EQ TEST_RAM_RW	; escrever e ler
				mov cx, 2
				ram_loop_rw:
					mov ds:[bx], al		; DS:[BX]=AL 
					nop
					xor ah, ah
					mov ah, ds:[bx]     ; AH=DS:[BX]
					cmp ah, al 
					je @@ram_loop_skip1	; SAO IGUAIS?
						call RamError
					@@ram_loop_skip1:	; sair sem erro
					not al
					loop ram_loop_rw
			ENDIF
			IF ACTION_ EQ TEST_RAM_W
					;mov ah, al
					;in al, dx
					;out dx, al
					;mov al, ah
				mov ds:[bx], al     ; DS:[BX]=AL 
			ENDIF
			If ACTION_ EQ TEST_RAM_V
					;mov ah, al
					;in al, dx
					;out dx, al
					;mov al, ah
				mov ah, ds:[bx]     ; AH=DS:[BX]
				cmp ah, al 
				je @@ram_loop_skip1
					call RamError
				@@ram_loop_skip1:	; sair sem erro
			ENDIF

			add al, VALUEINC_
			inc bx
			jnz @@ram_loop_bytes	; ---------------- bx roda 64K

									; iterar registo de segmento
		mov cx, ds
		add cx, DELTASEGS_ ; ds = seg. inicio + delta
		mov ds, cx
			
		mov si, ax					; mostrar o segmento
			M_DispDigits16 ds, bx
			M_beep
			M_Countdown bx
		mov ax, si

		mov cx, ds
		mov bx, ss:[bp+2]  ; bx = seg.fim 
		cmp bx, cx
		jne @@ram_loop_segs 
		
		add bp, 2

	cmp cx, TOPSEG_
	je @@ram_loop_mem_end  ; loop_segs
			
		db 0EAh
		dw @@ram_loop_mem
		dw BIOS_SEG

	@@ram_loop_mem_end:

ENDM
	; entra ds:bx a localização que falhou
	; ah = lido
	; al = escrito
	; bp = indice na array
RamError:
	; guardar bit escrito/lido
	mov es, ax					 ; ES=LIDO/ESCRITO
	mov si, bx					 ; BP=de DS:BX
		M_DispDigits 0Eh, 0Ah, 0Ah, ERROR_RAM ; indicar ERR
		M_errorBeep
		M_Countdown cx
		M_errorBeep
		M_Countdown cx
		M_DispDigits16 ds, cx    ; indicar segmento 
		M_errorBeep
			M_Countdown cx
			M_Countdown cx
		M_DispDigits16 bx, cx    ; indicar offset 
		M_errorBeep
			M_Countdown cx
			M_Countdown cx
	mov ax, es					 ; restaurar ax de es
	mov bx, si
	
	; pesquisar o bit falido
	xor al, ah
	xor cl, cl   ; al tem as diferenças (a 1)
	RamloopFailedBits:
		mov ah, al      
		and ah, 1        ; ah=bit baixo
		cmp ah, 1        ; se ah for igual a 1, existe erro
		je RamloopFailedFound
		shr al, 1		; shiftar
		inc cl
		cmp cl, 8
		jne RamloopFailedBits

	; isto não pode acontecer

	RamloopFailedFound:


	; mostrar [ coluna | bit falido ] 
	; a memória tem os bits MD0..D7 
	; pela ordem contrária do CPU (D0:D7)
	mov al, 7   ; subtrair a 7 para obter a
	sub al, cl  ; posicao bit falido

	mov dx, DISP_REG_0
	out dx, al
	mov ax, bp  ; o numero da coluna é (BP-DI)/2
	sub ax, di
	shr al, 1
	mov dx, DISP_REG_1
	out dx, al

	mov bx, si

	; mostrar byte escrito/lido
	; es    tem o byte escrito/lido
	; ds:bx tem ainda a localização
	RamloopErrorLoop:

		mov ax, es		; alternar byte lido/escrito e
		xchg al, ah		; guardar novamente em ES
		mov es, ax
			
		mov dx, DISP_REG_2	; mostrar um dos bytes
		out dx, al
		shr al, 4
		mov dx, DISP_REG_3
		out dx, al
							; beep
		M_errorBeep 
		M_Countdown cx

		; fazer sucessivas escritas e leituras ao byte afetado
		mov al, 055h
		mov cx, 0FFh
		RamloopError_testW:
			mov ds:[bx], al
		loop RamloopError_testW
		mov cx, 0FFh
		RamloopError_testR:
			mov al, ds:[bx]
		loop RamloopError_testR
	jmp RamloopErrorLoop
	
	ramTestsLines:
		M_beep
		M_beep
		M_beep
		M_dispDigits 0,0Ah,0,0
		mov cx, 0
		mov bx, 1					; A0=1
		addrLines1:
			mov ax, cx				; mostrar no ecra
			mov dx, DISP_REG_0
			out dx, al
			mov dx, DISP_REG_1
			shr al, 4
			out dx, al
			xor dx, dx				; repetir leitura
			addrLines1l:
				mov ds:[bx], al	
				dec dx
				jnz addrLines1l
			inc cx
			shl bx, 1				; A1=1
			wait		
			or bx, bx				
			jnz addrLines1			; ...até A16
			mov bx, 1000h			; A16=1
			mov cx, 16				
		addrLines2:
			mov ax, cx
			mov dx, DISP_REG_0
			out dx, al
			mov dx, DISP_REG_1
			shr ax, 4
			out dx, al
			mov ds, bx
			xor dx, dx
			addrLines2l:
				mov ds:[bx], al	
				dec dx
				jnz addrLines2l
			inc cx
			shl bx, 1
			wait
			or bx, bx
			jnz addrLines2