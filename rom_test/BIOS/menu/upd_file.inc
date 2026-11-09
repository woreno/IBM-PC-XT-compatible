;-----------------------------------------------------------------
; 1.            p_upd_file: menu para selecionar da unidade
; 2.        p_upd_FromDisk: define params; verifica MBR; define offset LBA 
; 3.        p_upd_Diskette: define params; define offset LBA 0
; 4.       p_udp_fromDisks: verifica FAT, chama a procurar o ficheiro
; 5. p_upd_getHeaderSector: apanha o 1º setor (consoante offset LBA)
; 6.   p_upd_setDiskParams: define os params em FAT_WORKSEG

upd_filename db "UPDATE  PKG"
include fat.inc

;-----------------------------------------------------------------
; 1. p_upd_file: menu para selecionar da unidade
p_upd_file:
	call p_menu_clear
	mov si, offset bk03+1
	call p_menu_setTitle
	mov si, offset bm15_footer_str
	call p_menu_setFooter

	call p_menu_clearScreen
	call tty_crlf
	M_tty_cwrStr "E' necessario que exista o ficheiro UPDATE.PKG numa das"
	M_tty_cwrStr "unidades e que o esquema de particoes seja MBR se discos;"
	M_tty_cwrStr "a formatacao FAT12/16/32 seja disco ou disquete."
	M_tty_cwrStr "Se discos, a primeira particao ativa vai ser procurada."
	
	mov dx, 0A20h
	call tty_setCur
	M_tty_wrStr "Escolha unidade:"
	
	add dh, 2
	mov bx, 0
	p_upd_diskloop2:
		mov bh, 0
		mov dx, 0C26h
		mov cx, 4
		mov di, offset bmenu_disks_strs
		p_upd_diskloop1:
			call tty_setCur
			push bx
				mov bl, B_MENU_BACKCOLOR
				call tty_setAtt
			pop bx
			cmp bh, bl
			jne p_upd_diskno
			push bx
				mov bl, B_MENU_SELCOLOR
				call tty_setAtt
			pop bx
			p_upd_diskno:
			mov si, cs:[di]
			call tty_str
			inc dh
			inc bh
			add di, 2
		loop p_upd_diskloop1

		p_upd_diskkeyloop:
			xor ax, ax
			INT 16h
			cmp ah, KEY_SCANCODE_ESCAPED
			je p_upd_diskkeyloop

		cmp ah, KEY_SCANCODE_ENTER
		je p_upd_diskloop_select
		cmp ah, KEY_SCANCODE_ESC
		je p_upd_diskloop_esc
		cmp ah, KEY_SCANCODE_UP
		je p_upd_diskloop_up
		cmp ah, KEY_SCANCODE_DOWN
		je p_upd_diskloop_down
		jmp p_upd_diskloop2

		p_upd_diskloop_up:	
			dec bl
			and bl, 3
		jmp p_upd_diskloop2
		p_upd_diskloop_down:	
			inc bl
			and bl, 3
		jmp p_upd_diskloop2

		p_upd_diskloop_esc:
			call p_menuShow
		p_upd_diskloop_select:
		
		; bl tem a unidade selecionada se bit 1 FDD
		test bl, 10b
		jnz p_upd_isDiskette		
			call p_upd_FromDisk 
		p_upd_isDiskette:
			call p_upd_FromDiskette

;-----------------------------------------------------------------
; 2. p_upd_FromDisk: define params; verifica MBR; define offset LBA 
p_upd_FromDisk:
	mov dl, bl
	and dl, 1h
	or  dl, 80h		
	call p_upd_setDiskParams
	call p_upd_getHeaderSector

	mov ax, word ptr es:[01FEh]
	cmp ax, 0AA55h
	je 	p_upd_FromDisk1
				
		call p_menu_clearScreen
		M_tty_cwrStr "Este disco nao tem assinatura MBR"
		call p_menu_errorDialog

	p_upd_FromDisk1:

		M_mov ds, FAT_WORKSEG
		mov di, 01BEh			; 1ª partição
		mov cx, 4
		p_upd_partLoop:			; rodar partições
			mov al, es:[di]
			test al, 7Fh		; a 1ª ativa testa
			jz p_upd_foundActivePart
			add di, 16
		loop p_upd_partLoop

			call p_upd_FromDiskNoPartsError

		p_upd_foundActivePart:

								; CHS
		mov dh, es:[di+1]		; cabeça
		mov cx, es:[di+2]		; setor+cilindro
		 
								; imprimir CHS
		IF FAT_DEBUG_DISK EQ 1
			call p_upd_FromDiskDebug1 
		ENDIF
		
		mov ax, es:[di+8]		; offset LBA
		mov dx, es:[di+10]		
		
		or ax, dx
		jz p_upd_FromDiskNoLBA
			; existe LBA usar este para offset
			mov ax, es:[di+8]
			jmp p_upd_FromDiskOK

		p_upd_FromDiskNoLBA:
			mov dh, es:[di+1]		; restaurar cabeça
									; nao há LBA,
									; converter CHS para LBA
			IF FAT_DEBUG_DISK EQ 1
				call p_upd_FromDiskDebug2
			ENDIF
			mov SI, ds:[fatwork_param_TC]
			mov BH, ds:[fatwork_param_TH]
			mov BL, ds:[fatwork_param_TS]
			call p_chs_to_lba
			xchg bh, dh

		p_upd_FromDiskOK:
			mov ds:[fatwork_LBAoffset], ax
			mov ds:[fatwork_LBAoffsetHi], dx

		; ler de novo o header (agora já com o offset da 1ª partição)
		call p_upd_getHeaderSector

			IF FAT_DEBUG_DISK EQ 1
				xor ax, ax
				INT 16h
			ENDIF
			jmp p_udp_fromDisks

		; continuar para FAT...
