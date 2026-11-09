TEST_RTC  EQU 0

;-------------------------------------------------------------------------------------
; 1. Trocar memória RTC por outra (DEBUG)
; 2. rtc_batIsOK: stc se o bit de bateria fraca
; 2. rtc_cfg_isOK: stc se o cookie não está gravado
; 3. rtc_saveCookie: grava  o cookie 
; 4. rtc_BootPriorityCheck: verifica se o vetor de arranque está OK
; 5. rtc_test: ativa o oscilador interno e faz set das data/hora e le

;-------------------------------------------------------------------------------------
; 1. Trocar memória RTC por outra (DEBUG)
IF RTC_DEBUG      EQ  1
	rtcTestArr: db 200 dup(0)
ENDIF


;-------------------------------------------------------------------------------------
; 2. rtc_batIsOK: stc se o bit de bateria fraca
rtc_batIsOK:
	M_RTC_Read_ RTC_STATUS_D
	test al, 80h				; se ZERO a bateria esgotou
	jnz rtc_batIsOK_Ok
		stc
		ret
	rtc_batIsOK_Ok:
		clc
		ret
		
;-------------------------------------------------------------------------------------
; 3. rtc_cfg_isOK: stc se o cookie não está gravado
rtc_cfg_isOK:	
	call rtc_batIsOK
	jc rtc_cfg_isNotOk
	M_RTC_Read_ RTC_RAM_COOKIE1
	xchg al, ah
	M_RTC_Read_ RTC_RAM_COOKIE2
	xchg al, ah
	cmp ax, RTC_RAM_COOKIE_SIGN1
	jne rtc_cfg_isNotOk
	
	M_RTC_Read_ RTC_RAM_COOKIE3
	xchg al, ah
	M_RTC_Read_ RTC_RAM_COOKIE4
	xchg al, ah
	cmp ax, RTC_RAM_COOKIE_SIGN2
	jne rtc_cfg_isNotOk
	clc
	ret
	rtc_cfg_isNotOk:
	stc
ret

rtc_romIsUpdated:
	call rtc_cfg_isOK
	jc rtc_romIsUpdated1
	M_RTC_Read_ RTC_RAM_ROMISNEW
	test al, 1
	jz rtc_romIsUpdated1
	M_RTC_Set_ RTC_RAM_ROMISNEW, 0
	clc
	ret
rtc_romIsUpdated1:
	stc
	ret

;-------------------------------------------------------------------------------------
; 3. rtc_saveCookie: grava  o cookie 
rtc_saveCookie:
	mov al, (RTC_RAM_COOKIE_SIGN1 AND 0FFh)
	M_RTC_Set_ RTC_RAM_COOKIE1, al
	mov al, (RTC_RAM_COOKIE_SIGN1 SHR 8)
	M_RTC_Set_ RTC_RAM_COOKIE2, al
	mov al, (RTC_RAM_COOKIE_SIGN2 AND 0FFh)
	M_RTC_Set_ RTC_RAM_COOKIE3, al
	mov al, (RTC_RAM_COOKIE_SIGN2 SHR 8)
	M_RTC_Set_ RTC_RAM_COOKIE4, al
ret

;-------------------------------------------------------------------------------------
; 4. rtc_BootPriorityCheck: verifica se o vetor de arranque está OK
rtc_BootPriorityCheck:
	
	M_RTC_Read_ RTC_RAM_BOOT_PRI
	
	mov dl, 0				; nº da unidade
	mov cx, 4				; CX=4	
	rtc_sbp_loop:
		push ax
		push cx		
			mov cx, 4		; CX=4 elementos a verificar
			mov dh, 0		; DH=0 contador de repetições
			rtc_sbp_unique:	; iterar valores
				mov ah, al	; AH=AL
				and ah, 11b	; AH=AL&3
				cmp dl, ah	; AH=DL?
				jne rtc_sbp_uniqueCont
					inc dh	; DH++ (1 repetição)
				rtc_sbp_uniqueCont:
				shr al, 2	; AL>>=2
			loop rtc_sbp_unique
		pop cx
		pop ax

		cmp dh, 1					; DH==1?
		jne rtc_sbp_error			; ERRO SENAO

		inc dl
	loop rtc_sbp_loop
		clc
		ret

	rtc_sbp_error:
		mov al, 11100100b
		M_RTC_Set_ RTC_RAM_BOOT_PRI, al
		stc
		ret



