;-----------------------------------------------------------------
; 1. p_fat_getType: entra em ES o HEADER, encontra a assinatura
;                   e chama inicialização
; 2. p_fat_init:    guarda o tamanho da alocacao e chama init
;                 

fat_12str db "FAT12", 0
fat_16str db "FAT16", 0
fat_32str db "FAT32", 0
fat_strs  dw  fat_12str, fat_16str, fat_32str
;-----------------------------------------------------------------
; 1. p_fat_getType: entra em ES o HEADER, encontra a assinatura
;                   e chama inicialização
p_fat_getType: 
	mov cx, 5
	IF FAT_DEBUG_SIGN EQ 1
		call p_fat_getTypeDebug
	ENDIF
	call ds2cs
	mov di, fat_hdrdos4_sign
		mov si, offset fat_12str
		call strncmp
		jnc p_fat_is12
		mov si, offset fat_16str
		call strncmp
		jnc p_fat_is16	
	mov di, fat_hdrdos7_sign
		mov si, offset fat_12str
		call strncmp
		jnc p_fat_is12	
		mov si, offset fat_16str
		call strncmp
		jnc p_fat_is16	
		mov si, offset fat_32str
		call strncmp
		jnc p_fat_is32	
	stc
	ret
p_fat_is12:
	push es
	M_mov es, FAT_WORKSEG
	M_movOff es:[fatwork_typeStr],			 fat_12str 
	M_movOff es:[fatwork_p_fatInit],		 p_fat1216_init
	M_movOff es:[fatwork_p_fatLookup],       p_fat12_lookup
	M_movOff es:[fatwork_p_fatInitRootDir],  p_fat12_initRootDir
	M_movOff es:[fatwork_p_fatItRootDir],	 p_fat12_itRootDir
	jmp p_fat_isOK
p_fat_is16:
	push es
	M_mov es, FAT_WORKSEG
	M_movOff es:[fatwork_typeStr],			 fat_16str
	M_movOff es:[fatwork_p_fatInit],		 p_fat1216_init
	M_movOff es:[fatwork_p_fatLookup],       p_fat16_lookup
	M_movOff es:[fatwork_p_fatInitRootDir],  p_fat12_initRootDir
	M_movOff es:[fatwork_p_fatItRootDir],	 p_fat12_itRootDir
	jmp p_fat_isOK
p_fat_is32:
	push es
	M_mov es, FAT_WORKSEG
	M_movOff es:[fatwork_typeStr],			 fat_32str
	M_movOff es:[fatwork_p_fatInit],		 p_fat32_init
	M_movOff es:[fatwork_p_fatLookup],       p_fat32_lookup
	M_movOff es:[fatwork_p_fatInitRootDir],  p_fat32_initRootDir
	M_movOff es:[fatwork_p_fatItRootDir],	 p_fat32_itRootDir
p_fat_isOK:
	pop es
	clc
	ret
;-----------------------------------------------------------------
; 2. p_fat_init: guarda o tamanho da alocacao e chama init
p_fat_init:
	M_mov ds, FAT_HEADERSEG
	M_mov es, FAT_WORKSEG

	; guardar multiplicador 
	mov al, ds:[fat_hdr_numSectorsPerAlloc]
	mov byte ptr es:[fatwork_allocMul], al
	
	; guardar shifter
	xor ah, ah
	call getFirstOne
	
	mov byte ptr es:[fatwork_allocShifter], cl

	xor ah, ah
	mov al, ds:[fat_hdr_numReservedSectors]	
	mov es:[fatwork_FATStartLB], ax			; inicio da FAT

	xor ah, ah								
	mov al, ds:[fat_hdr_numFATs]
	mov bl, ds:[fat_hdr_numSectorsPerFAT]	; tamanho RESERVADO + FAT
	mul bl									; AX = nº fats x setores por fat
	mov bl, ds:[fat_hdr_numReservedSectors]
	xor bh, bh
	add ax, bx	;	+ reservados

	mov es:[fatwork_rootDirLB],  ax

	mov cx, ds:[fat_hdr_numRootFiles]
	mov bx, cx								; tamanho do diretorio raíz:
	shr cx, 4								; CX = (nº de ficheiros root*32)/512
	test bx, 1111b							;     + 1 se resto
	jz p_fat1216_noadd
		inc cx
	p_fat1216_noadd:
	mov es:[fatwork_rootDirNumLBs], cx		

	add ax, cx
	mov es:[fatwork_allocStartLB], ax		;  inicio das alocações
	 
	; inicializar o buffer FAT
	mov ax, 0FFFFh
	mov es:[fatwork_lastUsedFATLB], ax
	mov es:[fatwork_lastUsedFATLBhi], ax

	; outra inicialização...
	call word ptr es:[fatwork_p_fatInit]

ret

;-----------------------------------------------------------------
; 2. p_fat_init1216: 
;                    e onde começa diretorio raiz, alocações, etc
p_fat1216_init:
ret

p_fat32_init:
	M_mov ds, FAT_HEADERSEG
	M_mov es, FAT_WORKSEG
	
	mov ax, ds:[fat_hdr_dos7_numRootDirLB]
	M_mov es:[fatwork_rootDirLB],  ax
	mov ax, ds:[fat_hdr_dos7_numRootDirLB+2]
	M_mov es:[fatwork_rootDirLB+2],  ax
ret