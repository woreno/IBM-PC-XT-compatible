
; descompressores
iF BUILD_BOOTSCR_DRAW EQ 1
	include RLE.INC		; variação de run-length-encoding
ENDIF
IF (BOOTSCR_TALKING_CLOCK+BUILD_TALKING_CLOCK) NE 0
	include ACP.INC		; variação de acomp (delta modulation)
	include SPKDEF.INC
	include SPK.INC
	include VOICES.INC
ENDIF

dsToSpkParams:
	push ax
		mov ax, SPK_PARAMS_SEG_
		mov ds, ax
	pop ax
ret


	; apenas se os recursos estiverem num ficheiro....

IF RESOURCES_USES_FILE EQ 1
res_filename:
	db RESOURCES_FILE, 0
res_FileHandle:
	dw 0
res_segAlloc:	; aloca o ficheiro em "res_filename"
	dw 0
	
	resources_init:
		call dsToSpkParams
		mov word ptr ds:[offset spk_param_seg], ax

		call dos_load_resources
								
		mov al, RES_ID_VOICES
		call dsToResource			; ds:si
		xor si, si					; alocar para o tamanho descomprimido
		call dos_alloc_sound		; ax = segmento		
		call dsToSpkParams
		mov word ptr ds:[offset spk_param_seg], ax ; guardar segmento
		mov es, ax	; agora é segmento de descompressão		
		xor di, di
									; ds = segmento origem
		mov al, RES_ID_VOICES
		call dsToResource			; ds:si
		call acp_uncompress
	ret

	dos_load_resources:
		; --------------------------------------- abrir ficheiro
		mov ax, cs
		mov ds, ax
		mov dx, offset res_filename
		mov ah, 3Dh
		xor al, al
		INT 21h
		or al, al
		jnc dos_load_resources_1
			call ds2cs
			M_tty_wrStr "nao consigo abrir res.bin err:"
			call tty_Hex16
			hlt
		dos_load_resources_1:
		; --------------------------------------- guardar handle
		mov word ptr cs:[offset res_FileHandle], ax
		; --------------------------------------- alocar 32KB
		mov al, 0
		mov bx, (UNIT_16K+UNIT_4K)/16	; 32K em paragrafos
		mov ah, 48h
		int 21h 
		jnc dos_load_resources_2
			call ds2cs
			M_tty_wrStr "nao consigo alocar res.bin, err:"
			call tty_Hex16
			M_tty_wrStr " max:"
			mov ax, bx
			call tty_Hex16
			hlt
		dos_load_resources_2:
		; --------------------------------------- guardar segmento
			mov word ptr cs:[offset res_segAlloc], ax
			call ds2cs
			M_tty_wrStr "seg. de recursos:"
			call tty_Hex16
			call tty_crlf
		; --------------------------------------- ler ficheiro
			mov ds, ax	   ; DS:DX=destino
			xor dx, dx
			mov bx, word ptr cs:[offset res_FileHandle]
			mov cx, UNIT_32K ; 32K
			xor al, al
			mov ah, 3Fh
			int 21h
		jnc dos_load_resources_3
			call ds2cs
			M_tty_wrStr "nao consigo ler mais res.bin, err:"
			hlt
		dos_load_resources_3:
		; --------------------------------------- ok, sair
			call ds2cs
			M_tty_wrStr "ficheiro recursos, lidos:"
			call tty_U16
			ret

			
	dsToResource:
		assume DS:CODE
		;    ______________segAlloc
		;	| segoff0 |--.
		;	|---------|  |		; se o teste se faz em DOS é chamado a resources_init
		;	| segoff1 |  |		; que lê os bonecos e sons pela a API 21h para uma
		;	xxxxxxxxxxx  |		; zona alocada previamente com o nome "res_segAlloc"
		;   |  seg0   |<´		; res_segAlloc tem o mapa de offsets para os conteudos a seguir
		;   |  seg1   |			; o registo de SEGMENTO tem de ficar a "res_segAlloc"+
								; um desses offsets
		mov bl, al
		xor bh, bh				; BX=um offset
		shl bx, 1
		mov ax, word ptr cs:[res_segAlloc] ; DS=segmento de alocacao
		mov ds, ax				; 
		mov ax, ds:[bx]			; AX=DS:[BX] segmento (offset) pretendido
		mov bx, word ptr cs:[res_segAlloc];	adicionar segAlloc
		add ax, bx				
		mov ds, ax
		xor si, si
	ret
ELSE

	; se ROM, o mapa está no inicio da ROM
	; e o conteudo em outro lugar
	assume DS:CODE
	;    _____________ROM_RES_SEG_START
	;	| segoff0 |--.
	;	|---------|  |		
	;	| segoff1 |  |		
	;	-----------
	dsToResource:
		mov bl, al
		xor bh, bh
		shl bx, 1
		mov ax, word ptr cs:[ROM_RES_SEG_START+bx]
		mov ds, ax
	ret
ENDIF


