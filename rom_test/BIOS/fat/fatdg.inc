
IF FAT_DEBUG_FATCASES EQ 1
p_fat12_lookupCase0:
push ds
	call ds2cs
	M_tty_wrStr " <0>"
pop ds
ret
p_fat12_lookupCase1:
push ds
	call ds2cs
	M_tty_wrStr " <1>"
pop ds
ret
ENDIF

IF FAT_DEBUG_CHAIN EQ 1
p_fat_copyFileDebug1:
M_push2 ds, ax
	call ds2cs
	M_tty_wrStr " tamanho:"
	M_mov ds, FAT_WORKSEG
	mov ax, ds:[fatwork_entry_SzHi]
	call tty_hex16
	mov ax, ds:[fatwork_entry_Sz]
	call tty_hex16
M_pop2 ds, ax
ret
p_fat_copyFileDebug2:
M_push2 ax, ds
	call ds2cs
	M_tty_wrStr " cluster:"
	mov ax, dx
	call tty_Hex16
M_pop2 ax, ds
call tty_Hex16
ret
ENDIF
IF FAT_DEBUG_CHAINBUFFER EQ 1
p_fat_copyFileDebug3:
	M_push6 ax, ds, es, di, si, di
	M_mov es,  UPDATE_SEG
	M_mov ds, es
	mov si, di
	call tty_buffer
	call tty_crlf
	xor ax, ax
	INT 16h
	M_pop6 ax, ds, es, di, si, di
	ret
ENDIF



IF FAT_DEBUG_LOADER EQ 1
p_fat_loadBuffDebug1:
	push ds
	call ds2cs
	call tty_crlf
	push ax
		M_tty_wrStr " load LB="
		mov ax, dx
		call tty_Hex16
	pop ax
	call tty_Hex16
	push ax
		call tty_sp
		M_tty_wrStr " ES:DI="
		mov ax, es
		call tty_Hex16
		call tty_ddot
		mov ax, di
		call tty_Hex16
	pop ax
	pop ds
ret
p_fat_loadBuffDebugCHS:
	M_push2 ax,ds
		call ds2cs
		call tty_crlf
		M_tty_wrStr " DRV:"
		mov al, dl
		call tty_Hex8
		call tty_chs
	M_pop2 ax,ds
ret
ENDIF

IF FAT_DEBUG_LOADERBUFFER EQ 1
p_fat_loadBuffDebug3:
	M_push4 ax, ds, di, si
	M_mov ds, es
	xor di, di
	xor si, si
	call tty_buffer
	xor ax, ax
	INT 16h
	add di, 256
	mov si, 256
	call tty_buffer
	xor ax, ax
	INT 16h
	M_pop4 ax, ds, di, si
ret
ENDIF


IF FAT_DEBUG_SIGN EQ 1
	p_fat_getTypeDebug:
	call tty_crlf
	call ds2cs
	M_tty_wrStr "sign4:"
	M_mov ds, es
	mov si, fat_hdrdos4_sign
	call tty_strn
	call ds2cs
	M_tty_wrStr " sign7:"
	M_mov ds, es
	mov si, fat_hdrdos7_sign
	call tty_strn
	xor ax, ax
	INT 16h
	ret
ENDIF

	