
INCLUDE B_INT.INC	

; 1. DADOS -------------------------------------
;	 1.1. strings
; 2. TESTES, INSTALAÇÃO E ARRANQUE -------------
;	 2.1. p_sys_setup
;	 2.2. p_sys_memdata
;	 2.3. p_sys_memdet
;	 2.4. p_sys_rtc
;	 2.5. p_sys_installHandlers
;	 2.6. p_sys_initKeyb
;	 2.7. p_sys_equipDefaults
;	 2.8. p_sys_portsConfig
;	 2.9. p_sys_detectDrives
;	2.10. p_sys_scanChecksumCnt..............soma de bytes de blocos de 512 = 0
;	2.11. p_sys_scanChecksum.................EXPANSAO: chamada a p_sys_scanChecksumCnt
;	2.12. p_sys_initTimer
;	2.13. p_sys_installSingleStep
; 3. IMPRESSÃO ---------------------------------
;	 3.1. p_sys_prn_info
;	 3.2. p_sys_prn_dateTime
;	 3.3. p_sys_prn_and_cfg_ports
;	 3.4. p_sys_prn_drive
;	 3.5. p_sys_prn_booterror

;====================================================================
;== 1. DADOS ========================================================
;====================================================================

;------------------------------------------------------------
; 1.1. Strings
;------------------------------------------------------------
str_title:			db "Manel/PC 2022", 0
str_rominfo:		db "ROM ultima compilacao:", 0
str_romupdated:		db "A ROM foi atualizada!", 0
str_mem:			db "Memoria:", 0
str_memdata:		db "Memoria para dados:", 0
str_dips:			db "Dip-switches:", 0
str_sysres:			db "O sistema foi reiniciado", 0
str_exp:			db "A rastrear expansao...", 0
str_pics:			db "PICs:",0
str_timer:			db "A testar temporizador:", 0
str_bootFrom:		db "Arrancando da unidade ", 0
str_bytes:			db "bytes",0
str_keyb:			db "Teclado:",0
str_drives			db "Unidades:",0
str_fdd			    db "fdd", 0
str_hdd			    db "hdd", 0
str_equip:			db "Equip:",0
str_ports:			db "Portas:", 0
str_lpt:			db "lpt", 0
str_com:			db "com", 0
str_int:			db "INT",0
str_atention:		db "<!> ", 0
str_notdetected:	db "Nao detetado", 0
str_nocontroller:	db "Controlador nao presente ou invalido", 0
str_nobootdrive:	db "Sem alguma unidade para arranque; Sistema parado", 0
str_bootIOError:	db "Erro E/S na unidade", 0
str_bootSignError:  db "Setor sem assinatura de arranque", 0
IF BUILD_BIOS_RTC EQ 1
str_rtc:			db "Data/Hora RTC:", 0
ENDIF
str_rtc_nok:		db "RTC nao presente ou invalido; usando defaults...", 0
IF BUILD_SSTEP EQ 1
	str_singleStep:		db "Single-step ativo", 0
ENDIF

COM_IO_PORTS	dw		-1, 03F8h, 02F8h
COM_IRQS		db		-1, 3, 4
LPT_IO_PORTS	dw		-1, 0378h, 0278h
LPT_IRQS		db		-1, 7, 5


;====================================================================
;== 2. INSTALAÇÃO, TESTES E ARRANQUE ================================
;====================================================================

