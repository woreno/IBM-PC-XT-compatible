
; p_menuKeybInt
; tratador do teclado para o écrã de inicio
; decide se entrar no menu da BIOS ou efetuar testes
; segundo a tecla pressionada no arranque
;  - os testes estão espalhados pela ROM 
;    este é o ponto de entrada apenas


IF TEST_MENU EQ 0

strMemtest			 db "Teste de memoria", 0
strCgaMemtest		 db "Teste CGA", 0
strSerialTest		 db "Teste COM1", 0
strJoyTest			 db "Teste Joystick",0
strPlelTest			 db "Teste LPT1",0
strIOTest			 db "Teste IO", 0

p_menuKeybIntTitle:
	call p_menu_clearRed
	call ds2cs
	call tty_centeredStr
ret


p_menuKeybInt:
	cli
	M_IRQ_OFF TIMER_IRQ		 ; desativar temporizador
	M_IRQ_OFF KEYB_IRQ		 ; desativar teclado
							 
	IN	AL,PPI_REG_KEYB		 
	push ax					 ; guardar SCANCODE
							 
	IN	AL,PPI_REG_OPTS		 ; RESET ao TECLADO
	MOV	AH,AL				 
	OR	AL,80H				 
	OUT	PPI_REG_OPTS,AL		 
	XCHG	AH,AL			 
	OUT	PPI_REG_OPTS,AL		 
							 
	xor ax, ax				 ; novo tratador de TECLADO
	mov ds, ax
	M_defIRQ 1,   p_keyb_int 	
	mov al, PIC_CMD_EOI      ; fim de interrupção, eoi
	out PIC0_L0, al
	
	pop ax					 ; RESTAURAR SCANCODE

	cmp al, KEY_SCANCODE_M
	je p_jmp_test_mem
	cmp al, KEY_SCANCODE_V
	je p_jmp_test_cga
	cmp al, KEY_SCANCODE_C
	je p_jmp_test_serial
	cmp al, KEY_SCANCODE_I
	je p_jmp_test_io
	cmp al, KEY_SCANCODE_J
	je p_jmp_test_joy
	cmp al, KEY_SCANCODE_L
	je p_jmp_test_plel
	cmp al, KEY_SCANCODE_ENTER
	je p_jmp_enterMenu
iret
	
	p_jmp_enterMenu:
		M_IRQ_ON KEYB_IRQ ; ativar teclado
		sti
		call p_menuShow
	p_jmp_test_joy:
		mov si, offset strJoyTest 
		call p_menuKeybIntTitle
		db 0EAh
		dw offset do_joy_test
		dw BIOS_SEG
	p_jmp_test_plel:
		mov si, offset strPlelTest
		call p_menuKeybIntTitle 
		db 0EAh
		dw offset do_plel_test
		dw BIOS_SEG
	p_jmp_test_serial:
		mov si, offset strSerialTest 
		call p_menuKeybIntTitle 
		db 0EAh
		dw offset do_serial_test
		dw BIOS_SEG
	p_jmp_test_mem:
		mov si, offset strMemTest 
		call p_menuKeybIntTitle 
		db 0EAh
		dw offset do_ram_tests_with_dma
		dw BIOS_SEG
	p_jmp_test_cga:
		mov si, offset strCgaMemtest 
		call p_menuKeybIntTitle 
		db 0EAh
		dw offset do_cga_test
		dw BIOS_SEG
	p_jmp_test_io:
		mov si, offset strIOTest 
		call p_menuKeybIntTitle 
		db 0EAh
		dw offset do_io_test
		dw BIOS_SEG




p_menu_prnCpu:
	; rep prefix 
	sti
	mov si, 0
	mov cx, 0FFFFh
	rep lods byte ptr [es:si]
	or cx, cx
	jz p_menu_prnCpu1
		M_tty_wrStr "Intel 8088"
		ret
	p_menu_prnCpu1:
		M_tty_wrStr "Nec V20"

ret
ENDIF
