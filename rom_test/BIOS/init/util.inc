;-----------------------------------------------------------
; Feedback

getDipState:
	in  al, PPI_REG_OPTS  
	mov ah, al            
	; ... o estado do grupo de botoes BAIXO
	M_PPI_showDip0
	in al, PPI_REG_INFO 
	and al, 0Fh
	mov bl, al
	; ... o estado do grupo de botoes ALTO
	M_PPI_showDip1
	in al, PPI_REG_INFO
	shl al, 4
	or al, bl 
	not al			   ; inverter ON
ret

testButton:
	WAIT
	call longWait
	call makeShortBeep
ret

lightLeds:
	not al
	mov dx, LEDS_REG
	out dx, al
	not al
ret

blinkLeds:	
	push ax
	push dx
	M_BlinkeLeds
	pop dx
	pop ax
ret
rotateLedsRight:
	M_RotateLeds 1
ret
rotateLedsLeft:
	M_RotateLeds 0
ret

makeShortBeep:
	push ax
	push dx
	M_beep 01FFFh
	pop dx
	pop ax
ret 
beep:
	push ax
	push dx
	M_beep 02FFFh
	pop dx
	pop ax
ret
errorBeep:
	push ax
	push dx
	M_errorBeep
	pop dx
	pop ax
ret 

;-------------------------------------------------------------
microWait:
	push cx
	M_Countdown cx, 0FFFh
	pop  cx
ret 
smallWait:
	push cx
	M_Countdown cx, 03FFFh
	pop  cx
ret 
longWait:
	push cx
	M_Countdown cx
	pop  cx
ret 
longlongWait:
	push cx
	mov cx, 5h
	longlongWait_:
		call longWait
		loop longlongWait_
	pop cx
ret 

;-------------------------------------------------------------
BCDtoU8:
	push bx
	push ax
		xor ah, ah
		shr al, 4
		mov bl, 10
		mul bl 
		mov bl, al
	pop ax
		and al, 0Fh
		add al, bl
	pop bx
	ret
	
U8toBCD:
push bx
	mov bh, ah
	xor ah, ah
	mov bl, 10
	div bl
	shl al, 4
	add al, ah
	mov ah, bh
pop bx
ret


;----------------------------------
; ax = ax - 90
angleCorrect:
	sub ax, 90
	cmp ax, 0
	jl angleCorrectL
		ret
	angleCorrectL:
		add ax, 360
		ret
;-------------------------------------------------------------


; trocar 2 grupos de "2 bits" num byte
; AL = BYTE
; DL = posição 1
; DH = posição 2
swapTwoBits:	; dh<->dl
push bx
push cx
push dx
	mov bl, al			; copiar os bytes em para BL, BH
	mov bh, al
	
	mov cl, dl			; DL=posição 1
	shl cl, 1			
		shr bl, cl		; >>CL
		and bl, 11b		; BL=valor 1
		push dx			; apagar valor em AL
			mov dl, 11b		
			shl dl, cl	; DL=11b<<CL
			not dl	
			and al, dl	; AL&=~DL
		pop dx

	mov cl, dh			; DH=posição 2
	shl cl, 1
		shr bh, cl		;>>CL
		and bh, 11b		;BH=valor 2
		push dx
			mov dl, 11b
			shl dl, cl
			not dl
			and al, dl
		pop dx
	
	mov cl, dl			;; DL=posição 1
	shl cl, 1
	shl bh, cl
	or  al, bh

	mov cl, dh
	shl cl, 1
	shl bl, cl
	or  al, bl
	
pop dx
pop cx
pop bx
ret
;---------------------------
;; conta as letras da string passada em <ds:si>
;; cx = tamanho, terminada por 0
strlen:
push es
push ax
push di
	mov ax, ds
	mov es, ax
	xor ax, ax
	mov cx, 0FFFFh
	mov di, si
	cld
	repne scasb 
	mov ax, 0FFFFh
	sub ax, cx
	mov cx, ax
	dec cx
pop di
pop ax
pop es
ret
;-------------------------------------------------------------
strncmp:
M_push4 cx,ax,di,si	; repe repete a comparação
	cld				; por N; pelo que CX pode chegar 
					; a zero mas a flag da comparação
					; não ser "igual"
	repe cmpsb		
	jne strncmpFail	; <!> tentei fazer and as flags
	or cx, cx		;    CX=0 e do CMPSB mas nao deu 
	jnz strncmpFail ;    em vez de 2 jumps
	call tty_ok
	clc
	jmp strncmpExit
strncmpFail:
	call tty_ddot
	stc
strncmpExit:
	M_pop4 cx,ax,di,si
	ret
	
toUpperChar:
	cmp al, 'a'
	jl toUpperSkip
	cmp al, 'z'
	ja toUpperSkip
		and al, 0DFh
	toUpperSkip:
ret

toUpper:
push cx
push di
	toUpperLoop:	
	mov al, es:[di] 
	call toUpperChar
	mov es:[di], al
	loop toUpperLoop
pop di
pop cx
ret

emergency_:
	IF TTY_ISCGA EQ 1
	push ax
		mov ax, 0
		call cga_setVideo
	pop ax
	ENDIF
	call tty_clear
	call tty_regs
	hlt
	hlt_


;-------------------------------------------------
	; LBA = (c*tH + h)*tS + (s-1)
	; dx:ax tem LBA, BL=tS BH=tH 
