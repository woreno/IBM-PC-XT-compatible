; - ESTE PROGRAMA É COMPILADO E PENSADO PARA
;   SER CÓDIGO DE ARRANQUE
; - NA EXECUÇÃO COPIA ALGUNS TRATADORES COMO OS
;   DA INT13H COM INTENÇÃO DE FAZER ENCADEAMENTO
;   E INSPECIONAR OS REGISTOS A CADA ACESSO
;   AO DISCO
; - LOGO QUE INSTALADO, FAZ ARRANCAR
;   O SISTEMA OPERATIVO COPIANDO O 1º SETOR 
;   E SALTANDO PARA A ZONA 7C00H
.8086
.model tiny
;---------------------------
PROGRAM_SEG		   EQU 2000h ; 128K
DATA_SEG		   EQU (PROGRAM_SEG-36) ; 128K->300 bytes 
SS_STACK_SEG	   EQU (PROGRAM_SEG-20) ; 128K-512 bytes
STACK_SEG		   EQU (PROGRAM_SEG-10) ; 128K-256 bytes
BIOS_SEG		   EQU PROGRAM_SEG	   ; equiv. 
TTY_MODE           EQU 1
TTY_ISCGA		   EQU 0
TTY_MAKE_ROLLSOUND EQU 0 
INT13_STEP_AT      EQU 0FFFFh;28
INT13_STEP_ON	   EQU 1
USE_SSTEP_SKIP_DOS EQU 0
DEBUG_BIOS_PARAM   EQU 1
 
; Área de dados da BIOS - aos 16KB
BIOSDATA SEGMENT AT 40H
	INCLUDE B_BDA.INC
BIOSDATA ENDS


INCLUDE MACROS.INC
INCLUDE OTHERS.INC		; outras macros
;---------------------------

code SEGMENT PUBLIC USE16 '_TEXT'
ASSUME cs:code, ds:code, es:code
org 0
Start:
	include BOOT.INC	; código de arranque, programa de cópia, etc

singleStepPrn:
	push ax
		call tty_restoreCur
		mov ax, 25
		call tty_clearLines
	pop ax
	push si 
	push ds  
		mov ax, cs:[SINGLE_STEP_CS]
		mov ds, ax
		mov si, cs:[SINGLE_STEP_IP]
		call prnOpcodes
	pop ds
	pop si

	inc cs:[SINGLE_INSTCOUNT]

	call tty_crlf
	M_tty_ChgAtt 047h
	M_tty_wrFieldSegReg 'C','S','I','P', cs:[SINGLE_STEP_CS], cs:[SINGLE_STEP_IP]
	M_tty_ChgAtt 007h
	M_tty_wrFieldReg ' ','F', cs:[SINGLE_STEP_FLAGS]
	M_tty_wrFieldReg 'S','S', cs:[SINGLE_STEP_SS]
	M_tty_wrFieldReg 'C','#', cs:[SINGLE_INSTCOUNT]

	call tty_crlf
	M_tty_wrFieldReg 'S','P', cs:[SINGLE_STEP_SP]
	M_tty_wrFieldReg 'B','P', cs:[SINGLE_STEP_BP]
	mov bp, cs:[SINGLE_STEP_BP]
	mov bx, cs:[SINGLE_STEP_BX]
	mov ax, cs:[SINGLE_STEP_AX]
	mov si, cs:[SINGLE_STEP_SI]
	mov ds, cs:[SINGLE_STEP_DS]
	call tty_regs

	push cx
	push si
	push ds
		mov cx, 8
		call tty_crlf
		mov si, cs:[SINGLE_STEP_SP]
		mov ds, cs:[SINGLE_STEP_SS]
		call tty_hex16arr
		call tty_crlf
		mov si, cs:[SINGLE_STEP_BP]
		mov ds, cs:[SINGLE_STEP_SS]
		call tty_hex16arr
	pop ds
	pop si
	pop cx
	call tty_crlf
	M_waitKey
ret

breakPoint:
	;FLAGS +4
	;CS    +2
	;IP    +0	; guardar registos
	mov cs:[SINGLE_STEP_AX], ax
	mov cs:[SINGLE_STEP_BX], bx
	mov cs:[SINGLE_STEP_DS], ds
	mov cs:[SINGLE_STEP_SI], si
	mov cs:[SINGLE_STEP_BP], bp
	mov cs:[SINGLE_STEP_SS], ss
	mov cs:[SINGLE_STEP_SP], sp
	mov bp, sp
	mov ax, ss:[bp+4]
	mov cs:[SINGLE_STEP_FLAGS], ax

				; restaurar opcode  (usa ax, ds, si)
	mov si, cs:[SINGLE_STEP_NEXTIP]
	mov ds, cs:[SINGLE_STEP_NEXTCS]
	mov al, cs:[SINGLE_STEP_NEXTOP]
	mov ds:[si], al
		
				; restaurar registos modificados
	mov ax, cs:[SINGLE_STEP_AX]
	mov ds, cs:[SINGLE_STEP_DS]
	mov si, cs:[SINGLE_STEP_SI]
	mov bp, cs:[SINGLE_STEP_BP]

	mov cs:[SINGLE_INSTCOUNT], 0

	jmp singleStepToPrn

