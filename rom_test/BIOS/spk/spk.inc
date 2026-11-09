; 1.        speaker_setup: configura IRQ0, PIT0 e PIT2
; 2.          speaker_end: repoem IRQ0
; 3.         speaker_play: reproduz as amostras de ES:SI até DI
; 4. speaker_irq0_Handler: repete ou interpola a amostra (tratador IRQ0)

; é preciso DECLARAR ANTES:
;  * SPK_OFFSETS            offset para array de samples
;  * SPK_PARAMS_SEG         segmento onde guardar alguns dados

; estas definicoes ligar para debug
SPK_DEBUG_IRQ0			 EQU 1
SPK_DEBUG_INTERPOL		 EQU 0
SPK_DEBUG_OFFSETS		 EQU 1
SPK_TIME_PRINTS			 EQU 1	; imprime o que está a falar

; estas definicoes nunca mudar!
SPK_FREQ_PIT0_KHZ		 EQU (1193/16)				; PIT0 = 16KHz
SPK_USE_INTERPOL_DIV	 EQU (16/SPK_SAMPLERATE_KHZ)

;-------------------------------------------------------------------------
; 1. speaker_setup: configura IRQ0, PIT0 e PIT2
speaker_setup:
	M_IRQ_OFF 0
	mov ax, SPK_PARAMS_SEG
	mov es, ax
	mov ax, 0
	mov ds, ax
	M_saveINT 8, spk_param_prevIRQHandler
	M_defIRQ 0, speaker_irq0_Handler	; tratador temporizador 0
	
								; reprogramar o temporizador 0
								; para que tenha uma frequencia
								; igual ao do som
	 

	mov al, 036h				; contador 0: modo contagem 3, normal, L+H
	out 43h, al
	mov al, (SPK_FREQ_PIT0_KHZ AND 0FFh)
	out 40h, al
	mov al, SPK_FREQ_PIT0_KHZ SHR 8
	out 40h, al

	mov al, 0B0h	; pic contador 2: modo 0, normal, L+H
	out 43h, al
ret
;-------------------------------------------------------------------------
; 2. speaker_end: repoem IRQ0
speaker_end:
	mov ax, SPK_PARAMS_SEG
	mov es, ax
	mov ax, 0
	mov ds, ax
	M_restoreINT 8, spk_param_prevIRQHandler
ret

;-------------------------------------------------------------------------
; 3. speaker_play: reproduz as amostras de ES:SI até DI
; ES:SI aponta para a 1ª AMOSTRA DE SOM
;       são lidas 2 AMOSTRAS para spk_param_currSample/nextSample
;       para continuar a interpolação em IRQ0
;		spk_osmp_cnt: conta quantas iterações para fazer
;				      oversampling
speaker_play:	
push ds
	mov ax, SPK_PARAMS_SEG
	mov ds, ax
													; no campo de dados "spk_param"
													; por o contador a ZERO
	xor ax, ax										; passar 2 amostras de ES:SI
	mov word ptr ds:[offset spk_param_cnt], ax		; caso preciso fazer interpolação	
	mov bl, es:[si]									; 1ª AMOSTRA
	mov byte ptr ds:[offset spk_param_currSample], bl
	mov al, es:[si+1]								; 2ª AMOSTRA
	mov byte ptr ds:[offset spk_param_nextSample], al

	in al, 61h
	or al, 11b			
	out 61h, al			
	
	M_IRQ_ON 0
	sti
	speaker_sound_loop:			; ESPERAR PELA INT0
		hlt						; E VERIFICAR SE ACABOU
		cmp si, di
		jne speaker_sound_loop
	cli 
	in al, 61h
	and al, 0FCh
	out 61h, al 
	M_IRQ_OFF 0
pop ds
ret


;-------------------------------------------------------------------------
; 4. speaker_irq0_Handler: repete ou interpola a amostra (tratador IRQ0)
;

speaker_irq0_Handler_exit1:
	mov al, PIC_CMD_EOI
	out PIC0_L0, al
	iret 