p_chs_to_lba:
	push bp
	mov bp, sp
	push 0		
	DISK_STK_TS	EQU bp-2
	push 0		
	DISK_STK_TH	EQU bp-4
	push 0			
	DISK_STK_TC	EQU bp-6	; nao usado
	push 0			
	DISK_STK_S	EQU bp-8
	push 0			
	DISK_STK_H	EQU bp-10
	push 0			
	DISK_STK_C	EQU bp-12
	
	push ax
		xor ah, ah
		mov al, BL
		mov ss:[DISK_STK_TS], ax
		mov al, BH
		mov ss:[DISK_STK_TH], ax
		mov ax, cx
		xchg al, ah
		shr ah, 6
		mov ss:[DISK_STK_C], ax
		mov al, dh				; atenção DH não é zero
		xor ah, ah				; based como a bios
		mov ss:[DISK_STK_H], ax
		mov ax, cx
		xor ah, ah
		and al, 3Fh
		mov ss:[DISK_STK_S], ax
	pop ax

		; LBA = (c*tH + h)*tS + (s-1)
	push bx
		mov ax, ss:[DISK_STK_C]
		mov bx, ss:[DISK_STK_TH]
		mul BX		; c*tH
		add ax, ss:[DISK_STK_H]; somar cabeças
		mov BX,	ss:[DISK_STK_TS]; <!>
		mul BX		; * tS
		mov BX,	ss:[DISK_STK_S]; 
		dec BX
	push cx
		xor cx, cx
		add AX, BX
		adc DX, cx
	pop cx
	pop bx

	mov sp, bp
	pop bp
ret
	
;-------------------------------------------------
	;  DX:AX tem o lba, sai CX, DX com CHS
	;  BL=tS BH=tH SI=tC
p_lba_to_chs:
	push bp
	mov bp, sp
	push 0		
	;DISK_STK_TS	EQU bp-2
	push 0		
	;DISK_STK_TH	EQU bp-4
	push 0			
	;DISK_STK_TC	EQU bp-6
	push 0			
	;DISK_STK_S		EQU bp-8
	push 0			
	;DISK_STK_H		EQU bp-10
	push 0			
	;DISK_STK_C		EQU bp-12
	push 0			
	DISK_STK_LBA	EQU bp-14
	push 0			
	DISK_STK_LBAHI	EQU bp-16

	M_push3 ax, bx, dx

	; guardar totais
		mov ss:[DISK_STK_LBA],	 ax
		mov ss:[DISK_STK_LBAHI], dx
		mov ss:[DISK_STK_TC], SI
		xor ah, ah
		mov al, BL
		mov ss:[DISK_STK_TS], ax
		mov al, BH
		mov ss:[DISK_STK_TH], ax

	; C = LBA / (tH * tS)
		xor dx, dx
		mov ax, ss:[DISK_STK_TH]
		mov bx, ss:[DISK_STK_TS]
		mul bx
		mov bx, ax 
		mov ax, ss:[DISK_STK_LBA]
		mov dx, ss:[DISK_STK_LBAHI]
		div bx
		mov ss:[DISK_STK_C], ax  

	; H = (LBA / tS) % tH
		mov ax,  ss:[DISK_STK_LBA]
		mov dx,  ss:[DISK_STK_LBAHI]
		mov bx,  ss:[DISK_STK_TS]
		div bx
		mov ss:[DISK_STK_S], dx 
		xor dx, dx
		mov bx, ss:[DISK_STK_TH]
		div bx
		mov ss:[DISK_STK_H], dx
	
	; S = (LBA % tS) + 1
	inc word ptr ss:[DISK_STK_S]

	; conversao
		mov cx, ss:[DISK_STK_C]
		xchg cl, ch
		shl cl, 6
		mov ax, ss:[DISK_STK_S]
		and al, 3Fh
		or cl, al
		mov ax, ss:[DISK_STK_H]
	pop dx
		mov dh, al
	M_pop2 ax, bx		; CX, DH mudados
mov sp, bp
pop bp
ret

CHS_testConvert:
	; dx:ax tem LBA, BL=tS BH=tH 
		M_encodeCHS 0h, 1h, 1h
		call tty_CHS
		call tty_crlf
	mov SI, 81h  ; tC
	mov BH, 0fh  ; tH
	mov BL, 3fh  ; tS
		call p_chs_to_lba
		M_tty_wrStr " lba:"
		push ax 
			mov ax, dx
			call tty_Hex16
		pop ax
		call tty_Hex16
	mov SI, 81h ; tC
	mov BH, 0fh  ; tH
	mov BL, 3fh ; tS
		call tty_crlf
		call p_lba_to_chs
		call tty_crlf
		call tty_CHS
	hlt_
ret


; retorna em CL o primeiro bit em AX
getFirstOne:
push ax
	mov cx, 16
	getFirstOne_:
		test ax, 1
		jnz getFirstOneFound
		shr ax, 1
	loop getFirstOne_
		pop ax
		stc
		ret 
	getFirstOneFound:
		mov ax, 16
		sub ax, cx
		mov cx, ax
		pop ax
		clc
		ret

dispDigits16:
	push ax
	push bx
	push dx
	M_DispDigits16 ax, bx
	pop dx
	pop bx
	pop ax
ret