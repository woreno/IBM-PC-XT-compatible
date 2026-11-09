;-----------------------------------------------------------------
; 1. p_fatX_InitRootDir: inicializa antes de iterar o diretorio
; 2.   p_fatX_ItRootDir: o que fazer para seg. pagina de diretorio
; 3.        p_fat_dirIt: itera o diretório raíz e procura o nome
;                        AX=sucesso/erro/não encontrado
;                        FAT_WORKSEG tem uma cópia da entrada
; 4.    p_fatIt_Entries: itera as entradas duma página de diretório

;-----------------------------------------------------------------
; 1. p_fatX_InitRootDir: inicializa antes de iterar o diretorio
;-----------------------------------------------------------------
; 2. p_fatX_ItRootDir: o que fazer para seg. pagina de diretorio
p_fat12_InitRootDir:
	M_mov ss:[FATSKT_ROOT_LB], ds:[fatwork_rootDirLB] 
	M_mov ss:[FATSKT_ROOT_LBHi], 0
	mov cx, ds:[fatwork_rootDirNumLBs]
ret
p_fat12_ItRootDir:					
	inc word ptr ss:[FATSKT_ROOT_LB]	
	dec cx						       ; na FAT12/16 se itera
	jz p_fat12_ItRootDirEnd		       ; página diretório raíz
		clc						       ; linearmente
		ret						       ; (CX=o nº de páginas
	p_fat12_ItRootDirEnd:		       ; está decidido pelo
		stc						       ; o numero de entradas
		ret						       ; no header)
;-----------------------------------------------------------------
p_fat32_InitRootDir:
	M_mov ss:[FATSKT_ROOT_LB], ds:[fatwork_rootDirLB] 
	M_mov ss:[FATSKT_ROOT_LBHi], ds:[fatwork_rootDirLBHi] 
ret
p_fat32_ItRootDir:					
	mov ax, ss:[FATSKT_ROOT_LB]		   ;
	mov dx, ss:[FATSKT_ROOT_LBHi] 	   ; na FAT32 se itera
	call p_fat32_lookup				   ; o diretório raiz como
	jc p_fat32_ItRootDirEnd			   ; qualquer outro:
		mov ss:[FATSKT_ROOT_LB],  ax   ; faz-se lookup da FAT
		mov ss:[FATSKT_ROOT_LBHi], dx  ; e carrega-se páginas
		M_mov es, FAT_DIRSEG		   ; enquanto o lookup não
		xor di, di					   ; dar <dx,ax>=terminal
		call p_fat_loadBuff			   ; 
		clc							   ; 
		ret							   ;
	p_fat32_ItRootDirEnd:
		stc
		ret

;-----------------------------------------------------------------
; 3. p_fat_dirIt: itera o diretório raíz e procura o nome

p_fat_dirIt:
	push bp
	mov bp, sp
	push 0
	FATSKT_ROOT_LB		EQU bp-2
	push 0
	FATSKT_ROOT_LBhi	EQU bp-4
	
	M_mov ds, FAT_WORKSEG

	; **** chamar a definir cluster inicial
	call word ptr ds:[fatwork_p_fatInitRootDir]
	  
	M_mov es, FAT_DIRSEG
	xor di, di
	p_fatDirItLoop:
						; carregar uma nova página
						; de entradas de diretório
		mov ax, ss:[FATSKT_ROOT_LB] 
		mov dx, ss:[FATSKT_ROOT_LBhi] 
		IF FAT_DEBUG_DIR EQ 1
			call p_fat_dirItDebug1
		ENDIF
						; carregar página de diretório
						; em DX:AX
		call p_fat_loadBuff
		jc p_fatItDirError
						; iterar à procura do FICHEIRO
		call p_fat_ItEntries
		jnc p_fatItDirFound 
						; se encontrou, ES:0 tem a ENTRADA
						; DO FICHEIRO

		IF FAT_DEBUG_DIR EQ 1
			call p_fat_dirItDebug2 
		ENDIF

		; *** chamar a definir o próximo cluster
		call word ptr ds:[fatwork_p_fatItRootDir]
		jc p_fatItNotFound

		jmp p_fatDirItLoop
		