;------------------------------------------------------------
; 2.1. p_sys_setup
;------------------------------------------------------------
p_sys_setup:

	call tty_init
	call p_sys_prn_info			; imprimir alguma info da ROM
	call p_sys_memdata			; imprimir memoria RAM baixa usada
	call p_sys_memdet			; determinar memoria RAM total

	IF BUILD_BIOS_RTC EQ 1
		call p_sys_rtc			; detetar, ligar RTC e anunciar horas
		jnc p_sys_setup_rtc_ok
	ENDIF
		call ds2cs					; se RTC invalido,
		mov si, offset str_rtc_nok	; anunciar que os defaults vao ser usados
		call tty_line

	p_sys_setup_rtc_ok:
	
	call p_sys_installHandlers	; instalar tratadores de int. padrão
	
	IF BUILD_BIOS_KEYB EQ 1
		call p_sys_initKeyb		; inicializar teclado
	ENDIF

	IF BUILD_BIOS_MENU EQ 1
		call p_menuShowSign		; anunciar opções para entrada no 
	ENDIF						; MENU da BIOS e OUTROS TESTES

	call p_sys_equipDefaults	; def. flags de equipamento padrão

	IF BUILD_BIOS_PORTS EQ 1
		call p_sys_portsConfig		; atualizar portas segundo RTC
	ENDIF

	IF (BUILD_BIOS_HDD+BUILD_BIOS_FDD) NE 0
		call p_sys_detectDrives	; detetar unidades de disquetes e discos duros
	ENDIF
	sti

	IF BUILD_BIOS_SCANEXP EQ 1
		call p_sys_scanExpansion	; rastrear expansão e executar ROMs
	ENDIF
	
	IF BUILD_BIOS_IRQ0 EQ 1
		call p_sys_initTimer		; testar/inicializar temporizador 
	ENDIF							

	IF BUILD_BIOS_PIC EQ 2			; ativar cascata (2º PIC)
		M_IRQ_ON CASCATE_IRQ			
	ENDIF
	
	IF BUILD_BIOS_ADCVOL EQ 1
		M_IRQ_ON VOLADC_IRQ			; ligar interrupção para os potenciometros+ADC
	ENDIF

	IF BUILD_SSTEP EQ 1
		call p_sys_installSingleStep	; instalar passo-a-passo
	ENDIF
	call longWait
	call beep

	IF BUILD_BIOS_MENU EQ 1
		call p_menuHideSign			; retirar anuncio para entrar na BIOS	
	ENDIF

	; ----------------------------- Iniciar as tentativas de arranque

	IF BUILD_BIOS_BOOT EQ 0
		sti
		call ds2cs
		call tty_crlf				
		M_tty_wrStr "fim"
		hlt_
	ELSE


	IF BUILD_BIOS_RTC EQ 1
		call rtc_cfg_isOK			; colocar ordem de arranque 0,1,2,3 se  
		jc boot_cfg_invalid			; se bateria nao OK ou cookie nao ok
		call rtc_BootPriorityCheck	; se repetições na ordem de arranque
		jc boot_cfg_invalid
									; senão aceitar a ordem de arranque 
									; guardada no RTC

		M_RTC_Read RTC_RAM_BOOT_PRI
		jmp boot_cfg_valid

	ENDIF

	boot_cfg_invalid:
		mov al, 11100100b		
		call ds2cs
		call tty_crlf
		M_tty_wrStr "Arranque: ordem invalida, a usar a ordem habitual..."

	boot_cfg_valid:

						; iterar os bits do byte da ordem de arranque
						; se a unidade existir, chamar
						; a INT19h para arranque
		mov cx, 4
		boot_loop1:
			push cx

						; a unidade AL=XX foi detetada?
			call boot_cfg_driveExists
			jz boot_next
						
						; bit 1 vai para o 7
			push ax
				and al, 11b	; AL=unidade
				mov ah, al	; 
				and al, 0FDh;    eliminar 2º bit de AL
				and ah, 10b	;    eliminar 1º bit de AH
				rol ah, 6	;    mover 2º para a 8ª posição 
				or al, ah   ; DL=formato unidade BIOS
				mov dl, al
				INT 19h
			pop ax
		
		boot_next:
			shr al, 2		; próxima unidade
			pop cx
			dec cx
		jnz boot_loop1
		
							; aviso, sem unidade para arranque
		call ds2cs
		mov si, offset str_atention
		call tty_line
		mov si, offset str_nobootdrive
		call tty_str
		call beep
		call smallWait
		call beep
		call smallWait
		call beep
		hlt_				; PARAGEM total

							; a unidade em AL=0..3 foi detetada?
	boot_cfg_driveExists:	; se FLAG ZERO nao existe
		ASSUME DS:DATA
		push ax
		push bx
		push cx
		call ds2data
		mov bl, drives_map		; BL = UNIDADES EXISTENTES
		mov cl, al
		and cl, 11b
		mov al, 1
		rol al, cl				; 1<<AL
		test al, bl				; 
		pop cx
		pop bx
		pop ax
		ASSUME DS:CODE
	ret