speaker_irq0_Handler:

	cmp si, di						; TERMINOU SOM?
	je speaker_irq0_Handler_exit1
		
									; TERMINOU CONTADOR DE DIVISAO?
	mov cl, byte ptr ds:[offset spk_param_cnt]
	cmp cl, SPK_USE_INTERPOL_DIV
	jne speaker_irq0_noNewSample

						; SE TERMINOU CONTADOR, CARREGAR
						; AMOSTRA CURRENTE E SEGUINTE
						; E RESET AO CONTADOR
		
		xor al, al
		mov byte ptr ds:[offset spk_param_cnt], al ; ...reset contador
		
		inc si						; ...proximas 2 amostras
		
		IF SPK_DEBUG_IRQ0 EQ 1
			call speaker_irq0_debugSample
		ENDIF

		mov bl, es:[si] 
		IF SPK_DONT_INTERPOLATE EQ 0
			mov byte ptr ds:[offset spk_param_currSample], bl
			mov al, es:[si+1]
			mov byte ptr ds:[offset spk_param_nextSample], al
		ENDIF
	jmp speaker_irq0_next

	speaker_irq0_noNewSample:
		IF SPK_DEBUG_IRQ0 EQ 1
			call tty_dot
		ENDIF

	speaker_irq0_next:
		IF SPK_DONT_INTERPOLATE EQ 0
			call speaker_irq0_interpolate
		ENDIF
		
		inc byte ptr ds:[offset spk_param_cnt]
	
	;------------- BL contem o VALOR a TOCAR --------------
	; por o speaker a ms = (sample * 60) / 255;
	; o contador do speaker está ao ritmo de FS=1.193180MHz
	; o novo contador = ((sample * 60) / 255) * 1e-6 / (1/FS)
	;                 = ((sample * 60) / 255) * 1e-6 * 1.193180 * 1e6
	;				  =  (sample * 71.59) / 255

	
	mov ax, 72
	mul bl
	shr ax, 8
 	out 42h, al
	mov al, ah
 	out 42h, al
	 
	mov al, PIC_CMD_EOI
	out PIC0_L0, al
	iret

speaker_irq0_interpolate:

	mov al, byte ptr ds:[offset spk_param_nextSample]

	; interpolação linear entre dois samples 
					; Y0+((Y1-Y0)*CNT)/MUL
			
	mov dl, 0			; DL = TROCAR SINAL DEPOIS
	mov dh, ch			; DH = AMOSTRA CURRENTE (Y0)

	cmp dh, al			; SE Y0 É MAIOR, TROCAR SUBTRACAO
	jb speaker_interpol_currIsLower

		xchg al, dh		; trocar 
		mov dl, 1

	speaker_interpol_currIsLower:
		sub al, dh		; (nextSample-currSample)
		xor ah, ah		
		mul cl			; * counter

		mov bl, SPK_USE_INTERPOL_DIV-1
		or al, al
		;jz skipInterpol0
		div bl			; AL = (...)/15

		test dl, 1
		jz speaker_skipInterpolNoSign
			not al		; SE SINAL ANTERIOR
			inc al		; COMPLEMENTAR CH
					
		speaker_skipInterpolNoSign:
			add ch, al 		; + currSample
			mov bl, ch		; BL=Y0+((Y1-Y0)*CNT)/MUL
			
			IF SPK_DEBUG_INTERPOL EQ 1
				call speaker_interpol_debug
			ENDIF

	jmp speaker_skipInterpol1

	speaker_skipInterpol0:
		mov bl, ch		; BL=Y0, SE AL=0

	speaker_skipInterpol1:
ret


IF SPK_DEBUG_IRQ0 EQ 1
speaker_interpol_debug:
push ax
	call tty_crlf
	mov al, byte ptr ds:[offset spk_param_cnt]
	call tty_Hex8
	call tty_sp
	mov al, byte ptr ds:[offset spk_param_currSample]
	call tty_U8
	mov al, '>'
	call tty_char
	mov al, bl		
	call tty_U8
	mov al, '>'
	call tty_char
	mov al, byte ptr ds:[offset spk_param_nextSample]
	call tty_U8
	call tty_sp
	M_Countdown ax, 0FFFFh
pop ax
ret

speaker_irq0_debugSample:
	push ax
		call tty_crlf
		mov ax, si 
		call tty_Hex16
		mov al, '/'
		call tty_char
		mov ax, di 
		call tty_Hex16
	pop ax
	ret
ENDIF