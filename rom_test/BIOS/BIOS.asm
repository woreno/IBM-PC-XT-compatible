;-----------------------------------
; DIP SWITCHES
; DIP 1 no arranque, TRAP
; DIP 2 ..parar em cada passo
; DIP 3 ..parar em INT
; DIP 4 mostrar registos
;---- --------------------
; DIP 5 -
; DIP 6 teste CGA
; DIP 7 testes RAM
; DIP 8 parar ao ligar DMA para refreshes
;-----------------------------------

.8086
.model tiny
;-------------------------------------------------------------------------------------
BIOS_START_OFFSET		EQU 030h   ; onde começa o código
BIOS_SEG				EQU 0F800h ; segmento desta ROM (2ª ROM neste PC)
include CONFIG.INC		; opções
include DEFS.INC		; definições de portas, registos, etc
include RESDEF.INC		; definições de recursos
include MACROS.INC		; macros
include B_MEM.INC		; esquema de memória (inclui Bios Data Area (BDA))
;-------------------------------------------------------------------------------------
code SEGMENT PUBLIC USE16 '_TEXT'
ASSUME cs:code, ds:code, es:code
org 0
Start:
	ROM_XSUM:			dw 0	 ; informação para verificação da ROM
	ROM_SIZE:			dw 0 	 ; 
	ROM_DATE:			db 0,0,0 ; 
	ROM_TIME:			db 0,0,0 ; 
	org 16
	ROM_RES_SEG_START	dw 0,0,0
	
	; espaço reservado para o resumo de RESOURCES

;-----------------------------------
; o código começa aqui (30h) 
	org BIOS_START_OFFSET			
ROMStart:
	cli
;-----------------------------------
; desligar DMA 
	M_CHECKPOINT 0
	mov al, 4
	out DMA_CMD, al
	M_WaitDMA
	
	;mov al, 0
	;M_fakeCall cga_setVideo
	;mov si, offset str_testing
	;M_text_wrStr 
;-----------------------------------
; PPI: definir interfaces 
	M_PPI_Config
;-----------------------------------
; PIT: ouve-se um som "vrrumm"
	include PIT.INC
;-----------------------------------
; VISOR: os digitos rodam, é feito checksum, e ouve-se um "beep"
	include INIT.INC
;-----------------------------------
; CGA
 M_CHECKPOINT 3
 IF BUILD_BIOS_NOVIDEO EQ 0

	M_PPI_showDip1
	M_PPI_TestDip 1
	jnz do_cga_test
		call skip_cga_test
	do_cga_test:
	
	M_PPI_TestDip 0
	jz do_cga_test1
	;M_fakeCall cga_testVideoByteMem
	M_fakeCall cga_testVideoPixel

	do_cga_test1:
	M_fakeCall cga_testVideoMem

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
	xor al, al
	mov dx, CGA_PALETTE

	cga_change_pal:
		out dx, al
		mov cx, ax
			M_beep 0FFFFh
		mov ax, cx
		wait
		M_Countdown cx,0FFFFh
		add al, 16
		cmp al, 040h
		jne cga_change_pal
	M_beep 0FFFFh
	wait
	
	skip_cga_test:
	IF BUILD_BOOTSCR_DRAW EQ 1
		include BOOTSCR.INC 
	ENDIF
 ENDIF

;-----------------------------------
; MEMÓRIA
M_CHECKPOINT 4
include RAM.INC

;-----------------------------------
; PIC
IF BUILD_BIOS_PIC NE 0 
	; PICs
	M_CHECKPOINT 6
	include PIC.INC
ENDIF

;-----------------------------------
; beep-beep
M_Countdown cx,0FFFFh
call makeShortBeep
M_Countdown cx,0FFFFh
call makeShortBeep

;-----------------------------------
; INICIAR
M_CHECKPOINT 7
M_DispDigits 3, 2, 1, 0	  ; mem, pic OK

; se BIOS inicializar o modo grafico
IF BUILD_BIOS_NOVIDEO EQ 0
	IF BUILD_BIOS_VIDEO EQ 1
		mov ax, 0
		mov ds, ax
		M_defINT 10h, VIDEO_IO
		mov ax, (TTY_MODE*2+1)
		INT 10h
		mov ah, 1
		mov cx, 0607h
		INT 10h
	ENDIF
ENDIF
M_DispDigits 7, 6, 5, 4		; video OK

IF JUST_TESTS EQ 1

	M_DispDigits 0Ah, 0Ah, 5, 5

	IF TEST_RAM_ENABLE_DMA EQ 1
		call tty_init
		call ds2cs
		mov si, offset str_testmode
		call tty_line
		IF BUILD_BIOS_KEYB EQ 1
			call ds2data
			assume DS:DATA
				MOV	BUFFER_HEAD,OFFSET KB_BUFFER	
				MOV	BUFFER_TAIL,OFFSET KB_BUFFER	
				MOV	BUFFER_START,OFFSET KB_BUFFER	
				MOV	BUFFER_END,OFFSET KB_BUFFER+32
				mov INTR_FLAG, 0
				mov ALT_INPUT, 0
				mov KB_FLAG_1, 0
				mov KB_FLAG, 0
				mov MFG_TST, 0
				mov bios_break, 0
				mov reset_flag, 0
			assume DS:CODE
			cli
			M_IRQ_OFF 1	
			xor ax, ax
			mov ds, ax
			M_defIRQ 1,   p_keyb_int
			M_defINT 16h, p_keyb_io
			M_defINT 1Bh, p_dummy_int
			M_defINT 5h,  p_dummy_int
			M_KeybOn
			M_KeybReset
			M_IRQ_ON KEYB_IRQ
			sti
		ENDIF
	ENDIF

	include TESTS.INC
	wait
	str_testmode db "Modo de teste",0
	p_dummy_int:
	iret
ELSE
	;IF BUILD_CHIME EQ 1
	;	mov ax, offset continue
	;	push ax
	;	EXTRN c_test
	;	jmp c_test
	;	continue:
	;ENDIF
	
	call p_sys_setup
	
	;---------------------------------------------------------------------------
	; outras inclusões:

	include B_INTS.INC
	IF BUILD_BIOS_RTC EQ 1
		include B_RTC.INC
	ENDIF
	IF BUILD_BIOS_MENU EQ 1
		include B_MENU.INC
	ENDIF
	IF BUILD_SSTEP EQ 1
		include B_SSTEP.INC
	ENDIF
	include B_DISK.INC
	IF BUILD_BIOS_HDD EQ 1
		include B_HDD.INC
	ENDIF
	IF BUILD_BIOS_FDD EQ 1
		include B_FDD.INC
	ENDIF
	include B_TIME.INC
	
	include B_SYS.INC
	IF (BUILD_BOOTSCR_DRAW+BUILD_BOOTSCR_CLOCK+BUILD_TALKING_CLOCK) NE 0
		include DRAW.INC
		assume DS:CODE
		include RES.INC
	ENDIF

ENDIF
include B_PORTS.INC
IF BUILD_BIOS_VIDEO EQ 1
	include B_VIDEO.INC
ENDIF
IF BUILD_BIOS_KEYB EQ 1
	include B_KEYB.INC
ENDIF
assume DS:CODE
include UTIL.INC
include TTY.INC

str_testing db "Testando PC...", 0

align 16  ; fill para checksum 
code ends
end Start
END 