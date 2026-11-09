IDE_CHUCKMODE		 EQU 0

include B_HDDX.INC

p_hdd_shortWait:
	;push cx
	;M_Countdown cx, 0FFh
	;pop cx
ret
;------------------------------------------------------------------------
p_hdd_start:
	; FLAGS					; +14
	; CS					; +12
	; IP					; +10
	; FLAGS					; +8
	pushf
	M_INT13_ENTER
	cli
	cld
	cmp ah, 15h				
	jne p_hdd_startCont
		test dl, 80h
		jnz p_hdd_DASD_disk
			mov ah, 2
		p_hdd_DASD_disk:
			popf
			clc
			M_retFarWithout 2

	p_hdd_startCont:
	cmp ah, 0Ah						; falhar se nao está
	jle p_hdd_start1				; no intervalo 
	cmp ah, 41h						; intervalo válido
	jl p_hdd_invalidFunction		; [0h..9h] [41h..44h]
	cmp ah, 44h						; algumas nao implementadas
	ja p_hdd_invalidFunction	
	
	p_hdd_start1:
		push si
		push ax
			xchg al, ah			; al = id da função
			xor ah, ah
			shl ax, 1			; duplicar
			mov si, ax			; si = offset do jump
		pop ax
		cmp ah, 0Ah
		jl p_hdd_start2     ; se extendido
		and si, 0EFh		; '4' passou a '8' zerar

		p_hdd_start2:
			jmp word ptr cs:[offset p_hdd_funcs + si]

		p_hdd_invalidFunction:
				M_IDE_SetError ide_error_invalid_func
			popf
			stc
			mov ah, 1		; função inválida
			jmp p_hdd_exit
		p_hdd_exitOK:
			pop si			; se veio dum jump, tirar si
			pop ax			; ignorar flag
			xor ah, ah
			clc
		p_hdd_exit:
			assume DS:CODE
			M_retFarWithout 2 
;------------------------------------------------------------------------
;
p_hdd_read: 
	;cli							; desligar interrupções
	push ax
	push cx
	push dx
	push di
	push es

	M_IDE_LEDS_READ

	call p_hdd_fillRegs
	push ax
		M_IDE_wrReg		IDE_CMD, IDE_CMD_READ
	pop ax
	
	call p_hdd_shortWait
	
	xor ch, ch
	mov cl, al		; Nº de setores a ler
	mov di, bx		; di = BX
		
		p_hdd_read_loop:
			M_IDE_DISP_SECTOR
			push cx
				call p_hdd_shortWait
				mov cx, 256
				mov dx, IDE_DATA
					p_hdd_read512:
						;call p_hdd_shortWait
						call p_hdd_waitRegs
						jc p_hdd_read_regserror

						;call p_hdd_shortWait
						call p_hdd_waitDREQ
						jc p_hdd_read_regserror

						M_IDE_DATAREAD_TO_AX
						stosw
					loop p_hdd_read512

			p_hdd_read_regserror:
				pop cx
				jc p_hdd_read_error

			mov dx, IDE_STATUS
			in al, dx
			test al, 111b
			jz p_hdd_read_success
				
					M_IDE_SetError ide_error_r

				p_hdd_read_error:
					stc
					jmp p_hdd_read_exit

			p_hdd_read_success:

		loop p_hdd_read_loop 

		IF INT13_DRAWMEM EQ 1
			wait
			call drawMemory
		ENDIF
		IF INT13_PEEKBUFFER EQ 1	
			M_PPI_TestDip 3
			jz p_hdd_read_skipbuff
			call p_hdd_peek_buffers
			p_hdd_read_skipbuff:
		ENDIF
	
clc
p_hdd_read_exit:
M_IDE_LEDS_OFF
M_IDE_EXIT_WAIT
pop es
pop di
pop dx
pop cx
pop ax
jmp p_hdd_exitOK
	 

