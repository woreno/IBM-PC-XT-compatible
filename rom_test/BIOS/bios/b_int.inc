;====================================================================
;== TRATADORES DE EXCEPÇÕES/INTERRUPÇÕES ============================
;====================================================================

; 11. p_divoverflow_int
; 12. p_nmi_int
; 13. p_dummy_int
; 14. p_dummy_irq
; 15. p_equipment_int
; 16. p_memsize_int
; 17. p_timerTest_int
; 18. p_bootstrap_int
; 19. p_adcVol_irq
; 10. IntTable

;------------------------------------------------------------
; 1. p_divoverflow_int
;------------------------------------------------------------
p_divoverflow_int:				; Divide overflow
	M_dispERROR ERROR_DIVIDE
	jmp p_divoverflow_int
	
;------------------------------------------------------------
; 2. p_nmi_int
;------------------------------------------------------------
p_nmi_int		:				; NMI
	cli							; desligar interrupções

	; guardar os registos na BDA
	; esta já não vai ser mais guardada
	mov ax, STACKSEG			; stack logo após área de 
	mov ss, ax					; memória da BIOS
	mov sp, STACKTOP

	M_dispERROR ERROR_NMI		; mostrar no visor ERRE

	M_DispDigits16 0, bx		; mostrar o segmento
	M_BlinkeLeds
	hlt_
	;IF BUILD_BIOS_MENU EQ 1
	;	db 0EAh 
	;	dw offset p_jmp_test_mem
	;	dw BIOS_SEG 
	;ENDIF

	
;------------------------------------------------------------
; 3. p_dummy_int
;------------------------------------------------------------
p_dummy_int:
iret

;------------------------------------------------------------
; 4. p_dummy_irq
;------------------------------------------------------------
p_dummy_irq:
	push ax
		mov al, PIC_CMD_EOI
		out PIC0_L0, al
		out PIC1_L1, al
	pop ax
iret

;------------------------------------------------------------
; 5. p_equipment_int
;------------------------------------------------------------
p_equipment_int:
	M_ENTER_INT 11h, "EQUIP"
	push ds
		assume ds:DATA
		mov ax, DATA
		mov ds, ax
		mov al, equip_flag0
		mov ah, equip_flag1
		assume ds:CODE
	pop ds
	M_EXIT_INT
iret

;------------------------------------------------------------
; 6. p_memsize_int
;------------------------------------------------------------
p_memsize_int:
	M_ENTER_INT 12h, "MEMSIZE"
		push ds
		assume ds:DATA
		mov ax, DATA
		mov ds, ax
		mov ax, memory_size 
		pop ds
		assume ds:CODE
	M_EXIT_INT
iret
  
;------------------------------------------------------------
; 7. p_timerTest_int
;------------------------------------------------------------
p_timerTest_int:
	push ds
	push ax
		assume DS:DATA
		call ds2data
		mov ax, time_lo
		test al, 5
		jz p_timerTestInt_exit
			call tty_dot
			mov al, clock_test
			cmp al, 0
			je p_timerTestInt_exit
			dec clock_test
		p_timerTestInt_exit:
		assume DS:CODE
	pop ax
	pop ds
iret 
 
;------------------------------------------------------------
; 8. p_bootstrap_int
;------------------------------------------------------------
IF BUILD_BIOS_BOOT EQ 1

p_bootstrap_int:

	; *** anunciar unidade a arrancar

	call ds2cs
	mov si, offset str_bootFrom
	call tty_line
	call p_sys_prn_drive	
	call tty_dot
	call tty_dot
	call tty_dot

	; *** ler primeiro setor

	xor dh, dh			; dh = cabeça 0 
	xor bx, bx
	mov es, bx			 	
	mov bx, boot_loc
	mov word ptr es:[bx+510], 0
	mov ax, 0201h		 ; ler | 1 setor
	mov cx, 1			 ; cl = cil 0, setor 1
	int 13h
	jnc p_bootstrap_test2

		mov si, offset str_bootIOError	; INT13 falhou
		call p_sys_prn_booterror
		iret
				
	; *** verificar se tem assinatura
		
	p_bootstrap_test2:
	push ax
		mov ax, 0
		cmp es:[bx+510], 0AA55h
	pop ax
	je b_bootstrap_asSign
		mov si, offset str_bootSignError	; Não há assinatura no setor
		call p_sys_prn_booterror
		iret
		b_bootstrap_asSign:
	pop ax
	
	; *** indicar que vai arrancar

	call tty_crlf
	push ax
	push dx
		M_DispLeds 01010101b
	pop dx
	pop ax

		;  *** imprimir os bytes da 1ª partição 
		;  *** testar vetor 41h, e INT13h AH=8
	IF INT13_DEBUG_PARAMS EQ 1
		call tty_clear
		call p_debugBootPart
	ENDIF

		; *** escrever SingleStep se "SWITCH 0" ON
	IF BUILD_SSTEP EQ 1
		M_PPI_TestDip 0
		test al, 1
		jz p_sys_skip_sstep
			call tty_clear
			call ds2cs
			mov si, offset str_singleStep
			call tty_line
			call tty_crlf 
		p_sys_skip_sstep:
	ENDIF

	call tty_saveCur		; ***  passar controlo do cursor
	call tty_saveCurToBDA	; ***  à INT10

	; *** preparar SingleStep se "SWITCH 0" ON
	IF BUILD_SSTEP EQ 1
		assume DS:DATA
			mov ax, DATA
			mov ds, ax
			xor ah, ah
			M_PPI_TestDip 0
			mov SINGLE_STEP_ON, al
		assume DS:CODE
					
		shl ax, 8	; se "1" passa ao bit 8	
		push ax		; por na stack
		popf		; pop da stack para flags
	ENDIF
	
	; *** salto FAR para o código carregado

	xor ax, ax		; DS=0
	sahf			; zerar outras flags
	mov ds, ax
	db 0EAh
	dw boot_loc
	dw 0