ENDIF
 
;------------------------------------------------------------
; 2.2. p_sys_memdata
;------------------------------------------------------------
p_sys_memdata:
	call ds2cs
	call tty_crlf
	mov si, offset str_memdata
	call tty_str
	IF BUILD_SSTEP EQ 1
		mov ax, offset DATA:SINGLE_STEP_CODEIP
		call tty_U16
		mov al, '+'
		call tty_char
		mov ax, offset p_singleStep_intCodeLength
		call tty_U16
		mov al, '='
		call tty_char
		mov ax, offset DATA:SINGLE_STEP_CODEIP+offset p_singleStep_intCodeLength
	ELSE
		mov ax, offset DATA:SINGLE_STEP_CODEIP
	ENDIF
	call tty_U16
	mov si, offset str_bytes
	call tty_str

	; limpar dados à excepção da tty
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
		mov fdd_number, 0
		mov hdd_number, 0
		mov seek_status, 0
		mov motor_status, 0
		mov motor_count, 0
		mov disq_status, 0
		mov nec_status, 0
		mov time_lo, 0
		mov time_hi, 0
		mov time_ovf, 0
		mov bios_break, 0
		mov reset_flag, 0
		mov cursor_posn, 0
		mov clock_test, 0
		mov drives_map, 0
		mov int13_accesses, 1
		mov dip_switches, 0
		IF (BUILD_SSTEP+INT13_LOG) NE 1
			mov dos_crash, 0
			mov SINGLE_STEP_ON, 0
		ENDIF
	assume DS:CODE
ret


;------------------------------------------------------------
; 2.3. p_sys_memdet
;------------------------------------------------------------
p_sys_memdet:
	call ds2cs
	mov si, offset str_mem
	call tty_line
	call tty_saveCur
	 
	call tty_prnMem
 
	assume ds:DATA
		call ds2data
		shr cx, 6				; guardar em unidades de 1k 
		mov memory_size, cx
	assume ds:CODE
ret


;------------------------------------------------------------
; 2.4. p_sys_rtc
;------------------------------------------------------------
IF BUILD_BIOS_RTC EQ 1
p_sys_rtc:
	
	; ativar o oscilador interno
	; (escrever 010 nos bits 6:4 do registo A)
	M_RTC_Set_ RTC_STATUS_A, RTC_STATUS_A_CFG
	M_RTC_Set_ RTC_STATUS_B, RTC_STATUS_B_CFG
	call tty_crlf	
	call ds2cs

	call rtc_batIsOK
	jnc p_sys_rtc_ok

		M_tty_wrStr "Bateria RTC falhou ou RTC ausente!"
		jmp p_sys_rtc_failed

	p_sys_rtc_ok:

	call rtc_cfg_isOK
	jnc p_sys_rtc_ok2

		M_tty_wrStr "Os dados no RTC parecem estar invalidos!"
	
		p_sys_rtc_failed:
			IF BUILD_BIOS_RTC_INFO EQ 1
				call ds2cs
				M_tty_wrStr "..registos RTC:"
				call rtc_printinfo
				call tty_crlf
			ENDIF
			stc
			ret

	p_sys_rtc_ok2:
		IF BUILD_TALKING_CLOCK EQ 1
			call dsToSpkParams
			mov ax, SND_DECOMPRESSION_SEG
			mov word ptr ds:[offset spk_param_seg], ax ; guardar segmento

			mov al, RES_ID_VOICES
			call dsToResource
							; ds = segmento origem
	
			mov ax, SND_DECOMPRESSION_SEG
			mov es, ax		; es = segmento destino

			call acp_uncompress
			jc p_sys_rtc_nosound
		
			call speaker_setup 
			call speak_time
			call speaker_end 
		ELSE
			mov si, offset str_rtc
			call tty_str
			call rtc_printClock
		ENDIF
		clc
	ret
