TEST_RAM_RW				EQU 1
TEST_RAM_W				EQU 2
TEST_RAM_V				EQU 3
TEST_RAM_R				EQU 4
TEST_RAM_RLOOP			EQU 5
TEST_RAM_DISPLAY		EQU 6

RAM_DELTASEGS			EQU 1000h ; núm. de seg. a iterar (64KB) 
VRAM_TOPSEG				EQU 0BC00h		 	
VRAM_DELTASEGS			EQU 40h   ; núm. de seg. a iterar (1KB) 

DO_SOME_RAM_TEST EQU (TEST_RAM+TEST_VRAM+TEST_RAM_LINES+TEST_RAM_NO_DMA+TEST_RAM_BYTE_BYTE)

mov ax, 0
M_DispDigits16 ax, ax
	
call ramStart

IF DO_SOME_RAM_TEST NE  0
	INCLUDE RAMTEST.INC
ENDIF
;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
; TESTES DE MEMÓRIA SEM DMA

ramStart:
	mov ax, 1111h
	M_DispDigits16 ax, ax
	 
	IF TEST_RAM_LINES EQ 1
		M_PPI_showDip1
		M_PPI_TestDip 3	
		jz ramWithoutTestsLines
			call ramTestsLines
	ramWithoutTestsLines:
	ENDIF


	; ESCREVER E LER APÓS NA DRAM 
	IF TEST_RAM_NO_DMA EQ 1				
		
		M_PPI_showDip1
		M_PPI_TestDip 2	; o DIP 3 do 2º grupo ON fazer testes
		jnz ramTestsWithoutDMA
	
		call skipRamTestsWithoutDMA

		ramTestsWithoutDMA:
			M_beep
			M_RamTest TEST_RAM_RW, RAM_SEGMENTS, RAM_DELTASEGS, TEST_RAM_TOPSEG, 055h, 3
			M_beep 0FFFFh
			M_fakeCall rotateLedsLeft

		skipRamTestsWithoutDMA:
	
	ENDIF

;-----------------------------------------------------------------------------------
;-----------------------------------------------------------------------------------
; TESTES DE MEMÓRIA COM DMA

	; LIGAR O DMA-REFRESHES PARA ATUALIZAR DRAM
	IF TEST_RAM_ENABLE_DMA EQ 1
		mov cl, 11000011b
		M_DispLed cl
		; testar DMA
		IF BUILD_BIOS_DMATEST EQ 1
			include DMATEST.INC
		ENDIF
		mov cl, 00011000b
		M_DispLed cl
		include DMACFG.INC
		M_OpenLeds
		
		mov ax, 0
		M_DispDigits16 ax, ax

		IF TEST_RAM_DMA_PAUSE EQ 1	
				; parar e verificar se DMA está a funcionar:
				; - PIT1 66KHz 
				; - linhas A0:7 a rodar 
				; - XMEMR seguido de IOW
				; - todas as RAS0:3 a zero
				
				M_PPI_showDip1
				M_PPI_TestDip 3	; o DIP 4 do 2º grupo ON se pausar
				jz ramTestsNoDMAPause
				wait
				ramTestsNoDMAPause:
		ENDIF
	ENDIF

	
	; ESCREVER TODA A DRAM, E VOLTAR A LER PASSADO UM POUCO
	IF TEST_RAM EQ 1

	M_PPI_showDip1
	M_PPI_TestDip 2	; o DIP 3 do 2º grupo ON fazer testes
	jnz ramTestsWithDMA
	
		call ramOK	; passar à frente dos testes DMA

	ramTestsWithDMA:

		mov al, 2
		M_fakeCall cga_setVideo

		
	do_ram_tests_with_dma:

		M_RamTest TEST_RAM_W, RAM_SEGMENTS, RAM_DELTASEGS, TEST_RAM_TOPSEG, 055h, 3
		IF TEST_RAM_USEBUTTON EQ 1
			wait                        ; parar enquanto TEST
		ELSE		
			mov cx, TEST_RAM_WAITTIME
			mov si, 0B800h
			mov bp, 0
			ram_test_wait:
				mov al, 0A0h			; comando identify ao disco
				mov dx, (1F0h+06h)
				out dx, al
				xor al, al
				mov dx, (1F0h+02h)
				out dx, al
				mov dx, (1F0h+0Ah)   
				out dx, al
				mov dx, (1F0h+04h) 
				out dx, al
				mov dx, (1F0h+0Ch)
				out dx, al
				mov al, 11101100b
				mov dx, (1F0h+0Eh)
				out dx, al				; mostrar tempo no visor
				M_DispDigits16 cx, ax
				mov dx, 1F0h
				mov bx, cx
				mov cx, 0FFFFh
				ramTestDummyIO:
					in al, dx 
					loop ramTestDummyIO
				xor si, 0200h				; trocar página video CGA
				mov es, si
				mov cx, (160*200)/4
				xor di, di
				mov dx, CGA_STATUS		
				M_CGA_WaitVSync			; esperar VSYNC
				mov dx, 1F0h
				ramTestDummyIO2:		; escrever na CGA, ler do disco
					inc bp
					mov ax, bp
					stosb
					;mov es:[di], al
					;inc di
					in al, dx 
					loop ramTestDummyIO2
				mov cx, bx
				dec cx
				jz ram_test_waitend
					db 0EAh
					dw offset ram_test_wait
					dw BIOS_SEG
			ram_test_waitend:
		ENDIF
		M_RamTest TEST_RAM_V, RAM_SEGMENTS, RAM_DELTASEGS, TEST_RAM_TOPSEG, 055h, 3
	ENDIF

ramOK:

	M_PPI_showDip0

IF TEST_RAM_ENABLE_DMA EQ 1
; definir uma stack válida: para que o CPU 
; possa chamar os tratadores e retornar
	mov ax, STACKSEG
	mov ss, ax
	mov sp, STACKTOP

; testar a stack
	mov ax, 0AA55h
	push ax
	mov ax, 01234h
	push ax
	;
	pop ax
	cmp ax, 01234h
	jne StackError
	pop ax
	cmp ax, 0AA55h
	jne StackError
	call StackOK 

	StackError:
		StackError_:
		M_dispERROR	ERROR_STACK 
		jmp StackError_
	StackOK:

	xor ax, ax
	M_DispDigits16 ax, cx
	mov dx, LEDS_REG
	out dx, al
	M_fakeCall beep
	M_fakeCall rotateLedsLeft
	M_fakeCall beep
	mov ax, STACKSEG
	mov ss, ax
	mov sp, STACKTOP

ENDIF