;------------------------------------------------------------------------
;
p_hdd_write:
	push ax 
	push cx 
	push dx 
	push di 
	push es 

	M_IDE_LEDS_WRITE

	
	call p_hdd_fillRegs
	push ax
		M_IDE_wrReg		IDE_CMD, IDE_CMD_WRITE
	pop ax
	
	
	xor ch, ch
	mov cl, al		; Nº de setores a ler
	mov di, bx		; di = BX
	p_hdd_write_loop:
			call p_hdd_shortWait
			M_IDE_DISP_SECTOR
			push cx
				mov cx, 256
				mov dx, IDE_DATA
				p_hdd_wr512:
					;call p_hdd_shortWait
					call p_hdd_waitRegs
					jc p_hdd_write_regserror
	
					;call p_hdd_shortWait
					call p_hdd_waitDREQ
					jc p_hdd_write_regserror
					
					;lodsw						
					mov ax, es:[di]				; ax = es:[di+=2]
					M_IDE_DATAWRITE_FROM_AX
					add di, 2
	
				loop p_hdd_wr512
	
			p_hdd_write_regserror:
				pop cx
				jc p_hdd_write_error
	
			mov dx, IDE_STATUS
			in al, dx
			test al, 111b
			jz p_hdd_write_success
				
				M_IDE_SetError ide_error_w
				
				p_hdd_write_error:
				stc
				jmp p_hdd_write_exit
	
			p_hdd_write_success:
		loop p_hdd_write_loop
		clc

p_hdd_write_exit:
M_IDE_LEDS_OFF
M_IDE_EXIT_WAIT
pop es
pop di
pop dx
pop cx
pop ax
jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_reset:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_status:
	pop si
	xor al, al	; AL=estado (diferente dos outros)
	popf
	M_retFarWithout 2
	
;------------------------------------------------------------------------
;
p_hdd_verify:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_format:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_formatflags:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_formattrack:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_params:
	;test dl, 80h
	;jnz p_hdd_paramsIsDisk
	;	push ax
	;		mov ax, BIOS_SEG
	;		mov es, ax
	;	pop ax
	;	mov cx, 4F12h
	;	mov dx, 0101h   ; unidades instaladas
	;	mov bl, 4		
	;	mov di, offset FDD_BASE
	;	jmp p_hdd_exitOK

	; sai:
	;	ES:DI=DBT
	;	ah=se ok
	;	bl=???
	;	cx=cilindros
	;	dl=nº unidades do mm. tipo
	;	dh=cabeças
p_hdd_paramsIsDisk:			
push ax						
push bx
push es
push di
	assume es:DATA
	mov ax, DATA			
	mov es, ax
	mov di, offset DATA:hdd_DBT0
	and dl, 7Fh		; tirar o bit alto
	mov ax, 20
	mul dl
	add di, ax		; es:di = localizacao DBT
	
	; nº de setores
	mov byte ptr cl, es:[di+BIOS_DBT_SECTPERCIL]
	and cl, 3Fh

	; nº total de cilindros-1
	mov byte ptr al, es:[di+BIOS_DBT2_CILS+1]
	mov byte ptr ch, es:[di+BIOS_DBT2_CILS]
	dec ch
	cmp ch, 0FFh
	jne p_hdd_params_cil_ne	; borrow
		dec al
	p_hdd_params_cil_ne:
	shl al, 6
	or cl, al

	; total cabeças-1
	mov byte ptr dh, es:[di+BIOS_DBT2_HEADS]
	dec dh	

	; nº de drives
	mov dl, es:hdd_number 
	;mov byte ptr dl, es:[offset DATA:hdd_number] ;<!>
	assume es:CODE

pop di
pop es
pop bx
pop ax
jmp p_hdd_exitOK

;------------------------------------------------------------------------
p_hdd_notimpl:
	M_IDE_SetError ide_error_notimpl
ret
;------------------------------------------------------------------------
;
p_hdd_init:
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_read_with_ECC:
	call p_hdd_notimpl
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_verifyExts:
	call p_hdd_notimpl
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_readExt:
	call p_hdd_notimpl
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_writeExt:
	call p_hdd_notimpl
	clc
	jmp p_hdd_exitOK
	
;------------------------------------------------------------------------
;
p_hdd_paramsExt:	
	call p_hdd_notimpl
	clc
	jmp p_hdd_exitOK
	