ENDIF
;------------------------------------------------------------
; 2.5. p_sys_installHandlers
;------------------------------------------------------------
p_sys_installHandlers:
	cld
	call ds2cs						; DS=CS
	mov si, offset str_pics
	call tty_line
	mov al, BUILD_BIOS_PIC
	call tty_U8

		mov ax, 0
		mov es, ax					; ES=0
		mov si, offset IntTable		; BIOS_SEG:IntTable
		mov di, 0
		mov cx, IntTableLength/2	
		mov ax, BIOS_SEG
		p_sys_iH1:
			movsw					; es:[di+=2] = ds:[si+=2]
			stosw					; es:[di+=2] = BIOS_SEG
		loop p_sys_iH1

	IF BUILD_BIOS_PIC EQ 2
		; instalar tratadores para o PIC2
			mov si, offset IntTable2
			mov di, 70h*4
			mov cx, IntTableLength2/2   ; 0:70h*4 = BIOS_SEG:IntTable2
			p_sys_iH2:
				movsw					; es:[di+=2] = ds:[si+=2]
				stosw					; es:[di+=2] = BIOS_SEG
			loop p_sys_iH2

			 
		IF BUILD_BIOS_ADCVOL EQ 1
		
			;call rtc_cfg_isOK
			;jc p_sys_adc_skip

				mov ax, 0 
				mov ds, ax
				M_defIRQ VOLADC_IRQ, p_adcVol_irq

			;p_sys_adc_skip:

		ENDIF
	ENDIF
	
	; ativar NMIs 
	mov al, 80h
	out NMI_REG, al
ret
 
;------------------------------------------------------------
; 2.6. p_sys_initKeyb
;------------------------------------------------------------
IF BUILD_BIOS_KEYB EQ 1
p_sys_initKeyb:

	call tty_crlf
	call ds2cs
	mov si, offset str_keyb
	call tty_str

	cli
	
	mov bx, 0FFFFh
	M_DispDigits16 bx, ax
	call p_kbreset
	xor bh, bh
	M_DispDigits16 bx, ax
	cmp bl, 0AAh
	je p_sys_keybok
		call ds2cs
		mov si, offset str_notdetected
		call tty_str
		jmp p_sys_keybcont
	p_sys_keybok:
		call tty_ok
	p_sys_keybcont:

	call tty_crlf
	
	cli
	M_IRQ_OFF KEYB_IRQ
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
ret
ENDIF
;------------------------------------------------------------
; 2.7. p_sys_equipDefaults
;------------------------------------------------------------
p_sys_equipDefaults:

	assume es:DATA
	call es2data
	call ds2cs
	
							; configurar defaults
							; porta serie e paralela
							
	mov di, offset DATA:print_tim_out
	mov ax, (LPT_DEF_TIMEOUT)+LPT_DEF_TIMEOUT*256
	stosw
	stosw
	mov ax, (COM_DEF_TIMEOUT)+COM_DEF_TIMEOUT*256
	stosw
	stosw
	
	mov di, offset DATA:rs232_base
	mov ax, COM1_IO
	stosw
	xor ax, ax
	stosw
	stosw
	stosw
	
	mov di, offset DATA:printer_base
	mov ax, LPT1_IO
	stosw
	xor ax, ax
	stosw
	stosw
	stosw
								; indicar dipswitches
	call getDipState
	mov byte ptr es:[offset DATA:dip_switches], al
	call tty_crlf
	mov si, offset str_dips
	call tty_str
	call tty_bin8
	call tty_sp
								; colocar defaults
		mov ax, \
			(0 SHL EQUIP_BITFLAG_IPLDISK)+			\
			(0 SHL EQUIP_BITFLAG_NPU)+				\
			(3 SHL EQUIP_BITFLAG_BASERAM)+			\
			(2 SHL EQUIP_BITFLAG_VIDEOMODE)+		\
			(0 SHL EQUIP_BITFLAG_DISKDRIVES)+		\
			(0 SHL EQUIP_BITFLAG_DMA)+				\
			(1 SHL EQUIP_BITFLAG_SERIALPORTS)+		\
			(1 SHL EQUIP_BITFLAG_JOYSTICKPORT)+		\
			(1 SHL EQUIP_BITFLAG_PLELPORTS)
			 
		mov word ptr es:[offset DATA:equip_flag0], ax
	
	mov si, offset str_equip
	call tty_str
	call tty_bin16
	
	; ... deixar o do grupo de botoes BAIXO
	M_PPI_showDip0
	assume es:CODE