ENDIF


;------------------------------------------------------------
; 9. p_adcVol_irq
;------------------------------------------------------------
IF BUILD_BIOS_ADCVOL EQ 1
p_adcVol_irq:
	cli
	push ax
	push bx
	push dx

		; ler o estado do ultimo potenciometro
		mov dx, VOLADC_READ
		in al, dx
		push ax
			; por A ou B no 4º digito
			and al, 1
			add al, 0Ah
			mov dx, DISP_REG_3
			out dx, al
		pop ax
		
		shr al, 1
		cmp al, 100
		jbe p_adcVol_irq_
			mov al, 100
		p_adcVol_irq_:

		xor ah, ah
		mov bl, 10				; al/=10
		div bl
		xchg ah, al				; al=Resto
		mov dx, DISP_REG_0
		out dx, al
		xchg ah, al				
		
		xor ah, ah
		div bl
		xchg ah, al				; al/=10
		mov dx, DISP_REG_1
		out dx, al
		xchg ah, al				

		mov dx, DISP_REG_2
		out dx, al

		mov al, PIC_CMD_EOI
		out PIC0_L0, al
		out PIC1_L1, al
	pop dx
	pop bx
	pop ax
	iret
ENDIF

;------------------------------------------------------------
; 10. IntTable
;------------------------------------------------------------
IntTable:
	DW	offset  p_divoverflow_int	; INT 00h divide error
	DW	offset  p_dummy_int			; INT 01h Single step								
	DW	offset  p_nmi_int			; INT 02h NMI - Non maskable interrupt 			
	DW	offset  p_dummy_int			; INT 03h Breakpoint								
	DW	offset  p_dummy_int 		; INT 04h by INTO instruction if Overflow flag set	
	DW	offset  p_dummy_int			; INT 05h
	DW	offset  p_dummy_int			; INT 06h
	DW	offset  p_dummy_int			; INT 07h
	IF BUILD_BIOS_IRQ0 EQ 1 
		DW	offset  p_timer_int			; INT 08h  /IRQ0
	ELSE
		DW	offset	p_dummy_irq			; INT 08h  /IRQ0
	ENDIF
	DW	offset	p_dummy_irq			; INT 09h  /IRQ1
	DW	offset	p_dummy_irq			; INT 0Ah  /IRQ2
	DW	offset	p_dummy_irq			; INT 0Bh  /IRQ3
	DW	offset	p_dummy_irq			; INT 0Ch  /IRQ4
	DW	offset	p_dummy_irq			; INT 0Dh  /IRQ5
	IF BUILD_BIOS_FDD EQ 1
		DW  offset  p_fdd_int			; INT 0Eh  /IRQ6
	ELSE
		DW	offset  p_dummy_irq			 
	ENDIF
	DW	offset	p_dummy_irq			; INT 0Fh  /IRQ7
	IF BUILD_BIOS_VIDEO EQ 1
		DW	offset  VIDEO_IO		; INT 10h  
	ELSE
		DW	offset  tty_int10h		 
	ENDIF
	DW	offset  p_equipment_int		 ; INT 11h
	DW	offset  p_memsize_int		 ; INT 12h
	
	IF (BUILD_BIOS_FDD+BUILD_BIOS_HDD) NE 0
		DW	offset  p_disk_io			 ; INT 13h
	ELSE
		DW 0
	ENDIF 

	DW	offset  p_rs232_io 			 ; INT 14h
	DW	offset  p_services_io		 ; INT 15h
	DW	offset  p_keyb_io			 ; INT 16h
	DW	offset  p_printer_io		 ; INT 17h
	DW	offset  p_dummy_int			 ; INT 18h

	IF BUILD_BIOS_BOOT EQ 1
		DW	offset  p_bootstrap_int		 ; INT 19h
	ELSE
		DW	offset  p_dummy_int			 
	ENDIF

	DW	offset  p_timer_io			 ; INT 1Ah
	DW	offset  p_dummy_int			 ; INT 1Bh - CTRL BREAK
	DW	offset  p_timerTest_int		 ; INT 1Ch - TIMER BREAK ADDR

	IF BUILD_BIOS_VIDEO EQ 1
		DW	offset  VIDEO_PARAMS		 ; INT 1Dh - VIDEO PARAMETERS
	ELSE
		DW	offset  CGA_VIDEO_PARAMS	 ; INT 1Dh - VIDEO PARAMETERS
	ENDIF
	
	IF BUILD_BIOS_FDD EQ 1
		DW	offset  FDD_PARAMS		 ; INT 1Eh - DISK PARMS
	ELSE
		DW	offset  0				 ; INT 1Eh - DISK PARMS
	ENDIF
	DW	0							 ; INT 1Fh - POINTER TO VIDEO EXT

IntTableLength EQU $-IntTable

IF BUILD_BIOS_PIC EQ 2
	IntTable2:
		DW	offset  p_dummy_irq		 ; INT 70h /IRQ8
		DW	offset  p_dummy_irq		 ; INT 71h /IRQ9
		DW	offset  p_dummy_irq		 ; INT 72h /IRQ10
		DW	offset  p_dummy_irq		 ; INT 73h /IRQ11
		DW	offset  p_dummy_irq		 ; INT 74h /IRQ12
		DW	offset  p_dummy_irq		 ; INT 75h /IRQ13
		DW	offset  p_dummy_irq		 ; INT 76h /IRQ14
		DW	offset  p_dummy_irq		 ; INT 77h /IRQ15
	IntTableLength2 EQU $-IntTable2
ENDIF
