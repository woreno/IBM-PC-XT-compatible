TEST_VIDEOLATCH    EQU 0	    ; roda os bits nas latches
TEST_VIDEOMODES    EQU 0	    ; itera os modos de video
TEST_VIDEOPIXEL    EQU 0	    ; escreve pixel-a-pixel
TEST_VIDEOMEMORY   EQU 0	    ; testa a VRAM
TEST_VIDEO_TEXT    EQU 0        ; testa modo texto 40 e 80 colunas 
TEST_VIDEO_PAL     EQU 0        ; testa paletas
TEST_VIDEO_PALWAIT EQU 0	    ; carregar no botao para mudar paleta
TEST_PARALLEL_PORT EQU 0		

	
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

IF TEST_VIDEOPIXEL EQ 1
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
		wait	
		M_Countdown dx, 0FFFFh
		inc di
	jmp pxloop
ENDIF

IF TEST_VIDEOLATCH EQ 1
	; confirmar que os bits são postos na latch
	wait
	M_Countdown dx, 0FFFFh
	mov al, 1
	cga_test_latch:
		mov dx, CGA_MODE
		out dx, al
		shl al, 1
		or al, 1
		wait
		M_Countdown dx, 0FFFFh
		cmp al, 1111111b
	jne cga_test_latch
	M_beep 0FFFFh
ENDIF

IF TEST_VIDEOMODES EQ 1
	mov al, 0
	M_fakeCall cga_setVideo
	M_beep 0FFFFh
	wait
	M_CountDown cx, 0FFFFh
	mov al, 1
	M_fakeCall cga_setVideo
	M_beep 0FFFFh
	wait
	M_CountDown cx, 0FFFFh
	mov al, 2
	M_fakeCall cga_setVideo
	M_beep 0FFFFh
	M_CountDown cx, 0FFFFh
	M_beep 0FFFFh
	wait
ENDIF

IF TEST_VIDEOMEMORY EQ 1
	M_fakeCall cga_testVideoMem
ENDIF
 
IF TEST_VIDEO_TEXT EQ 1
	M_CGA_testTextMode 0
	M_beep 0FFFFh
	M_Countdown dx, 0FFFFh
	M_Countdown dx, 0FFFFh
	M_Countdown dx, 0FFFFh
	wait

	M_CGA_testTextMode 1
	M_beep 0FFFFh
	M_Countdown dx, 0FFFFh
	M_Countdown dx, 0FFFFh
	M_Countdown dx, 0FFFFh
	wait

	mov al, 2
	M_fakeCall cga_setVideo
	M_beep 0FFFFh
	M_CountDown cx, 0FFFFh
	M_beep 0FFFFh
	wait
ENDIF 


IF TEST_VIDEO_PAL EQ 1
	xor al, al
	mov dx, CGA_PALETTE
	cga_change_pal:
		out dx, al
		mov cx, ax
			M_beep 0FFFFh
		mov ax, cx
		IF TEST_VIDEO_PALWAIT EQ 1
			wait
		ELSE
			M_Countdown cx,0FFFFh
		ENDIF
		add al, 16
		cmp al, 040h
		jne cga_change_pal
	M_beep 0FFFFh
	wait
ENDIF
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

IF TEST_PARALLEL_PORT EQ 1 
	
ENDIF