ret

;------------------------------------------------------------
; 2.8.1. p_sys_portsConfig
;------------------------------------------------------------
IF BUILD_BIOS_PORTS EQ 1
									; coloca o endereço I/O
									; na BDA es:[di]
									; segundo o que está no RTC
p_sys_portsConfig_set:

	M_RTC_Read al					; ler enumeração
	;call tty_Hex8
	;call tty_sp
	xor bh, bh
	mov bl, al
	shl bl, 1
	mov dx, cs:[si+bx]				; indexar porta IO
	cmp dx, -1
	je p_sys_portsConfig_skip
		mov es:[di], dx				; colocar porta IO
		inc cx
		add di, 2
	p_sys_portsConfig_skip:
ret

;------------------------------------------------------------
; 2.8. p_sys_portsConfig
;------------------------------------------------------------
p_sys_portsConfig:

	call es2data
	call ds2cs
	call tty_crlf
	mov si, offset str_ports
	call tty_str

								; com rtc redefinir portas
	IF BUILD_BIOS_RTC EQ 1

		call rtc_cfg_isOK
		jc p_sys_portsdet_skip

		;----------------------------------------------------
		xor cx, cx	; cx nº de portas série
		
			mov al, RTC_RAM_COM1_IO
			mov si, offset COM_IO_PORTS
			mov di, offset DATA:rs232_base
			call p_sys_portsConfig_set

			mov al, RTC_RAM_COM2_IO
			call p_sys_portsConfig_set

		; colocar o numero de portas series em equiflags
		
			mov ax, word ptr es:[offset DATA:equip_flag0]
			and ax, NOT (111b SHL EQUIP_BITFLAG_SERIALPORTS)	; apagar
			shl cx, EQUIP_BITFLAG_SERIALPORTS
			or ax, cx 
			mov word ptr es:[offset DATA:equip_flag0], ax

		;----------------------------------------------------
		xor cx, cx	; cx nº de portas paralelas
		
			mov al, RTC_RAM_LPT1_IO
			mov si, offset LPT_IO_PORTS
			mov di, offset DATA:printer_base
			call p_sys_portsConfig_set
			
		; colocar o numero de portas series em equiflags
		
			mov ax, word ptr es:[offset DATA:equip_flag0]
			and ax, NOT (11b SHL EQUIP_BITFLAG_PLELPORTS)	; apagar
			shl cx, EQUIP_BITFLAG_PLELPORTS
			or ax, cx 
			mov word ptr es:[offset DATA:equip_flag0], ax
		;----------------------------------------------------

		p_sys_portsdet_skip:
	ENDIF
	
		mov bp, 0
		mov si, offset str_com
		mov di, offset DATA:rs232_base
		call p_sys_prn_and_cfg_ports
		
		mov bp, 1
		mov si, offset str_lpt
		mov di, offset DATA:printer_base
		call p_sys_prn_and_cfg_ports

		call ds2cs
		call es2data
		mov si, offset str_equip
		call tty_str
		mov ax, word ptr es:[offset DATA:equip_flag0]
		call tty_bin16


ret
ENDIF
;------------------------------------------------------------
; 2.9. p_sys_detectDrives
;------------------------------------------------------------
IF (BUILD_BIOS_HDD+BUILD_BIOS_FDD) NE 0
p_sys_detectDrives:
	call ds2cs
	call tty_crlf
	mov si, offset str_drives
	call tty_str
	IF BUILD_BIOS_HDD EQ 1
		call p_hdd_detect
	ENDIF
	IF BUILD_BIOS_FDD EQ 1
		call p_fdd_detect 
	ENDIF
	call ds2data
	assume ds:DATA
		mov bl, fdd_number
		mov bh, hdd_number
	assume ds:CODE
	call ds2cs
	call tty_crlf 
	M_tty_wrStr " ...total "
	mov si, offset str_fdd
	call tty_str
	mov al, ':'
	call tty_char
	mov al, bl
	call tty_U8
	call tty_sp
	mov si, offset str_hdd
	call tty_str
	mov al, ':'
	call tty_char
	mov al, bh
	call tty_U8
	call tty_sp
	assume ds:DATA
		call ds2data
		mov al, drives_map
		call tty_bin8
	assume ds:CODE
