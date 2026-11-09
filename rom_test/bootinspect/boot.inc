mov ax, 0B800h
	mov es, ax
	mov al, '>'
	mov es:[0], al

	; transferir todo este código para
	; para PROGRAM_SEG
	mov ah, 02 ; ler
	mov al, (codeLength+512)/512  ; número de setores
	mov cx, 1 ; cil=0, setor=1 
	mov dx, 0 ; cab=0, disquete
			  ; es:bx = PROGRAM_SEG:0	
	mov bx, PROGRAM_SEG	
	mov es, bx
	xor bx, bx
	int 13h
	
	mov ax, 0B800h
	mov es, ax
	mov al, 'o'
	mov es:[0], al
	mov al, 'k'
	mov es:[2], al

	; saltar para o código de cópia
	db 0EAh
	dw offset copyCode
	dw PROGRAM_SEG
	
org 510
	dw 055AAh

copyCode:			; cs:ip = PROGRAM_SEG : copyCode
	mov ax, 0B800h
	mov es, ax
	mov al, 'k'
	mov es:[0], al
	mov al, 'o'
	mov es:[2], al
	
	mov ax, STACK_SEG	; definir STACK
	mov ss, ax
	mov sp, 256

	push ds
		call ds2data
		mov al, 0
		mov byte ptr ds:[offset SINGLE_STEP_ON], al
		xor ax, ax			; zerar o contador de INT13
		mov word ptr ds:[offset int13s], ax
		call tty_init
		call ds2cs
		M_tty_wrStr "Arrancando"
		call tty_crlf
		call tty_saveCur
	pop ds

	IF DEBUG_BIOS_PARAM EQ 1
		call testParams
	ENDIF

	; copiar ponteiro do tratador da INT13 da BIOS
	mov bx, PROGRAM_SEG
	mov es, bx
	mov ax, 0
	mov ds, ax
					; DS = 0 (para vetor de ints)
					; ES = PROGRAM_SEG

	; copiar ptr tratador INT13h
		mov di, offset int13BIOSHandlerPtr
		mov word ptr ax, ds:[13h*4]
		mov word ptr es:[di], ax		
		mov word ptr ax, ds:[13h*4+2]
		mov word ptr es:[di+2], ax		
	; trocar tratador da INT15h (serv)
		M_installINT 15h, int15Handler
	; trocar tratador da INT11h (equip)
		M_installINT 11h, int11Handler
	; trocar tratador da INT12h (size det)
		M_installINT 12h, int12Handler
	; instalar tratador encadeado
		M_installINT 13h, int13Handler
	
	
	;						
	pushf
	pop ax
	or ax, 100h
	push ax
	popf

	; transferir MBR para 7C00h 
	; se no arranque da BIOS
	mov ah, 02   ; ler
	mov al, 1    ; número de setores
	mov cx, 1    ; cil=0, setor 1
	mov dl, 80h  ; disco
	mov dh, 0	 ; cab=0
	xor bx, bx	 ; es:bx=0:7C00
	mov es, bx
	mov bx, 7C00h
	int 13h

	; saltar para o arranque MBR
	db 0EAh
	dw 7C00h 
	dw 0

	; variaveis (em CS)
	dos_crash			db 0
	int13s				dw 0
	SINGLE_INSTCOUNT	DW 0
	SINGLE_STEP_AX		DW 0
	SINGLE_STEP_BX      DW 0
	SINGLE_STEP_SS      DW 0
	SINGLE_STEP_SP      DW 0
	SINGLE_STEP_BP      DW 0
	SINGLE_STEP_DS      DW 0
	SINGLE_STEP_SI      DW 0
	SINGLE_STEP_IP      DW 0
	SINGLE_STEP_CS      DW 0
	SINGLE_STEP_NEXTIP  DW 0
	SINGLE_STEP_NEXTCS  DW 0
	SINGLE_STEP_NEXTOP  DB 0
	SINGLE_STEP_FLAGS   DW 0
	SINGLE_STEP_ON		DB 0

	

INCLUDE TTY.INC
INCLUDE UTIL.INC 
cga_clear:
ret
cga_setvideo:
ret
instalSSHandlers:
push ax
push ds
	; instalar INT1, INT3
	mov ax, 0
	mov ds, ax
	M_installINT 1h, singleStep
	M_installINT 3h, breakPoint
pop ds
pop ax
ret

int15Handler:
	stc
	mov ah, 86h			; XT 
	M_retFarWithout 2

int11Handler:
	M_enterInt 11h
	mov ah, 010000b	; joystick resto zero
	mov al, 111000b
iret 
int12Handler:
	M_enterInt 12h
	mov ax, 256
iret
prnOpcodes:
push cx
	call tty_crlf
	call tty_sp
	mov cx, 16
	sub si, 8
	p_ss_opcodeprn:
		cmp cx, 8
		jne p_ss_opcodeprn_nxt
			call tty_crlf
			mov al, '>'
			call tty_char
		p_ss_opcodeprn_nxt:
		lodsb
		call tty_Hex8
		call tty_sp
	loop p_ss_opcodeprn