p_fatItNotFound:
	mov ax, FAT_DIRIT_RES_NOTFOUND
	jmp p_fatItDirExit 

p_fatItDirFound:			; copiar info da entrada de diretorio
	M_mov ds, FAT_WORKSEG
	mov ax, word ptr es:[fat_entry_LB]
	mov ds:[fatwork_entry_LB], ax
	mov ax, word ptr es:[fat_entry_LBhi]
	mov ds:[fatwork_entry_LBHi], ax
	mov ax, word ptr es:[fat_entry_size]
	mov ds:[fatwork_entry_Sz], ax
	mov ax, word ptr es:[fat_entry_sizeHi]
	mov ds:[fatwork_entry_SzHi], ax
	mov ax, FAT_DIRIT_RES_SUCCESS	
	jmp p_fatItDirExit
p_fatItDirError:
	mov ax, FAT_DIRIT_RES_ERROR
p_fatItDirExit:		; e seguir para a cópia do ficheiro
	add sp, 2
	M_pop5 es, bx, cx, dx, si
	mov sp, bp
	pop bp   
	ret 
IF FAT_DEBUG_DIR EQ 1
	p_fat_dirItDebug1:
	push ds
		call ds2cs
		M_tty_wrStr " pagina " 
		push ax
			mov ax, cx
			call tty_U16
		pop ax
		M_tty_wrStr " de dir LB:"
		push ax
			mov ax, dx
			call tty_Hex16
		pop ax
		call tty_Hex16
	pop ds
	ret
	p_fat_dirItDebug2:
	push ax
		xor ax, ax
		INT 16h
	pop ax
	ret
ENDIF
;-----------------------------------------------------------------
; 3. p_fatItEntries: itera as entradas duma página de diretório
p_fat_ItEntries:
	M_push4 ds, cx, si, di
        M_mov ds, es
		xor si, si
		xor di, di
		IF FAT_DEBUG_ENTRIESBUF EQ 1
			push di
			push si
			call tty_buffer
			add di, 256
			mov si, di
			call tty_buffer
			pop si
			pop di
		ENDIF

	mov cx, 16
	p_fat_ItEntriesLoop:
		IF FAT_DEBUG_ENTRIES EQ 1
			call tty_crlf
			mov ax, cx
			call tty_U8time
			call tty_ddot
		ENDIF
		M_mov ds, es
		xor si, si
		xor di, di
		mov al, es:[0]
		or al, al
		jnz p_fat_ItEntriesCont
		
				IF FAT_DEBUG_ENTRIES EQ 1
					mov al, 'X'
					call tty_char
				ENDIF
				jmp p_fat_ItEntriesNext

		p_fat_ItEntriesCont:
		IF FAT_DEBUG_ENTRIES EQ 1
			 call p_fat_ItEntriesLoopDebug1 
		ENDIF
		;--------------------------
		push cx
			mov cx, 11
			M_mov ds, FAT_WORKSEG
			mov si, offset fatwork_searchFilename
			mov di, offset fat_entry_name
			call toUpper
			call strncmp
		pop cx
		jc p_fat_ItEntriesNext
			clc
			M_pop4 ds, cx, si, di
			ret

		p_fat_ItEntriesNext:	
			IF FAT_DEBUG_ENTRIES EQ 1
				xor ax, ax
				INt 16h
			ENDIF
		mov ax, es
		add ax, 2			; incrementar por 32 bytes
		mov es, ax			; ou 2 segmentos que é igual
		loop p_fat_ItEntriesLoop
		M_pop4 ds, cx, si, di
		stc
		ret
		 
IF FAT_DEBUG_ENTRIES EQ 1
p_fat_ItEntriesLoopDebug1:
	mov al, 0
	mov ah, ds:[fat_entry_ext]	; trim à string
	mov ds:[fat_entry_ext], al
	mov si, fat_entry_name
	call tty_str
	mov ds:[fat_entry_ext], ah	; e voltar a por o caractere de volta
	;--------------------------
	call tty_dot
	;--------------------------
	mov ah, ds:[fat_entry_atts]
	mov ds:[fat_entry_atts], al
	mov si, fat_entry_ext
	call tty_str
	mov ds:[fat_entry_atts], ah
ret
ENDIF