ret
ENDIF

;------------------------------------------------------------
; 2.10. p_sys_scanChecksumCnt
;------------------------------------------------------------
IF BUILD_BIOS_SCANEXP EQ 1
p_sys_scanChecksumCnt:
	xor al, al
	p_sys_scanCCloop:
		add al, ds:[bx]
		inc bx
	loop p_sys_scanCCloop
	or al, al
	ret
ENDIF

;------------------------------------------------------------
; 2.11. p_sys_scanChecksum
;------------------------------------------------------------
IF BUILD_BIOS_SCANEXP EQ 1
p_sys_scanChecksum:
	mov ax, DATA
	mov es, ax
	xor ah, ah
	mov al, ds:[bx+2]	; 2º W16 tamanho/512
	mov cl, 09h
	shl ax, cl			; ax=tamanho
	mov cx, ax			
	push cx
		mov cx, 4 
		shr ax, cl		; ax=nº de segmentos
		add dx, ax		; dx=próximo segmento
	pop cx
	call p_sys_scanChecksumCnt
	jz p_sys_scanCC_exec		; OK executar
	jmp p_sys_scanCCExit
p_sys_scanCC_exec:
	push dx
	assume es:DATA
		mov es:[io_rom_init], 3h
		mov es:[io_rom_seg], ds
		call dword ptr es:io_rom_seg
	assume es:CODE
	pop dx
p_sys_scanCCExit:
	ret
p_sys_scanExpansion:
	call ds2cs
	mov si, offset str_exp
	call tty_line
	call tty_saveCur
	mov dx, 0C800h
p_sys_scan1:
	push ax
		call tty_restoreCur
		mov ax, dx
		call tty_Hex16
	pop ax
	mov ds, dx
	xor bx, bx
	mov ax, ds:[bx]		; ax=ds:[bx]	1º W16 código
	push bx
	pop bx
	cmp ax, 0AA55h
	jnz p_sys_scanNext
	call p_sys_scanChecksum
	jmp p_sys_scanIsEnd
p_sys_scanNext:
	add dx, 080h		; próximos 2KB
p_sys_scanIsEnd:
	cmp dx, 0F600h
	jl p_sys_scan1
	
	call tty_restoreCur
	call tty_cleanLineFromCur
	call tty_OK
ret
ENDIF

;------------------------------------------------------------
; 2.12. p_sys_initTimer
;------------------------------------------------------------
IF BUILD_BIOS_IRQ0 EQ 1
p_sys_initTimer: 
	call ds2cs
	call tty_crlf
	mov si, offset str_timer
	call tty_str
	call tty_saveCur
	
	call ds2data
	ASSUME ds:DATA

	mov clock_test, 6
	cli					; desativar ints
						; configurar PIT0

	MOV	AL, 36h; SEL TIM 0, LSB/MSB, MODE 0, BINARY
	OUT	PIT_REG_CTRL, al
	mov al, 0
	out PIT_REG_COUNT0, al
	out PIT_REG_COUNT0, al

	M_IRQ_ON TIMER_IRQ	; ativar IRQ0
	sti					; ativar ints

	p_sys_initTimerTest:		; esperar por clock_test
		mov al, clock_test		; ficar a ZERO
		or al, al
	jnz p_sys_initTimerTest

	ASSUME ds:CODE
	cli					; desligar ints
	M_IRQ_OFF TIMER_IRQ ; desativar IRQ0

						; colocar novo tratador dummy
	mov ax, 0
	mov ds, ax
	mov ax, offset p_dummy_int
	mov ds:[1Ch*4], ax

	M_IRQ_ON TIMER_IRQ	; ativar IRQ0
	sti					; ativar ints
	
	call tty_restoreCur
	call tty_cleanLineFromCur
	call tty_ok
ret
ENDIF