;------------------------------------------------------------------------
;
p_hdd_infoRead:					; lê 512B para es:[di]
push ax
push cx
push dx
push di
	mov cx, 256
	mov dx, IDE_DATA
		
	p_hdd_infoRead_:
		call p_hdd_waitregs
		jnc p_hdd_infoRead__
			stc
			jmp p_hdd_infoRead_exit
		p_hdd_infoRead__:
		M_IDE_DATAREAD_TO_AX
		stosw				; guardar em es:[di+=2]
	loop p_hdd_infoRead_
	clc

p_hdd_infoRead_exit:
pop di
pop dx
pop cx
pop ax
ret

;------------------------------------------------------------------------
;
p_hdd_waitDREQ:			; espera por registo livres
M_push4 ax,bx,cx,dx
	mov bx, 0FFh
	xor cx, cx
	mov dx, IDE_STATUS
	p_hdd_waitDREQ_:
		dec cx
		jnz p_hdd_waitDREQ_cont
		xor cx, cx
		dec bx
		jnz p_hdd_waitDREQ_cont

			M_IDE_SetError ide_error_dataready
			stc
			jmp p_hdd_waitDREQ_exit

		p_hdd_waitDREQ_cont:
		in al, dx
		test al, IDE_STATUS_DRQ
		jz p_hdd_waitDREQ_
clc
p_hdd_waitDREQ_exit:
M_pop4 ax,bx,cx,dx
ret
	
;------------------------------------------------------------------------
;
p_hdd_waitRegs:			; espera por registo estar livre
	M_push4 ax,bx,cx,dx
		mov dx, IDE_STATUS
		mov bx, 0FFh
		xor cx, cx
		p_hdd_wregs_loop:
			dec cx
			jnz p_hdd_wregs_cont
			xor cx, cx
			dec bx
			jnz p_hdd_wregs_cont
			
			M_IDE_SetError ide_error_busy
			stc
			jmp p_hdd_wregs_ok

		p_hdd_wregs_cont:
			in al, dx
			test al, IDE_STATUS_BSY
			jnz p_hdd_wregs_loop

			clc

	p_hdd_wregs_ok:
		M_pop4 ax,bx,cx,dx

	ret

	
;------------------------------------------------------------------------
;
p_hdd_fillDBT:		; CL tem o numero do disco 0 ou 1
	push ax
	push cx
	push si
	push ds
		assume ds:DATA
		mov ax, DATA				; DS=DATA
		mov ds, ax

		xor ah, ah
		mov al, 20					; SI=offset da TABELA
		mul cl						;   =20*CL em DATA
		xor ah, ah
		mov si, ax
		add si, offset DATA:hdd_DBT0

		; instalar vetor para BIOS_DBT (a "segunda DBT")
		push ax
		push cx
		push di
		push si
		push es
			xor ax, ax		
			mov es, ax			; ES=0
			mov di, 41h*4
			test cl, 1
			jz p_hdd_fillDBT_notOne
				mov di, 46h*4
			p_hdd_fillDBT_notOne:

			add si, BIOS_DBT2_START		
			mov ax, si			; AX=SI+11
				
			mov es:[di], ax		; ES:DI=[ AX, CS ]
			mov ax, DATA
			mov es:[di+2], ax
		pop es
		pop si
		pop di
		pop cx
		pop ax
		
		push es			; apagar tabela
		push di
		push cx
			mov di, si
			push ds
			pop es
			mov cx, 10
			xor ax, ax
			rep stosw
		pop cx
		pop di
		pop es

			; preencher apenas o necessário
		mov al, 2		; 512bytes		
		mov ds:[si+BIOS_DBT_BYTESPERSECT], al
		mov word ptr ax, es:[IDE_INFOWRD_SECTPERCIL]
		mov ds:[si+BIOS_DBT_SECTPERCIL], al
		
		mov word ptr ax, es:[IDE_INFOWRD_CIL] 
		mov ds:[si+BIOS_DBT2_CILS], ax
		mov word ptr al, es:[IDE_INFOWRD_HEADS]
		;mov al, 20h  ; TESTE
		mov ds:[si+BIOS_DBT2_HEADS], al

		mov ax, 03FA0h 
		mov ds:[si+BIOS_DBT2_RWC_CIL], ax
		mov ax, 0
		mov ds:[si+BIOS_DBT2_WPRC_CIL], ax
		mov ds:[si+BIOS_DBT2_ECC_MAXBURST], al 
		mov ds:[si+BIOS_DBT2_CTRLBYTE], al
		
		push cx
			mov cx, 0
			mov word ptr ax, es:[IDE_INFOWRD_CIL]
			add cx, ax
			mov word ptr ax, es:[IDE_INFOWRD_HEADS]
			add cx, ax
			mov word ptr ax, es:[IDE_INFOWRD_SECTPERCIL]
			add cx, ax
		pop cx
		jnz p_hdd_fillDBT_cont 

			call ds2cs
			mov si, offset str_notdetected
			call tty_str 
			jmp p_hdd_fillDBT_exit

	p_hdd_fillDBT_cont:
	
	assume ds:DATA
		call ds2data
		inc hdd_number
		push cx
			inc cl
			shl cl, 2
			or drives_map, cl
		pop cx
	assume ds:CODE

		push dx
			mov cx, es:[IDE_INFOWRD_CIL]
			mov dx, es:[IDE_INFOWRD_HEADS]
			mov ax, es:[IDE_INFOWRD_SECTPERCIL]
			call tty_CHS_normal
			call ds2cs
			M_tty_wrFieldStr "lba?"
			mov word ptr ax, es:[IDE_INFODWRD_FEATURES]
			shr al, 9
			call tty_bit
		pop dx

	p_hdd_fillDBT_exit:
	pop ds
	pop si
	pop cx
	pop ax
