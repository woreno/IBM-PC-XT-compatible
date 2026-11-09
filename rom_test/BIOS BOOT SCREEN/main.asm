; testa as FUNCIONALIDADES da ROM 
; produzindo um executavel a executar em MS-DOS para rodar em dosbox
; algumas coisas como (os ecrãs e o som) não são possiveis concatenar
; com o executável, pelo que se lê do disco e se aloca memória para estes
; testar:
;  - ECRA DE BOOT (RLE)
;  - LEITOR DE HORAS (ACP)
;  - MENU DA BIOS 
;  - UPDATE (COM/FDD) o update por HDD só funciona em VIRTUALBOX
;    porque o virtualbox monta C: como diretório e tem setor de arranque
;
;-------------------------------------------------------------------------
DATA					EQU code
BIOS_SEG				EQU code
; STACK aos 12KB+256
STACKTOP				EQU 256
TTY_MODE				EQU 1
TTY_ISCGA				EQU 0
TTY_MAKE_ROLLSOUND		EQU 0 
BUILD_BIOS_VIDEO		EQU 0
BUILD_BIOS_USEVSYNC		EQU 0
BUILD_BIOS_KEYB			EQU 1
BUILD_BOOTSCR_DRAW		EQU 1
BUILD_BOOTSCR_ANIM		EQU 1
BUILD_BOOTSCR_CLOCK		EQU 1
BOOTSCR_CLOCK_ITS		EQU 5
BOOTSCR_CLOCK_X			EQU 286
BOOTSCR_CLOCK_Y			EQU 78
BOOTSCR_CLOCK_BKCOLOR	EQU 3 
BOOTSCR_TALKING_CLOCK   EQU 1
BUILD_TALKING_CLOCK		EQU 1
BUILD_BIOS_DISPLAY		EQU 0
;-------------------------------------------------------------------------
SPK_PARAMS_SEG_			EQU 50h
RESOURCES_USES_FILE		EQU 1
RESOURCES_FILE			EQU "res.bin"
;-------------------------------------------------------------------------

TEST_MENU				EQU   0
TEST_PORTS				EQU   1
	RTC_DEBUG				EQU   1
	; para simular memoria RTC numa array; o relogio nao vai funcionar com isto
TEST_BOOTSCREEN			EQU   0
TEST_SPEAK				EQU   0
TEST_DISABLE_IRQ0		EQU   0
TEST_FUNCS				EQU   0

;-------------------------------------------------------------------------
include MACROS.INC
include DEFS.INC
include RESDEF.INC
;include SPKDEF.INC
;-------------------------------------------------------------------------
code SEGMENT PUBLIC USE16 '_TEXT'
ASSUME cs:code, ds:code, es:code
org 0
Start:
	cli
	mov ax, cs
	mov ss, ax
	mov sp, offset sstack + 512
	call ds2cs


	IF TEST_PORTS EQ 1
		call tty_init
		call do_plel_test
	ENDIF

	IF TEST_FUNCS EQ 1
		call tty_init
		mov cx, UNIT_IN_SEGS_64K
		mov es, cx
		xor di, di
		tty_inputTest:
			call tty_crlf
			call tty_inputHex
			jc tty_inputTestErr

			call tty_sp
			call tty_Hex16
			jmp tty_inputTestCont

			tty_inputTestErr:
			call ds2cs
			M_tty_wrStr " err"

			tty_inputTestCont:

			xor ax, ax
			INT 16h
		jmp tty_inputTest

	ENDIF


	IF TEST_BOOTSCREEN EQ 1
		call tty_init
		call resources_init 
		mov ax, cs
		mov ds, ax
		call speaker_setup
		;call play_chime
		INCLUDE BOOTSCR.INC
		call tty_init
		;call p_menuShowSign
		xor ax, ax
		INT 16h
		;call CHS_testConvert
	ENDIF

	IF TEST_MENU EQ 1
		call tty_init
	call p_menuKeybInt_io
		;mov bl, 0h
		;call p_upd_FromDisk
		;call p_upd_FromDiskette
		call p_menuShow
		INCLUDE B_MENU.INC
	ENDIF

	IF TEST_SPEAK EQ 1 
		call tty_init
		call resources_init 
		call speaker_setup
		loopi:
			call tty_crlf
			call speak_time
		jmp loopi
	ENDIF 

	IF TEST_DISABLE_IRQ0 EQ 1
		; teste desligar a INT10
		;mov ax, cs
		;mov es, ax
		;mov ah, 13h
		;mov al, 1
		;xor bh, bh
		;mov bl, 4Fh
		;mov cx, offset IRQMSGSz - offset IRQMSG
		;xor dh, dl
		;mov bp, offset IRQMSG
		;INT 10h
		M_IRQ_OFF TIMER_IRQ
		xor al, al
		mov ah, 4Ch
		INT 21h
		hlt
		IRQMSG: db "IRQ0 desligado"
		IRQMSGSz:
	ENDIF
	hlt_
	INCLUDE DRAW.INC
	INCLUDE UTIL.INC
	INCLUDE TTY.INC
	INCLUDE RES.INC
	INCLUDE B_RTC.INC
	INCLUDE B_PORTS.INC
	
	CURSOR_POSN     DW  0
	tty_col			DB  0
	tty_row			DB  0
	tty_att			DB  0
	tty_lastRow		DB  0
	tty_lastCol		DB  0
	tty_lastAtt		DB  0
	tty_strbuff		DB	8 dup(0)
	tty_strbuffEnd  DB  0

	align 16
	sstack:
		db 512 dup(0)
	
	 
code ends
end Start
END 