;------------------------------------------------------------
; 2.13. p_sys_installSingleStep
;------------------------------------------------------------
IF BUILD_SSTEP EQ 1
p_sys_installSingleStep:
	push ax
	push cx
	push ds
	push es
	push si
	push di

		call ds2cs
		mov ax, DATA
		mov es, ax
		mov di, offset SINGLE_STEP_CODEIP
		mov ax, BIOS_SEG
		mov ds, ax
		mov si, offset p_singleStep_intCode
		mov cx, (p_singleStep_intCodeLength+1)/2
		rep movsw					; es:[di]=ds:[si]
		 
		mov ax, 0			
		mov ds, ax
		mov ax, offset DATA:SINGLE_STEP_CODEIP
		mov word ptr ds:[01h*4], ax
		mov ax, DATA  
		mov word ptr ds:[01h*4+2], ax

		call checkSingleStepInstall
	pop di
	pop si
	pop es
	pop ds
	pop cx
	pop ax
ret
ENDIF

;====================================================================
;== 3. IMPRESSÃO ====================================================
;====================================================================

;------------------------------------------------------------
; 3.1. p_sys_prn_info
;------------------------------------------------------------
p_sys_prn_info:
	call ds2cs
	IF BUILD_BIOS_RTC EQ 1
		call rtc_romIsUpdated
		jc p_sys_prn_info1
			mov si, offset str_romupdated
			call tty_line
			call beep
		p_sys_prn_info1:
	ENDIF 

	mov si, offset str_title
	call tty_str
	mov si, offset str_rominfo
	call tty_line
	call p_sys_prn_dateTime
	mov ax, word ptr cs:[ROM_SIZE]
	call tty_bytesFormat
ret

;------------------------------------------------------------
; 3.2. p_sys_prn_dateTime
;------------------------------------------------------------
p_sys_prn_dateTime:
	mov ax, cs
	mov es, ax
	mov di, offset ROM_DATE
	call tty_prnDate
	call tty_sp
	mov di, offset ROM_TIME
	call tty_prnTime
	call tty_sp
ret

;------------------------------------------------------------
; 3.3. p_sys_prn_port
;------------------------------------------------------------
IF BUILD_BIOS_PORTS EQ 1
	; si: nome da porta (CS), di: (DS) endereços IO
	; bp=0 serie, bp=1 paralela
p_sys_prn_and_cfg_ports:

	mov cx, 1
	p_sys_prn_ports_loop:

		call ds2data		; porta IO
		mov dx, ds:[di]
		or dx, dx
		jz p_sys_prn_ports_skip

		call tty_sp
		mov al, '['
		call tty_char
		call ds2cs			; nome
		call tty_str
		mov al, cl			; numero
		call tty_U8time
		mov al, ':'
		call tty_char
		mov ax, dx			; porta
		call tty_Hex16
		mov al, 'h'
		call tty_char
		mov al, ']'
		call tty_char
		call tty_sp
							; configurar a porta serie ou
							; testar a porta paralela
		cmp bp, 0
		jne p_sys_prn_ports_isLPT

			push bp
				mov bp, 0					; confg serial mas NAO imprimir registos BP=0
				call p_sys_configSerial
			pop bp
			jmp p_sys_prn_ports_skip

		p_sys_prn_ports_isLPT:
			call p_sys_testParallel

		p_sys_prn_ports_skip:
			add di, 2
			inc cx
			cmp cx, 5
			jne p_sys_prn_ports_loop

ret
ENDIF

;------------------------------------------------------------
; 3.4. p_sys_prn_drive
;------------------------------------------------------------
p_sys_prn_drive:
	push ds
	push ax
		call ds2cs
		test dl, 80h
		jnz tty_drive_isDisk
			mov si, offset str_fdd
		jmp tty_drive_next
		tty_drive_isDisk:
			mov si, offset str_hdd
		tty_drive_next:
			call tty_str
			mov al, dl
			and al, 1
			call tty_bit
	pop ax
	pop ds
	ret
	
;------------------------------------------------------------
; 3.5. p_sys_prn_booterror
;------------------------------------------------------------
p_sys_prn_booterror:
	call ds2cs
	push si
		call tty_crlf
		call tty_sp
		mov si, offset str_atention
		call tty_str
	pop si
	call tty_str
ret
