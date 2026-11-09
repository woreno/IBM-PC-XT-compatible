
M_waitEnterKey MACRO 
LOCAL @@loop
	pushf
	push ax
	@@loop:
		xor ah, ah
		INT 16h
		cmp al, 13
		jne @@loop
	pop ax
	popf
ENDM

M_waitKey MACRO 
	push ax
		xor ah, ah
		INT 16h
	pop ax
ENDM

M_enterInt MACRO INT_
	push ds
	push ax
		call tty_restoreCur
		mov ax, 25
		call tty_clearLines
		call ds2cs
		call tty_crlf
		M_tty_ChgAtt 017h
		M_tty_wrStr "INT"
		mov al, INT_
		call tty_Hex8
		M_tty_ChgAtt 007h
		M_Countdown ax, 0FFFFh
	pop ax
	pop ds
ENDM
M_installINT MACRO VECTOR_, NAME_
	mov ax, offset NAME_
	mov word ptr ds:[VECTOR_*4], ax
	mov ax, PROGRAM_SEG
	mov word ptr ds:[VECTOR_*4+2], ax
ENDM

DATA SEGMENT AT DATA_SEG 
	tty_row			db   ?
	tty_col			db   ?
	tty_att			db   ?
	tty_lastRow		db   ?
	tty_lastCol		db   ?
	tty_lastAtt		db   ?
	tty_strbuff 	db 	 10 dup(?)
	tty_strbuffEnd	db   ?
DATA ENDS