;-----------------------------------------------------------------
; 3. p_upd_Diskette: define params; define offset LBA 0

p_upd_fromDiskette:
	mov DL, bl
	and DL, 1h
	call p_upd_setDiskParams
	call p_upd_getHeaderSector		

;-----------------------------------------------------------------
; 4. p_udp_fromDisks: verifica FAT, chama a procurar o ficheiro

p_udp_fromDisks:
	call p_fat_getType
	jnc p_udp_fromDisksOk
			call p_upd_fatSignError
	p_udp_fromDisksOk:
			call p_fat_init

			; copiar string a copiar
			mov cx, 8+3
			M_mov ds, cs
			mov si, offset upd_filename
			M_mov es, FAT_WORKSEG
			mov di, offset fatwork_searchFilename
			rep movsb

			; nome do tipo
			call p_menu_clearScreen
			mov si, es:[fatwork_typeStr]
			call ds2cs
			call tty_crlf
			M_tty_wrStr "   o tipo e' "
			call tty_str
			M_tty_wrStr " e tamanho dos clusters e' "
			mov cl, es:[fatwork_allocShifter]
			mov ax, 200h
			shl ax, cl
			call tty_bytesformat
			M_tty_wrStr " - shift:"
			mov al, cl
			call tty_U8
			call tty_crlf

			call p_fat_dirIt
			cmp ax, FAT_DIRIT_RES_SUCCESS
			je p_udp_fromFileSuccess
			cmp ax, FAT_DIRIT_RES_NOTFOUND
			je p_udp_fromFileNotFound
			jmp p_udp_fromFileError


p_udp_fromFileNotFound:		
	call ds2cs
	M_tty_cwrStr "UPDATE.PKG nao encontrado"
	call p_menu_errorDialog
p_udp_fromFileError:
	call ds2cs
	M_tty_cwrStr "Erro ao ler diretorio"
	call p_menu_errorDialog
p_udp_fromFileSuccess:		
	call ds2cs
	M_tty_cwrStr "UPDATE.PKG encontrado, a copiar..."
	xor si, si
	xor di, di
	M_mov es, UPDATE_SEG
	call p_fat_copyFile
	call p_upd_verifyFileHdr
	call p_upd_verifyFile 
	call p_upd_ready
ret
;-----------------------------------------------------------------
; 5. p_upd_getHeaderSector: apanha o 1º setor
p_upd_getHeaderSector:
	M_mov es, FAT_HEADERSEG
	xor ax, ax
	xor dx, dx
	xor di, di
	call p_fat_loadBuff
	jc p_upd_getHeaderSectorError
ret	
p_upd_fatSignError:
	call p_menu_clearScreen
	M_tty_cwrStr "Esta unidade nao tem assinatura FAT"
	call p_menu_errorDialog
	
p_upd_getHeaderSectorError:
	call p_menu_clearScreen
	M_tty_cwrStr "Nao consigo ler sector de arranque desta unidade"
	call p_menu_errorDialog
;-----------------------------------------------------------------
; 6. p_upd_setDiskParams: poem os params em FAT_WORKSEG
p_upd_setDiskParams:
	; 1. ES=FAT_WORKSEG
	M_mov es, FAT_WORKSEG, ax 
	
	; 2. apanhar parametros da unidade e gravar em FAT_WORKSEG
	mov byte ptr es:[fatwork_param_DRIVE], DL
	mov ah, 8
	M_push2 es, di
		INT 13h	
	M_pop2 es, di
	IF FAT_DEBUG_DISKPARAMS EQ 1
		push ds
			call ds2cs
			call tty_crlf
			M_tty_wrStr "diskParams:"
			call tty_CHS
		pop ds
	ENDIF
	mov ax, cx
	and ax, 3Fh
	mov byte ptr es:[fatwork_param_TS], AL
	xchg cl, ch
	shr ch, 6
	mov word ptr es:[fatwork_param_TC], CX
	inc DH
	mov byte ptr es:[fatwork_param_TH], DH
	xor ax, ax
	mov es:[fatwork_LBAoffset], ax
	mov es:[fatwork_LBAoffsetHi], ax
ret


p_upd_FromDiskNoPartsError:
	call ds2cs
	M_tty_cwrStr "Este disco nao tem nenhuma particao ativa"
	call p_menu_errorDialog


IF FAT_DEBUG_DISK EQ 1
	p_upd_FromDiskDebug1:
	M_push2 ax, ds
		call ds2cs
		call tty_crlf
		M_tty_wrStr "particao n."
		mov ax, di
		sub ax, 01BEh
		shr ax, 4
		inc ax
		call tty_U16
		M_tty_wrStr "/4 "
		call tty_chs
		M_tty_wrStr " lba:"
		mov ax, es:[di+10]
		call tty_Hex16
		mov ax, es:[di+8]
		call tty_Hex16
		xor ax, ax
		INT 16h
	M_pop2 ax, ds
	ret

p_upd_FromDiskDebug2:
	push ds
		call ds2cs
		call tty_crlf
		M_tty_wrStr "o MBR nao tem LBA"
	pop ds
ret
ENDIF