;-------------------------------------------------------------------------------------
; 5. rtc_test: ativa o oscilador interno e faz set das data/hora e le
IF TEST_RTC EQ 1
rtc_test:
	call longlongWait
	call rtc_set
	call rtc_read
	call beep
	call blinkLeds
	call rtc_set
	call beep

rtc_set:
	M_PPI_showDip1
	M_PPI_TestDip 3
	jnz rtc_reset
		ret
	rtc_reset:
	cli
	; ativar o oscilador interno
	; (escrever 010 nos bits 6:4 do registo A)
	M_RTC_Set_ RTC_STATUS_A, RTC_STATUS_A_CFG
	M_RTC_Set_ RTC_STATUS_B, 80h+RTC_STATUS_B_CFG
	M_RTC_Set_ RTC_AL_SEC,  0FFh
	M_RTC_Set_ RTC_AL_MIN,  0FFh
	M_RTC_Set_ RTC_AL_HOUR, 0FFh

	M_RTC_Set_ RTC_HOUR,    06h
	M_RTC_Set_ RTC_MIN,     50h
	M_RTC_Set_ RTC_SEC,     05h

	M_RTC_Set_ RTC_WDAY,    07h
	M_RTC_Set_ RTC_MDAY,    17h
	M_RTC_Set_ RTC_MONTH,   04h
	M_RTC_Set_ RTC_YEAR,    22h
	M_RTC_Set_ RTC_STATUS_B, RTC_STATUS_B_CFG
	
	M_RTC_Set_ RTC_RAM+0,	067h
	M_RTC_Set_ RTC_RAM+1,	0A6h
	sti

	call ds2cs
	M_tty_wrStr "RTC SET!"
	call beep
ret



rtc_read:
	call tty_clear
	call tty_saveCur
	mov ax, 0
loop1:
	push ax
	call ds2cs
	call tty_restoreCur
	M_RTC_DebugRead RTC_STATUS_A
	M_RTC_DebugRead RTC_STATUS_B
	M_RTC_DebugRead RTC_STATUS_C
	M_RTC_DebugRead RTC_STATUS_D
	M_RTC_DebugRead RTC_HOUR
	M_RTC_DebugRead RTC_MIN
	M_RTC_DebugRead RTC_SEC
	M_RTC_DebugRead RTC_WDAY
	M_RTC_DebugRead RTC_MDAY
	M_RTC_DebugRead RTC_MONTH
	M_RTC_DebugRead RTC_YEAR
	M_RTC_DebugRead RTC_RAM+0
	M_RTC_DebugRead RTC_RAM+1
	pop ax
	call tty_crlf
	inc al
	call tty_U8
jmp loop1
ret

ENDIF


rtc_printinfo:	
	mov al, 0Ah
	mov cx, 4
	rtc_info_:
		call tty_crlf
		call tty_Hex8
		call tty_ddot
		push ax
			M_RTC_Read al
			call tty_bin8
		pop ax
		inc al
	dec cx
	jnz rtc_info_

ret
;-----------------------------------------------------------------
; imprimir data/hora
rtc_printDate:
push ax
push dx
	M_RTC_Read_ RTC_MDAY
	call BCDtoU8
	call tty_U8time
	mov al, '/'
	call tty_char
	M_RTC_Read_ RTC_MONTH
	call BCDtoU8
	call tty_U8time
	mov al, '/'
	call tty_char
	M_RTC_Read_ RTC_YEAR
	call BCDtoU8
	call tty_U8time
pop dx
pop ax
ret

rtc_printTime:
push ax
push dx
	M_RTC_Read_ RTC_HOUR
	call BCDtoU8
	call tty_U8time
	mov al, ':'
	call tty_char
	M_RTC_Read_ RTC_MIN
	call BCDtoU8
	call tty_U8time
	mov al, ':'
	call tty_char
	M_RTC_Read_ RTC_SEC
	call BCDtoU8
	call tty_U8time
pop dx
pop ax
ret

rtc_printClock:
	call rtc_printDate
	call tty_sp
	call rtc_printTime
ret


; es,di,bx=registo, cx=tamanho
rtc_readString:
push di
	rtc_readString_:
	M_RTC_Read_ bl
	mov dx, RTC_ADDR 
	mov al, bl
	out dx, al
	mov dx, RTC_DATA 
	in al, dx
	stosb
	loop rtc_readString_
pop di
ret


M_RTC_ReadString MACRO REGNAME_, COUNT_
LOCAL @@loop1
mov cx, COUNT_
loop1:
	in  al, dx       
	stosb
ret
ENDM
