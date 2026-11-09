;-----------------------------------------------------------------
; 1. p_fat_copyFile:      copiar o ficheiro para a ES:0
; 2. p_fatX_lookup:       entra DX:AX nº de bloco e sai seguinte 
;                         carry se terminou
; 3. p_fat_loadTab:	      chamada por p_fatX_lookup, p_fatX_lookup dá
;                         um deslocamento em AX na tabela; este devolve 
;                         DI relativo a uma primeira página da tabela
;                         da tabela que carregou na RAM
; 4. p_fat_loadBuff:      entra DX:AX com LB, carrega em ES:DI, 512b
; 5. p_fat_loadCluster:   entra DX:AX com Cluster, carrega em ES:DI, Xb

include FAT.DA
include FATINIT.INC
include FATDIR.INC
include FATDG.INC

;-----------------------------------------------------------------
; 1. p_fat_copyFile: copiar o ficheiro para ES:0
p_fat_copyFile:
	M_mov ds, FAT_WORKSEG 	; começar no LB inicial guardado

	IF FAT_DEBUG_CHAIN EQ 1
		call p_fat_copyFileDebug1 
	ENDIF

	; carregar 1º cluster e tamanho do ficheiro
	mov ax, ds:[fatwork_entry_LB]
	mov dx, ds:[fatwork_entry_LBhi]
	mov bx, ds:[fatwork_entry_Sz]
	mov cx, ds:[fatwork_entry_SzHi]
	
	IF FAT_DEBUG_CHAIN EQ 1	
		call tty_crlf
	ENDIF

	p_fat_copyFile_loop:
		IF FAT_DEBUG_CHAIN EQ 1	
			call p_fat_copyFileDebug2
		ENDIF
		 
		call p_fat_loadCluster 
		
		IF FAT_DEBUG_CHAINBUFFER EQ 1
			call p_fat_copyFileDebug3
		ENDIF

		; pedir próximo CLUSTER em <dx:ax>
		; se carry ACABOU
		call word ptr ds:[fatwork_p_fatLookup]
		jc p_fat_copyFileExit

	jmp p_fat_copyFile_loop
	p_fat_copyFileExit:
		IF FAT_DEBUG_CHAIN EQ 1	
			call ds2cs
			call tty_crlf
			M_tty_wrStr "fim"		
		ENDIF
	ret

;-----------------------------------------------------------------
; 2. p_fatX_lookup: DX:AX entra nº de bloco, e sai o próximo
;					carry se terminou

p_fat12_lookup:		
	M_push2 es, di
		xor dx, dx	
		test ax, 1	; paridade do nº de bloco
		pushf
		shr ax, 1	; encontrar o 1º byte que corresponde à entrada	
		mov bx, 3	; (ax*12)/8 => ax*(3/2) na tabela FAT	
		mul bx		; 
		IF FAT_DEBUG_LOOKUP EQ 1	
		push ds
			call ds2cs
			call tty_crlf
			M_tty_wrStr " offset na tab:"
			call tty_Hex16
		pop ds
		ENDIF
		; AX tem a localização de 2 BYTES na tabela FAT
		; loadTable vai devolver ES:DI relativo a uma
		; página que ele carregar
		call p_fat_loadTab 
	popf	; restaurar paridade do nº de bloco
	jz p_fat12_lookup_case0
		
		IF FAT_DEBUG_FATCASES EQ 1	
			call p_fat12_lookupCase1
		ENDIF
		mov ax, word ptr es:[di+1]
		shr ax, 4
		and ax, 0FFFh
		jmp p_fat12_lookup_exit

	p_fat12_lookup_case0:
		IF FAT_DEBUG_FATCASES EQ 1
			call p_fat12_lookupCase0
		ENDIF
		xor dx, dx
		mov ax, word ptr es:[di+0]
		and ax, 0FFFh
	p_fat12_lookup_exit: 
		IF FAT_DEBUG_FATCASES EQ 1
			call tty_Hex16
		ENDIF		
		cmp ax, 0FFFh
		je p_fat12_lookup1
			clc
			jmp p_fat12_lookup2
		p_fat12_lookup1:
			stc
		p_fat12_lookup2:
			M_pop2 es, di
			ret

p_fat16_lookup:
	M_push2 es, di		; como ocupam 2 bytes cada
		xor dx, dx		; AX deve ser multiplicado
		test ax, 8000h
		jz p_fat16_noadd
			inc dx
		p_fat16_noadd:
		shl ax, 1
		call p_fat_loadTab 
		mov ax, es:[di]
		cmp ax, 0FFF8h
		jb p_fat16_lookup1
			stc
			jmp p_fat16_lookup2
		p_fat16_lookup1:
			clc
		p_fat16_lookup2:
	M_pop2 es, di
ret

p_fat32_lookup:
	M_push3 es, di, cx
		shl dx, 2
		mov cx, ax
		shr cx, 14
		or dx, ax
		shl ax, 2
		call p_fat_loadTab 
		mov ax, es:[di]
		mov dx, es:[di+2]
		cmp ax, 0FFFFh
		jne p_fat32_lookup1
		cmp dx, 00FF8h
		jb p_fat32_lookup1
			stc
			jmp p_fat32_lookup2
		p_fat32_lookup1:
			clc
		p_fat32_lookup2:
	M_pop3 es, di, cx
