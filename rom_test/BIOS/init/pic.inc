
M_defIRQFunc MACRO IRQ_ 
	PIC_IRQ&IRQ_:
	cli
	mov ax, IRQ_
	jmp PIC_Action
ENDM


	; ICW1

IF BUILD_BIOS_PIC EQ 1
	; configuração única
	; linhas int: disparam na transição (edge-triggered)
	; vai escrever ICW4
	mov al, 013h        
	out PIC0_ICW1, al
ELSE
	; configuração dupla
	; linhas int: disparam na transição (edge-triggered)
	; vai escrever ICW4
	mov al, 011h      
	out PIC0_ICW1, al
	out PIC1_ICW1, al    
ENDIF

	; ICW2
		; base dos ponteiros para os tratadores,
		; no IBM PC é 0x8 (PIC0) e 0x70 (PIC1)
		; para o registo os 3 bits LSBs não interessam 
		; pelo que tem de ser mesmo multiplos de 8 
		mov al, 08h          
		out PIC0_ICW2, al
		IF BUILD_BIOS_PIC EQ 2
			mov al, 070h
			out PIC1_ICW2, al
		ENDIF

	; ICW3 (apenas se existe cascata)
		IF BUILD_BIOS_PIC EQ 2
			mov al, 04h         ; (bit) linha 2 está ligada a um slave
			out PIC0_ICW3, al
			mov al, 02h         ; linha 2 no master ligada 
			out PIC1_ICW3, al
		ENDIF

	; ICW4
		; operar no modo 8088/86
		; EOI manual
		; SP/EN distingue slave/master
		; não buffered
		; prioritário
		mov al, 01h
		out PIC0_ICW4, al
		IF BUILD_BIOS_PIC EQ 2
			out PIC1_ICW4, al
		ENDIF

; Inibir todas as linhas por enquanto
; OCW1
	mov al, 0FFh
	out PIC0_IMR, al
	IF BUILD_BIOS_PIC EQ 2
		out PIC1_IMR, al
	ENDIF

; salto neste código, onde se definem os tratadores de IRQ
jmp PIC_SkipFuncts


IF TEST_IRQ EQ 1
		M_defIRQFunc 0
		M_defIRQFunc 1
		M_defIRQFunc 2
		M_defIRQFunc 3
		M_defIRQFunc 4
		M_defIRQFunc 5
		M_defIRQFunc 6
		M_defIRQFunc 7
IF BUILD_BIOS_PIC EQ 2
		M_defIRQFunc 8
		M_defIRQFunc 9
		M_defIRQFunc 10
		M_defIRQFunc 11
		M_defIRQFunc 12
		M_defIRQFunc 13
		M_defIRQFunc 14
		M_defIRQFunc 15
ENDIF
	PIC_Action:
	push ax
		M_DispDigits16 ax, cx
		M_beep 0FFFh
	pop bx
	mov al, PIC_CMD_EOI
	cmp bl, 8
	jb PIC_ActionEnd2
		out PIC1_L0, al
	PIC_ActionEnd2:
		out PIC0_L0, al

	PIC_ActionEnd:
iret
ENDIF

PIC_SkipFuncts:

IF TEST_IRQ EQ 1
		; definir ponteiros para tratadores acima
		; todos em DS=0
		xor cx, cx
		mov ds, cx
		M_defIRQ  0, PIC_IRQ0
		M_defIRQ  1, PIC_IRQ1
		M_defIRQ  2, PIC_IRQ2
		M_defIRQ  3, PIC_IRQ3
		M_defIRQ  4, PIC_IRQ4
		M_defIRQ  5, PIC_IRQ5
		M_defIRQ  6, PIC_IRQ6
		M_defIRQ  7, PIC_IRQ7
	IF BUILD_BIOS_PIC EQ 2
		M_defIRQ  8, PIC_IRQ8
		M_defIRQ  9, PIC_IRQ9
		M_defIRQ 10, PIC_IRQ10
		M_defIRQ 11, PIC_IRQ11
		M_defIRQ 12, PIC_IRQ12
		M_defIRQ 13, PIC_IRQ13
		M_defIRQ 14, PIC_IRQ14
		M_defIRQ 15, PIC_IRQ15
	ENDIF
ENDIF


IF TEST_IRQ EQ 1
	; PIC: ativar todas linhas
	xor al, al
	out PIC0_IMR, al
	IF BUILD_BIOS_PIC EQ 2
		out PIC1_IMR, al
	ENDIF 



	IF TEST_IRQ_ADDRESS EQ 1
		wait
		; verificar se os endereços dos tratadores estao bem
		mov bx, 8*4
		xor cx, cx
		mov ds, cx 
		loop1:
			mov word ptr ax, ds:[bx+2]
			M_DispDigits16 ax, cx
			M_Countdown cx, 0FFFFh
			mov word ptr ax, ds:[bx]
			M_DispDigits16 ax, cx
			M_Countdown cx, 0FFFFh
			;M_RotateLeds 1
			add bx, 4
			cmp bx, 8*4 + 8*4
		jne loop1
	ENDIF
	

; testar 
	IF TEST_IRQ EQ 1
		; ligar interrupções
			sti
		PIC_loop:
			M_RotateLeds 0
		jmp PIC_loop
	ENDIF
ENDIF