ret

;------------------------------------------------------------------------
p_hdd_detectFail:
	call ds2cs
	call tty_crlf
	call tty_sp
	mov si, offset str_hdd
	call tty_str
	mov al, ':'
	call tty_char
	call tty_sp
	mov si, offset str_nocontroller
	call tty_str
ret

p_hdd_detect:
	call ds2data
	assume ds:DATA
		mov hdd_number, 0
	assume ds:CODE
	
	mov ax, DISK_TEST_SEGMENT
	mov es, ax

	call p_hdd_waitRegs
	jc p_hdd_detectFail

	call ds2cs
	mov cx, 0
	p_hdd_detect_loop:

		call p_hdd_waitRegs
		jc p_hdd_detect_no
		
		call tty_crlf
		call tty_sp
		mov si, offset str_hdd
		call tty_str
		mov al, cl
		call tty_bit
		mov al, ':'
		call tty_char

		mov al, 0A0h
		push cx
			shl cl, 4		; selecionar unidade Master/Slave
			add al, cl
		pop cx
		M_IDE_wrAlToReg IDE_DRIVEHEAD
		xor al, al
		M_IDE_wrAlToReg IDE_SECTCOUNT
		M_IDE_wrAlToReg IDE_SECTOR   
		M_IDE_wrAlToReg IDE_CIL_L
		M_IDE_wrAlToReg IDE_CIL_H
		M_IDE_wrReg		IDE_CMD, IDE_CMD_IDENTIFY 

		xor di, di
		call p_hdd_infoRead
		jc p_hdd_detect_no

			call p_hdd_fillDBT	; disco encontrado, preencher Disk Table

		jmp p_hdd_detect_ok
		p_hdd_detect_no:
		
			call tty_sp
			call ds2cs
			mov si, offset str_notdetected
			call tty_str

		p_hdd_detect_ok:
		inc cl
		cmp cl, 2
		jne p_hdd_detect_loop
		
	p_hdd_detect_exit:

ret


;------------------------------------------------------------------------
;
p_hdd_funcs:
	dw  offset p_hdd_reset			; 00
	dw  offset p_hdd_status			; 01
	dw  offset p_hdd_read			; 02
	dw  offset p_hdd_write			; 03
	dw  offset p_hdd_verify			; 04
	dw  offset p_hdd_format			; 05
	dw  offset p_hdd_formatflags	; 06
	dw  offset p_hdd_formattrack	; 07
	dw  offset p_hdd_params			; 08
	dw  offset p_hdd_init			; 09
	dw  offset p_hdd_read_with_ECC	; 0A
p_hdd_funcsExt:
	dw 0
	dw offset p_hdd_verifyExts		; 41
	dw offset p_hdd_readExt			; 42
	dw offset p_hdd_writeExt		; 43
	dw offset p_hdd_paramsExt		; 44 