singleStep:
	;FLAGS +4
	;CS    +2
	;IP    +0
	mov cs:[SINGLE_STEP_AX], ax
	mov cs:[SINGLE_STEP_BX], bx
	mov cs:[SINGLE_STEP_DS], ds
	mov cs:[SINGLE_STEP_SI], si
	mov cs:[SINGLE_STEP_BP], bp
	mov cs:[SINGLE_STEP_SS], ss
	mov cs:[SINGLE_STEP_SP], sp
	mov bp, sp
	mov ax, ss:[bp+4]
	mov cs:[SINGLE_STEP_FLAGS], ax
	
	; o próximo endereço for INT13 largar
	mov si, ss:[bp+0]
	mov ds, ss:[bp+2]
	mov ax, ds:[si]
	cmp al, 0CDh
	jne singleStepNotInt
	cmp ah, 013h
	je singleStepSkipTrap

singleStepNotInt:

	; se endereço CS:IP for o mesmo, passar à frente
	mov ax, ss:[bp+2]
	mov bx, cs:[SINGLE_STEP_CS]
	cmp ax, bx
	jne singleStepToPrn
	mov ax, ss:[bp+0]
	mov bx, cs:[SINGLE_STEP_IP]
	cmp ax, bx
	je singleStepSkip

singleStepToPrn:
	; mudar stack e imprimir
		mov ax, SS_STACK_SEG
		mov ss, ax
		mov sp, 256
		call singleStepPrn
	; restaurar stack
		mov ss, cs:[SINGLE_STEP_SS]
		mov sp, cs:[SINGLE_STEP_SP]
		
	mov bp, sp

	; passar em frente do CRASH
	IF USE_SSTEP_SKIP_DOS EQ 1
		mov al, cs:[dos_crash]			; se flag já escrita
		or al, al						; passar à frente
		jnz p_singleStepNotDosInit
			
		mov ax, ss:[bp+2]
		cmp ax, 070h				; se CS==70h 
		jne p_singleStepNotDosInit	
		mov ax, ss:[bp+0]
		cmp ax, 1965h				; IP seguinte é o do CRASH?
		jne p_singleStepNotDosInit
			mov cs:[dos_crash], 1		; escrever flag	
			mov ss:[bp], 1968h			; saltar em frente
		p_singleStepNotDosInit:
	ENDIF
	
singleStepSkip:

					; trap 
	mov ax, ss:[bp+4]
	or ax, 100h
	mov ss:[bp+4], ax
	
singleStepSkipTrap:

					; guardar ultimo ip
	mov ax, ss:[bp+0]
	mov cs:[SINGLE_STEP_IP], ax
	mov ax, ss:[bp+2]
	mov cs:[SINGLE_STEP_CS], ax

	; restaurar 
	mov ax, cs:[SINGLE_STEP_AX]
	mov bx, cs:[SINGLE_STEP_BX]
	mov ds, cs:[SINGLE_STEP_DS]
	mov si, cs:[SINGLE_STEP_SI]
	mov bp, cs:[SINGLE_STEP_BP]
iret


int13Handler:
    ; FLAGS ;+8
    ; CS    ;+6
	; IP    ;+4
	; este é o tratador encadeado
	push ax ;+2
	push bp ;+0
	
		mov bp, sp
							; guardar seguinte CS:IP
		mov ax, ss:[bp+4]
		mov cs:[SINGLE_STEP_NEXTIP], ax
		mov ax, ss:[bp+6]
		mov cs:[SINGLE_STEP_NEXTCS], ax

		M_enterInt 13h
		
							; aumentar e imprimir contador
							; de acessos INT13
		inc word ptr cs:[offset int13s]
		mov ax, word ptr cs:[offset int13s]
		call tty_sp
		call tty_U16
		call tty_crlf
									; imprimir opcodes
		push ds		 	
		push si			
			mov ds, ss:[bp+6]
			mov si, ss:[bp+4]		
			call prnOpcodes			; estraga AX, SI, DS
		pop si
		pop ds
									; imprimir registos 
		call tty_crlf
		call tty_crlf
		M_tty_ChgAtt 047h
		M_tty_wrFieldSegReg 'N','E','X','T', ss:[bp+6], ss:[bp+4]
		M_tty_ChgAtt 007h
		M_tty_wrFieldReg ' ','F',ss:[bp+8]
		M_tty_wrFieldReg 'D','S',ds
		call tty_crlf
		M_tty_wrFieldReg 'S','S',ss
		mov ax, sp
		sub ax, 4
		M_tty_wrFieldReg 'S','P',ax
	pop bp
		M_tty_wrFieldReg 'B','P',bp
	pop ax
									; imprimir outros registos 
	call tty_regs
	call tty_crlf
									; imprimir CHS
	pushf
		call p_hdd_prnCHSfromRegs
	popf

	IF INT13_STEP_ON EQ 1
		pushf
		push ax
		push ds
		push si
			mov ax, word ptr cs:[offset int13s]
			cmp ax, INT13_STEP_AT
			jb int13_skipSStep  
									; instalar tratadores INT1 e INT3
				call instalSSHandlers

									; guardar seguinte CS:IP
				mov si, cs:[SINGLE_STEP_NEXTIP]
				mov cs:[SINGLE_STEP_IP], si
				mov ax, cs:[SINGLE_STEP_NEXTCS]
				mov cs:[SINGLE_STEP_CS], ax
				

									; substituir por OPCODE de 1byte
									; para chamar INT3		
				mov al, ds:[si] 
				mov cs:[SINGLE_STEP_NEXTOP], al
				mov byte ptr ds:[si], 0CCh

			int13_skipSStep:
			M_waitEnterKey

		pop si
		pop ds
		pop ax
		popf
	ELSE
		pushf
		M_waitKey
		popf
	ENDIF

	; saltar para o tratador 
	db 0EAh
	int13BIOSHandlerPtr:
	dw 0,0

codeLength EQU $-Start
code ends
end Start
END 
