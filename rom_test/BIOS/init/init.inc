TEST_DATALINES		  EQU 0
TEST_ADDRLINES		  EQU 0
TEST_LINES_NUM		  EQU 0FFFFh
TEST_LINES_NUM_HI	  EQU 4
DEBUG_XSUM			  EQU 1
DELAY_DIGITS_INIT     EQU 01FFFh
DELAY_DIGITS          EQU 02FFFh

M_DispDigits 8,0,8,8
mov dx, LEDS_REG
mov ax, 0FFh
out dx, al
M_Countdown cx, DELAY_DIGITS_INIT
mov ax, 0h
out dx, al
; colocar digitos
M_DispDigits 3, 2, 1, 0
M_Countdown cx, DELAY_DIGITS 
M_DispDigits 7, 6, 5, 4
M_Countdown cx, DELAY_DIGITS
M_DispDigits 0Bh, 0Ah, 9, 8
M_Countdown cx, DELAY_DIGITS
M_DispDigits 0Fh, 0Eh, 0Dh, 0Ch
M_Countdown cx, DELAY_DIGITS
M_Countdown cx, DELAY_DIGITS
M_Countdown cx, DELAY_DIGITS
 
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
IF TEST_DATALINES  EQ 1
wait
M_beep 0FFFFh
M_Countdown cx, 0FFFFh
mov si, 0
testData1:
	mov ax, si
	M_fakeCall U8toBCD
	mov bx, ax
	mov bh, bl
	shr bh, 4
	and bl, 0Fh
	M_dispDigits 0,0Dh,bh,bl	; mostrar no
								; visor DADOS
	mov al, 1
	mov cx, si
	shl al, cl

	mov dx, TEST_LINES_NUM_HI
	testData1L1:
		mov cx, TEST_LINES_NUM
		testData1L1_:
			out 00h, al
			not al
			out 00h, al
			not al
		loop testData1L1_
	dec dx
	jnz testData1L1
	;
	M_beep 02FFFh
	wait
	M_Countdown cx, 0FFFFh
	;
	inc si
	cmp si, 8
	jne testData1

	M_beep 0FFFFh
	M_Countdown cx, 0FFFh

ENDIF
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
IF TEST_ADDRLINES EQ 1
mov di, 1
xor ax, ax
mov es, ax
testAddr1:				; A0:A15
	
	mov cx, ax
		M_fakeCall U8toBCD
		mov bx, ax
		mov bh, bl
		shr bh, 4
		and bl, 0Fh
		M_dispDigits 0,0Ah,bh,bl
	mov ax, cx

	mov dx, TEST_LINES_NUM_HI
	testAddrL1:
		mov cx, TEST_LINES_NUM
		testAddrL1_:
			mov es:[di], al
		loop testAddrL1_
	dec dx
	jnz testAddrL1
		
	;
	mov bx, ax
	M_beep 02FFFh
	mov ax, bx
	;
	wait
	M_Countdown cx, 0FFFFh
	inc al
	shl di, 1
	or di, di
	jz testAddr1

mov di, 1000h
mov es, di
testAddr3:				; A16:A19
	mov cx, ax
		M_fakeCall U8toBCD
		mov bx, ax
		M_dispDigits 0,0Ah,bh,bl
	mov ax, cx

	mov dx, TEST_LINES_NUM_HI
	testAddr3L1:
		mov cx, TEST_LINES_NUM
		testAddr3L1_:
			mov es:[0], al
		loop testAddr3L1_
	dec dx
	jnz testAddr3L1
	;
	mov bx, ax
	M_beep 02FFFh
	mov ax, bx
	;
	wait
	M_Countdown cx, 0FFFFh
	inc al
	shl di, 1
	or di, di
	mov es, di
	jnz testAddr3	
	M_beep 0FFFFh
	M_Countdown cx, 0FFFh
ENDIF


 M_CHECKPOINT 1
 
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
IF BUILD_BIOS_CPUTEST EQ 1 
	MOV	AH,0D5H 		; SET SF, CF, ZF, AND AF FLAGS ON
	SAHF
	JNC	flagError
	JNZ	flagError
	JNP	flagError
	JNS	flagError
	LAHF		
	MOV	CL,5	
	SHR	AH,CL	
	JNC	flagError
	MOV	AL,40H	
	SHL	AL,1	
	JNO	flagError
	XOR	AH,AH	
	SAHF		
	JBE	flagError
	JS	flagError
	JP	flagError
	LAHF		
	MOV	CL,5	
	SHR	AH,CL	
	JC	flagError
	SHL	AH,1	
	JO	flagError
	jmp flagOK
	flagError:
		M_dispERROR ERROR_CPU
	jmp flagError
	flagOK:
ENDIF

M_CHECKPOINT 2

;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
; CHECKSUM
	cld
	mov bx, word ptr cs:[offset ROM_SIZE]	; a ROM está alinhada
	shr bx, 1								; a 16 bytes logo 
	sub bx, 2								; divisivel por 2 bytes;
	xor ax, ax								; subtrair 1 word
	xloop:									; para começar do fim
		xor ax, word ptr cs:[bx]			; no loop, ir xorando
		sub bx, 2							; até ao inicio
	jnz xloop								; onde o XOR com 
	mov cx, word ptr cs:[bx]				; 1º word da ROM
	xor ax, cx								; deve dar 0
	jnz xloopOk
		jmp xloopNotOk

	xloopOk:
		call xloopContinue

	xloopNotOk:
		mov si, word ptr cs:[offset ROM_XSUM]
		mov di, ax
		M_DispDigits16 si, cx
		M_DispLed 0
		M_Countdown cx, 0FFFFh
		M_DispDigits16 di, cx
		M_DispLed 1
		xloopNotOk_:
			M_Countdown dx, 0FFFFh
			M_dispERROR ERROR_XSUM
		jmp xloopNotOk_
	;-----------------------------------
	;-----------------------------------
	;-----------------------------------
	xloopContinue:
 
	xor ax, ax
	SAHF
	M_dispDigits16 ax, ax
	
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
;----------------------------------------------------------
M_rotateLeds 1, 03FFFh
M_Countdown dx, 0FFh
M_beep
jmp control_jmp			; ao compilar
	db "CONTROL"		; usado para saber este endereço
control_jmp:
