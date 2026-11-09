 
; estas sao posições para as frases de SPKTIME
VOICE_ID_FSTDIGITS			EQU  0 ; 1,2,3...19
VOICE_ID_UPDIGITS			EQU 19 ; 20,30,40,50
VOICE_ID_E_					EQU 23 ; "e'"
VOICE_ID_SAO				EQU 24 ; "sao"
VOICE_ID_MDIA				EQU 25 ; "meio-dia"
VOICE_ID_MNOITE				EQU 26 ; "meia-noite"
VOICE_ID_E					EQU 27 ; "e"
VOICE_ID_HORAS				EQU 28 ; "horas"
VOICE_ID_MIN				EQU 29 ; "minutos"
VOICE_ID_SEC				EQU 30 ; "segundos"
VOICE_ID_CHIME				EQU 31 ; 

IF SPK_TIME_PRINTS EQ 1
	M_spk_ttyStr MACRO X_
		push ds
			call ds2cs
			M_tty_wrStr X_
		pop ds
	ENDM
	M_spk_ttyAL	MACRO 
		call tty_U8
	ENDM
ELSE
	M_spk_ttyStr MACRO X_
	ENDM
	M_spk_ttyAL	MACRO 
	ENDM
ENDIF

; estas funções correspondem a um ficheiro especial 
; que tem os offsets (no fim)

; ------------------------------------------------------------------
; 1.           play_chime: toca o chime
; 1.           speak_time: fala as horas
; 2.         speak_number: reproduz o numero em AL (em formato BCD)
; 3.         speak_offset: toca um som AL, indexando a array de offsets "SND_OFFSETS"

play_chime:
	mov ax, VOICE_ID_CHIME
	call speak_offset
ret
;-------------------------------------------------------------------------
; 1. speak_time: fala as horas
speak_time:
	call ds2cs

	M_RTC_Read_ RTC_HOUR
	call BCDtoU8
	cmp al, 0
	je speak_time_00h	; é meia-noite
	cmp al, 12
	je speak_time_12h ; é meio-dia
	cmp al, 1
	je speak_time_1h ; é 1h
	jmp speak_time_intro 
	
	speak_time_1h:
		M_spk_ttyStr "E' "
		mov ax, VOICE_ID_E_
		call speak_offset
		mov ax, 1
		call speak_number
		M_spk_ttyStr "hora "
		mov ax, VOICE_ID_HORAS
		call speak_offset
		jmp speak_time_min 

	speak_time_00h:
		M_spk_ttyStr "E' meia noite "
		mov ax, VOICE_ID_E_
		call speak_offset
		mov ax, VOICE_ID_MNOITE
		call speak_offset	
		jmp speak_time_min 

	speak_time_12h:
		M_spk_ttyStr "E' meio dia "
		mov ax, VOICE_ID_E_
		call speak_offset
		mov ax, VOICE_ID_MDIA
		call speak_offset
		jmp speak_time_min

	speak_time_intro:
		M_spk_ttyStr "Sao "
		mov ax, VOICE_ID_SAO
		call speak_offset
		
		M_RTC_Read_ RTC_HOUR
		call speak_number
		M_spk_ttyStr "horas "
		
		mov ax, VOICE_ID_HORAS
		call speak_offset
		
	speak_time_min:

		M_RTC_Read_ RTC_MIN
		or al, al
		jz speak_time_sec
		push ax
			M_spk_ttyStr "e "
			mov ax, VOICE_ID_E
			call speak_offset
		pop ax
		call speak_number

		M_spk_ttyStr "min "
		mov ax, VOICE_ID_MIN
		call speak_offset

	speak_time_sec:
		;M_RTC_Read_ RTC_SEC
		;or al, al
		;jz speak_time_end
		;push ax
		;	M_spk_ttyStr "e "
		;	mov ax, VOICE_ID_E
		;	call speak_offset
		;pop ax
		;
		;call speak_number
		;M_spk_ttyStr "sec "
		;mov ax, VOICE_ID_SEC
		;call speak_offset
	
	speak_time_end:
	ret	
;-------------------------------------------------------------------------
; 2. speak_number: reproduz o numero em AL (em formato BCD)
;				   de 1 a 59
speak_number:
	or al, al
	jnz speak_numberNonZero
		ret
	speak_numberNonZero:
	cmp al, 20h
	ja speak_numberHi			
		call BCDtoU8			; se menor que 20
		M_spk_ttyAL
		xor ah, ah
		dec al
		call speak_offset
		ret
	speak_numberHi:				; se maior que 20
		test al, 0Fh
		jnz speak_numberHi1
								; 1º digito é zero (da direita)
			shr al, 4
			M_spk_ttyAL 
			dec al
			add al, VOICE_ID_UPDIGITS
			call speak_offset
			ret

		speak_numberHi1:
								; 1º digito não é zero
			push ax
				shr al, 4
				M_spk_ttyAL 
				sub al, 2
				add al, VOICE_ID_UPDIGITS
				call speak_offset
				mov al, VOICE_ID_E_
				call speak_offset
			pop ax
			and al, 0Fh
			call speak_number
		ret

	speak_numberHi2:
		push ax
			shr al, 4
			add al, VOICE_ID_UPDIGITS
			call speak_number
		pop ax
		and al, 0Fh
		call speak_number
		ret


	
;-------------------------------------------------------------------------
; 3. speak_offset: toca um som AL, indexando a array de offsets
; AL: posição na array de offsets (usar as constantes para as frases,
;                                  ou usar digito-1 para os números 1..19)
speak_offset:
push ax
push ds

	IF SPK_DEBUG_OFFSETS EQ 1
		call tty_crlf
		call tty_U8
		push ax
	
			mov al, '/'
			call tty_char
			mov al, VOICES_SZ/4
			call tty_U8
	
		pop ax
	ENDIF
	
	cmp al, VOICES_SZ/4
	jbe speak_offset_continue
		
		call ds2cs
		M_tty_wrStr "voices: fora do intervalo"
		hlt

	speak_offset_continue:

	mov si, offset VOICES_OFFSETS	; INDEXAR ARRAY DE OFFSETS
	xor ah, ah						; com AL
	shl ax, 2						
	add si, ax						; SI = AX*4
									; vetor de offsets
									; .------4bytes---.
									;   0     2           4     6
									; [ SEGR1 OFFSET1 ] [ SEGR2 OFFSET2 ]
									;	  |      |                 |
									;  bx=[si] si=[si+2]      di=[si+6]

	mov word ptr bx, cs:[si]		; BX = SEGMENTO RELATIVO
	mov word ptr di, cs:[si+6]		; DI = onde PARAR no SOM
	mov word ptr si, cs:[si+2]		; SI = onde COMECAR no SOM

	mov ax, SPK_PARAMS_SEG
	mov ds, ax
		IF SPK_DEBUG_OFFSETS EQ 1
			call tty_sp 
			call tty_Hex16
			call tty_sp
		ENDIF
	mov ax, word ptr ds:[offset spk_param_seg]
	mov es, ax
		IF SPK_DEBUG_OFFSETS EQ 1
			call tty_Hex16
			call tty_sp
			mov ax, si
			call tty_ddot
			call tty_Hex16
		ENDIF
	
	call speaker_play		; ES:SI aponta para o SOM
pop ds
pop ax
ret


VOICES_OFFSETS:                                                                                                                                              
dw 00h,00h			; [ SEGR , OFFSET ] 4 bytes
dw 00h,02F7h
dw 00h,06E3h
dw 00h,0981h
dw 00h,0CFFh
dw 00h,0115Ah
dw 00h,015B0h
dw 00h,01A08h
dw 00h,01DB5h
dw 00h,02122h
dw 00h,024F6h
dw 00h,02999h
dw 00h,02EA1h
dw 00h,0335Bh
dw 00h,03940h
dw 00h,03E6Eh
dw 00h,04590h
dw 00h,04C26h
dw 00h,051F4h
dw 00h,057C1h
dw 00h,05CD1h
dw 00h,0615Bh
dw 00h,064F5h
dw 00h,06A4Ah
dw 00h,06B9Dh
dw 00h,06F78h
dw 00h,07541h
dw 00h,07A61h
dw 00h,07BF6h
dw 00h,0804Fh
dw 00h,08638h
dw 00h,08BF8h
dw 00h,09704h
VOICES_SZ EQU $-offset VOICES_OFFSETS