ret
;-----------------------------------------------------------------
; 3. p_fat_loadTab: carrega em RAM 2 FATs (se necessário)
;                   onde se encontra indice DX:AX e 
;                   devolve ES:DI (DI o indice relativo)
p_fat_loadTab:
	M_push3 ax, bx, cx

	M_mov es, FAT_WORKSEG, bx		; ES=FAT_WORKSEG
	mov DI, AX	 ; DI fica com resto
	and DI, 1FFh ; da divisão por 512b <!>
	shr AX, 9    ; a divisão por 512 é a 1ª pagina
			     ; = LBA (relativo)

	IF FAT_DEBUG_LOADTAB EQ 1
		push AX
			call tty_crlf
			M_tty_wrStr " offset rel. (DI):"
			mov ax, di
			call tty_Hex16
		pop AX
	ENDIF

	; comparar este buffer: foi o ultimo usado?
	mov BX, ES:[fatwork_lastUsedFATLB]
	cmp AX, BX 
	jne p_fat_loadTab_isNew

			IF FAT_DEBUG_LOADTAB EQ 1
				call tty_crlf
				M_tty_wrStr " loadtab - reusando parte da tab #"
				call tty_hex16
			ENDIF
			M_mov es, FAT_FATSEG
			M_pop3 ax, bx, cx
			ret

	p_fat_loadTab_isNew:

		IF FAT_DEBUG_LOADTAB EQ 1
			call tty_crlf
			M_tty_wrStr " loadtab - nova parte tab #"
			call tty_Hex16
		ENDIF

		; marcar que esta foi o última tab
		mov es:[fatwork_lastUsedFATLB], AX
		
		M_push2 dx, di	; guardar o DI que interessa devolver
		
		; é preciso 2 carregar buffers
		; mas antes somar o inicio da FAT em LBs	
		xor dx, dx								 ; <!>
		add ax, es:[fatwork_FATStartLB]
		
		M_mov es, FAT_FATSEG, bx		; seg de instalação
		xor di, di
		mov cx, 2
		p_fat_loadTab_loop:
			call p_fat_loadBuff
			add di, 512
			inc ax
			loop p_fat_loadTab_loop

			M_pop2 dx, di
			M_pop3 ax, bx, cx
			ret
		 
;-----------------------------------------------------------------
; 4. p_fat_loadBuff: entra DX:AX com LB, carrega em ES:DI
p_fat_loadBuff:
M_push6 ax, bx, cx, dx, ds, si
	
	M_mov ds, FAT_WORKSEG, bx
									; adicionar a DX:AX o
									; inicio da partição
	add ax, ds:[fatwork_LBAoffset]
	adc dx, ds:[fatwork_LBAoffsetHi]

	IF FAT_DEBUG_LOADER EQ 1
		call p_fat_loadBuffDebug1 
	ENDIF	
	
	; agora AX,DX foram somados a um offset
	; que pode ser duma partição

	; por os totais
	; BL=tS BH=tH SI=tC
	mov si, ds:[fatwork_param_TC]
	mov bh, ds:[fatwork_param_TH]
	mov bl, ds:[fatwork_param_TS]
	push ds
		call p_lba_to_chs
	pop ds
	; CX e DH tem o CHS

	mov dl, ds:[fatwork_param_DRIVE]

	mov bx, di		; BX=DI destino
	mov ax, 0201h	; AH=ler AL=1 setor
	IF FAT_DEBUG_LOADER EQ 1
		call p_fat_loadBuffDebugCHS
	ENDIF
	INT 13h
	jc p_fat_loadBuffError

	IF FAT_DEBUG_LOADERBUFFER EQ 1
		call p_fat_loadBuffDebug3
	ENDIF

	IF FAT_DEBUG_LOADER EQ 1
		push ax
			xor ax, ax
			INT 16h
		pop ax
	ENDIF

	p_fat_loadBuffError:
M_pop6 ax, bx, cx, dx, ds, si
ret

;-----------------------------------------------------------------
; p_fat_loadCluster: entra DX:AX com o CLUSTER, carrega e avança ES:DI
p_fat_loadCluster:
M_push4 ax, bx, cx, dx

	M_mov ds, FAT_WORKSEG, bx
									; subtrair por 2, para possuir
									; o espaço que seria usado pelos 
									; primeiros 2º primeiros IDs 
									; (estes são reservados)
	sub ax, 2	
									; obter o shifter 
	mov cl, ds:[fatwork_allocShifter]
	or cl, cl
	jz p_fat_loadCluster1

	shl dx, cl						; shiftar DX:AX
	push cx							; para obter o setor
		mov bl, 16
		sub bl, cl
		mov cl, bl
		mov bx, ax
		shr bx, cl
		or dx, bx
	pop cx
	shl ax, cl

	p_fat_loadCluster1:
									; somar o inicio 
									; das alocacoes
					
	mov bx, ds:[fatwork_allocStartLB]
	xor cx, cx
	add ax, bx	; <dx,ax>+<bx=alloc>
	adc dx, cx 
							; repetir a cópia de 512B
							; pelo o multiplicador
							; cluster 1,2,8, etc
	xor ch, ch
	mov cl, ds:[fatwork_allocMul]
	p_fat_loadClusterCopy:
				
							; chamar apanhar 512B
		call p_fat_loadBuff

		xor bx, bx	; incrementar LB	
		inc ax
		adc dx, bx
		mov bx, es	; avançar ES por 512B
		add bx, 32
		mov es, bx

	loop p_fat_loadClusterCopy

M_pop4 ax, bx, cx, dx
ret