pop cx
ret


cleanRegs:
	xor ax, ax
	xor bx, bx
	xor cx, cx
	xor dx, dx
	mov es, dx
	mov ds, dx
	mov di, dx
	mov si, dx
ret

debugBDA:
	xor ax, BIOSDATA
	mov es, ax	
	mov ax, cs
	mov ds, ax

	M_tty_wrESFieldStr rs232_base    , 4, DW
	M_tty_wrESFieldStr printer_base  , 4, DW
	M_tty_wrESFieldStr equip_flag0   
	M_tty_wrESFieldStr equip_flag1   	
	M_tty_wrESFieldStr mfg_tst       	
	M_tty_wrESFieldStr memory_size   , 1, DW	
	M_tty_wrESFieldStr mfg_err_flag  
	M_tty_wrESFieldStr error_codes   
	M_tty_wrESFieldStr kb_flag 	  
	M_tty_wrESFieldStr kb_flag_1	  
	M_tty_wrESFieldStr alt_input
	M_tty_wrESFieldStr buffer_head	 , 1, DW
	M_tty_wrESFieldStr buffer_tail	 , 1, DW
	M_tty_wrESFieldStr kb_buffer	 , 16, DW
	M_tty_wrESFieldStr seek_status	  
	M_tty_wrESFieldStr motor_status  
	M_tty_wrESFieldStr motor_count   
	M_waitKey 
	M_tty_wrESFieldStr disq_status   
	M_tty_wrESFieldStr nec_status    , 7
	M_tty_wrESFieldStr crt_mode	  
	M_tty_wrESFieldStr crt_cols	     ,1, DW
	M_tty_wrESFieldStr crt_len 		 ,1, DW
	M_tty_wrESFieldStr crt_start	 ,1, DW
	M_tty_wrESFieldStr cursor_posn	 ,8, DW
	M_tty_wrESFieldStr cursor_mode	 ,8, DW
	M_tty_wrESFieldStr active_page	  
	M_tty_wrESFieldStr addr_6845	 ,1, DW
	M_tty_wrESFieldStr crt_mode_set  
	M_tty_wrESFieldStr crt_palette	 
	M_waitKey  
	M_tty_wrESFieldStr io_rom_init	 ,1, DW
	M_tty_wrESFieldStr io_rom_seg	 ,1, DW
	M_tty_wrESFieldStr intr_flag	  
	M_tty_wrESFieldStr time_lo		 ,1, DW
	M_tty_wrESFieldStr time_hi		 ,1, DW
	M_tty_wrESFieldStr time_ovf	  
	M_tty_wrESFieldStr bios_break	  
	M_tty_wrESFieldStr reset_flag	 ,1, DW
	M_tty_wrESFieldStr hdd_op0   
	M_tty_wrESFieldStr hdd_number
	M_tty_wrESFieldStr hdd_op1   
	M_tty_wrESFieldStr hdd_op2   
	M_tty_wrESFieldStr print_tim_out ,4
	M_tty_wrESFieldStr rs232_tim_out ,4
	M_tty_wrESFieldStr buffer_start  ,1, DW
	M_tty_wrESFieldStr buffer_end	 ,1, DW
	M_waitKey  
ret

testParams:
	pushf
		call tty_crlf
		call ds2cs
		M_tty_wrFieldStr "Params"
		call tty_crlf
		call tty_regs
	popf
	
	call debugBDA

	; get params disquete
	call cleanRegs
	mov ah, 8
	mov dl, 0
	INT 13h
	call tty_regs
	M_waitKey

	; get params disco
	call cleanRegs
	mov ah, 8
	mov dl, 80h
	INT 13h
	call tty_regs
	M_waitKey

	; DASD
	call cleanRegs
	mov ah, 15h
	mov dl, 0
	INT 13h
	call tty_regs
	M_waitEnterKey

	
	mov ax, 0
	mov ds, ax
	mov di, ds:[41h*4]
	mov es, ds:[41h*4+2]
	call ds2cs
	M_tty_wrFieldStr "cils"
	mov ax, es:[di]
	add di, 2
	call tty_hex16
	M_tty_wrFieldStr "heads"
	mov al, es:[di]
	add di, 1
	call tty_hex8
	M_tty_wrFieldStr "wccil"
	mov ax, es:[di]
	add di, 2
	call tty_hex16
	M_tty_wrFieldStr "wpcil"
	mov ax, es:[di]
	add di, 2
	call tty_hex16
	M_tty_wrFieldStr "maxEccBurst"
	mov al, es:[di]
	add di, 1
	call tty_hex8
	M_tty_wrFieldStr "ctrlbyte"
	mov al, es:[di]

	call tty_hex8
	M_waitEnterKey


	
ret