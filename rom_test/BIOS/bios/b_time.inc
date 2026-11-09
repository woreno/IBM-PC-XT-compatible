; Interrupção Hardware Temporizador (PIT)
;	o pit foi configurado a 18.2064819 interrupções
;	por segundo (ticks a cada 55ms); 
;  - mantem a noção do dia:
;        <time_hi> e <time_lo> sao incrementam a cada tick
;        <time_ovf> é colocado quando passa um dia
;        ~18.2 * 3600 = 65.543 (-7 que 65536)
;        em 24 horas se produz um erro de -7*24=-168
;  - desliga o motor da disquete, apos expiração
;    dum contador
;  - invoca um procedimento de utilizador
; ORG	(0FEA5h-08000h)
p_timer_int:
	sti
	pushf
	push ds
	push ax
	push dx

assume ds:DATA

	call ds2data
	
	inc time_lo
	jnz intTimer_notAnHour
	inc time_hi  ; incrementar hora

	intTimer_notAnHour:

	cmp time_hi, 24 ; fez um dia?
	jne intTimer_disq
					; se passou um dia e mais o 
					; acerto, overflow (no XT usam
					; outro valor pela precisão 
					; talvez)
	cmp time_lo, 176
	jne intTimer_disq

					; se fez um dia, zerar tudo
	xor ax, ax
	mov time_hi, ax
	mov time_lo, ax
	mov time_ovf, al

					; a cada interrupção destas...
	intTimer_disq:
	 	dec motor_count
	 	jnz intTimer_userProc   ; ...desligar os motores das unidades 
	 	;mov dx, FDC_DOR			; disquetes se rodar o seu contador
	 	;mov al, FDC_DOR_DMA_MOTOROFF
		;out dx, al 
	 intTimer_userProc:
	 	int 1Ch					; ...executar o procedimento
	 	mov al, PIC_CMD_EOI		; de utilizador registado no vetor 1Ch
	 	out PIC0_L0, al
	
	pop dx
	pop ax
	pop ds
	popf
	;M_SETINT_TRAP				; instalar trap se DIP
	M_retFarWithout 2
			
p_timer_io:
	M_ENTER_INT 1Ah, "TIMER"
	sti
	push ds
	call ds2data
	or ah, ah
	jz p_timer_io_2
	dec ah
	jz p_timer_io_3
p_timer_io_1:
	sti
	pop ds
	M_EXIT_INT
	iret
p_timer_io_2:
	cli
	mov al, time_ovf
	mov time_ovf, 0
	mov cx, time_hi
	mov dx, time_lo
	jmp p_timer_io_1
p_timer_io_3:
	cli
	mov time_lo, dx
	mov time_hi, cx
	mov time_ovf, 0
	jmp p_timer_io_1



assume ds:CODE




