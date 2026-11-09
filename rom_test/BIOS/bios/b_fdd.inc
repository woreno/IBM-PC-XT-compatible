M_FDC_RX_STATUS MACRO LENGTH_
	push bx
	mov bh, LENGTH_
	call fdc_rx_status
	pop bx
ENDM
M_FDC_TX_CMD MACRO LENGTH_
	push bx
	mov bh, LENGTH_
	call fdc_tx_cmd
	pop bx
ENDM
M_FDC_SaveAt MACRO POS_, VALUE_
	mov al, VALUE_
	mov ds:[POS_], al 
ENDM

assume ds:DATA
;  1. p_fdd_reset		- reinicia unidade e desliga controlador, volta a ligar 
;  2. p_fdd_start		- ponto de redireção para as funções...
;  3. p_fdd_status		- devolve o estado nas variáveis
;  4. p_fdd_read		- ler
;  5. p_fdd_verify		- verificar
;  6. p_fdd_format		- formatar		
;  7. p_fdd_write		- escrever
;  8. p_fdd_rw			- ...executar um das 4 funções anteriores
;  9. p_fdd_cvt		- número de setores transf. (pode diferir, se erro)
; 10. p_fdd_seek		- reposicionar
; 11. p_fdd_dmasetup	- definir modo DMA, base e contador 
; 12. p_fdd_getparam	- indexar a tabela de unidades com bx, deixar em ah
; 13. p_fdd_intack		- confirmar ao FDC a interrupção
; 14. p_fdd_waitint	- esperar pela interrupção
; 15. p_fdd_res		- apanhar os resultados do FDC
; 16. p_fdd_nec_out	- emitir os comandos para o FDC
; 17. p_fdd_int		- tratador da interrupção hardware
;------------------------------------------------------
; 18. p_fdd_waitSeek
; 19. p_fdd_detect_start
; 20. p_fdd_detectFail
; 21. p_fdd_detect
;------------------------------------------------------
; 22. FDD_PARAMS

;------------------------------------------------------
; 2. p_fdd_start
;------------------------------------------------------
p_fdd_start:
	sti
	PUSH	BX	
	PUSH	CX
	PUSH	DS	
	PUSH	SI	
	PUSH	DI
	PUSH	BP
	PUSH	DX
	MOV	BP, SP	
	call ds2data
	call p_fdd_options
	MOV	BX,4				
	CALL p_fdd_getparam
	MOV	motor_count, AH
	MOV	AH, disq_status
	CMP	AH, 1				
	CMC			; complement carry		
	POP	DX	
	POP	BP
	POP	DI
	POP	SI
	POP	DS
	POP	CX
	POP	BX	
	M_RetFarWithout 2	
;------------------------------------------------------
; 1. p_fdd_reset
;------------------------------------------------------
p_fdd_reset:
	mov dx, FDC_DOR			
	cli
	mov al, motor_status	; bits de "ocupada"
	mov cl, 4				
	sal al, cl				; agora no nibble alto
	test al, 20h			; estão as unidades 
	jnz p_fdd_reset_j5		; com bit de "ocupada"
	test al, 40h			; aqui apenas se considera 
	jnz p_fdd_reset_j4		; haver 1 "ocupada"
	test al, 80h			; e passa-se o bit para
	jz  p_fdd_reset_j6		; para o nibble baixo
	p_fdd_reset_j4:			; essa posição para BCD
		inc al
	p_fdd_reset_j5:
		inc al
	p_fdd_reset_j6:
		or al, 8			; + interrupção/DMA ligado
		out dx, al			; al tem o comando de RESET, enviar
		mov seek_status, 0  ; indicar próx. vez, necessário recalibração
		mov disq_status, 0
		or al, 4            
		out dx, al			; al tem agora o comando de LIGAR, enviar
		sti					; ligar interrupções
		call p_fdd_intack  ; indicar confirmação de int
		mov al, nec_status
		cmp al, 0C0h		; ST0 indica erro?
		jz p_fdd_reset_ok
		or disq_status, BAD_NEC
		ret
	p_fdd_reset_ok:
							; comandos especificar
		mov ah, FDC_CMD_SPEC0  
		call p_fdd_nec_out
		mov bx, 1
		call p_fdd_getparam
		mov bx, 3
		call p_fdd_getparam
		ret
	
p_fdd_options:
	and motor_status, 07Fh  
	mov dh, al			 ; dh = número de setores
	or ah, ah            ; se AH=0 RESET
	jz p_fdd_reset		
	dec ah				 ; se AH=0 agora (era 1), STATUS
	jz p_fdd_status
	mov disq_status, 0   ; reset ao indicador
	cmp dl, 4            ; unidade 0-3 ?
	jae p_fdd_start_error ; se acima erro 
	dec ah               ; se AH=0 agora (era 2), LER
	jz p_fdd_read
	dec ah               ; se AH=0 agora (era 3), ESCREVER
	jz p_fdd_write
	dec ah               ; se AH=0 agora (era 4), VERIFICAR
	jz p_fdd_verify
	dec ah               ; se AH=0 agora (era 5), FORMATAR
	jz p_fdd_format
						 ; outra coisa erro
	p_fdd_start_error:
	mov disq_status, BAD_CMD
	ret

;------------------------------------------------------
; 3. p_fdd_status
;------------------------------------------------------
p_fdd_status:
	mov al, disq_status
	ret
	
;------------------------------------------------------
; 4. p_fdd_read
;------------------------------------------------------
p_fdd_read:
	mov al, 046h			; DMA: simples, canal 2, CPU<<DISP
	call p_fdd_dmasetup
p_fdd_readCont:
	mov ah, 0E6h			; comando para o FDC
	jmp p_fdd_rw

;------------------------------------------------------
; 5. p_fdd_verify
;------------------------------------------------------
p_fdd_verify:
	mov al, 42h  ; comando ao nec
	jmp p_fdd_readCont
	 
;------------------------------------------------------
; 6. p_fdd_format
;------------------------------------------------------
p_fdd_format:
	or motor_status, 80h	; indicar operação escrita
	mov al, 4Ah				; DMA: simples, canal 2, CPU>>DISP
	call p_fdd_dmasetup
	mov ah, 4Dh				; comando para o FDC
	jmp p_fdd_rw
p_fdd_rw_format:
	mov bx, 7
	call p_fdd_getparam
	mov bx, 9
	call p_fdd_getparam
	mov bx, 15
	call p_fdd_getparam
	mov bx, 17
	jmp p_fdd_rw_formatCont
	
;------------------------------------------------------
; 7. p_fdd_write
;------------------------------------------------------
p_fdd_write:
	or motor_status, 80h	; indicar operação escrita
	mov al, 4Ah				; DMA: simples, canal 2, CPU>>DISP
	call p_fdd_dmasetup
	mov ah, 0C5h			; comando para o FDC
							; continuar para rw...

;------------------------------------------------------
; 8. p_fdd_rw
;    ler/escrever/verificar
;------------------------------------------------------
p_fdd_rw:
	jnc p_fdd_rw_ok1	
	mov disq_status, DMA_BOUNDARY	; erro DMA se CARRY
	mov al, 0
	ret
p_fdd_rw_ok1:
	push ax			; guardar comando nec

	push cx			; guardar pista/setor
	mov cl, dl		
	mov al, 1
	sal al, cl		; AL = bit ativo é unidade
	cli				
	mov motor_count, 0FFh  ; para o "contador de desligar"
						   ; não interferir por agora
	test al, motor_status  ; verificar se ON
	jnz	p_fdd_rw_nowait   ; se ligado, proceder
							
						   ; se nao ligado 
	and motor_status, 0F0h  ; indicar todos desligados
	or motor_status, al     ; e ligar o bit, do a ligar
	sti
	mov al, 10h			    ; ALh = indicar com bit
	sal al, cl				; motor ligado
	or al, dl				; ALl = unidade 
	or al, 0Ch				; controlador ligado e 
							; pedidos DMA

	push dx					; guardar unidade
	mov dx, FDC_DOR         ; DX=registo da FDC
	out dx, al				; emitir comando
	pop dx					; DL=unidade

	; *** esperar pelo o motor se é uma escrita

							; verificar se escrita
	test motor_status, 80h  ; bit alto=1
	jz p_fdd_rw_nowait
							; é escrita
	mov bx, 20				; apanhar o parametro
	call p_fdd_getparam	; de espera para a unidade
	or ah, ah
	p_fdd_rw_loop1:
		jz p_fdd_rw_nowait ; se zero saltar
		sub cx, cx			; senao esperar
		p_fdd_rw_loop2:
		loop p_fdd_rw_loop2
		dec ah
		jmp p_fdd_rw_loop1

	p_fdd_rw_nowait:
		sti
		pop cx

	; *** fazer a operação de reposicionamento
	call p_fdd_seek

	pop ax				; recuperar comando em ah
	mov bh, ah			; bh = comando
	mov dh, 0 
	jc p_fdd_rw_seekerror	; se falhou SEEK...

	mov si, offset p_fdd_rw_seekerror; <!>
	push si
						; emitir a parte comum
						; a todos os comandos

	call p_fdd_nec_out	; emitir comando r/w/v/f
		mov ah, [bp+1]			
		sal ah, 1
		sal ah, 1
		and ah, 4
		or ah, dl				; emitir comando
	call p_fdd_nec_out	; [  cabeça|unidade ]
		
		cmp bh, 04Dh		; é de formatar?
		jne p_fdd_rw_normal
		jmp p_fdd_rw_format

	p_fdd_rw_normal:
		mov ah, ch			; pista
		call p_fdd_nec_out 
		mov ah, [bp+1]		; cabeça
		call p_fdd_nec_out 
		mov ah, cl          ; setor
		call p_fdd_nec_out 
		mov bx, 7           ; bytes/setor (GET_PARAM)
		call p_fdd_getparam
		mov bx, 9			; EOT (ultimo setor fim da pista)
		call p_fdd_getparam
		mov bx, 11			; GAP3 em bytes
		call p_fdd_getparam
		mov bx, 13			; DTL
	p_fdd_rw_formatCont:
		call p_fdd_getparam
		pop si
		
		call p_fdd_waitint ; esperar por interrupção

	p_fdd_rw_seekerror:
		jc p_fdd_rw_interror
		call p_fdd_res
		jc p_fdd_rw_reserror

		cld 
		mov si, offset nec_status
		lodsb 				; al=ds:[si], si++
		and al, 0C0h		; algum erro?
		jz p_fdd_rw_ok 
		cmp al, 040h		
		jne	p_fdd_rw_bad ; não anormal, mas mal, saltar
		
			; anormal porque...				
			; porque nao consigo acabar ou porque mudou estado READY?
			; consultar ST1 
		lodsb				
		sal al, 1
		mov ah, RECORD_NOT_FND
		jc p_fdd_rw_abn; bit 7 (pista alem da final)
		sal al, 1
		sal al, 1		; bit 5 (CRC mau)
		mov ah, BAD_CRC
		jc p_fdd_rw_abn
		sal al, 1		; bit 4 (não consegue acompanhar
		mov ah, BAD_DMA ;        ritmo da transferência)
		jc p_fdd_rw_abn
		sal al, 1
		sal al, 1		; bit 2 (nao setor ou ID nao consegue
		mov ah, RECORD_NOT_FND ; ser lido)
		jc p_fdd_rw_abn
		sal al, 1		; bit 1 (protegido-contra-escrita)
		mov ah, WRITE_PROTECT
		sal ah, 1		; bit 0 (nao encontrou ID)
		mov ah, BAD_ADDR_MARK
		jc p_fdd_rw_abn
		 
	p_fdd_rw_bad:	
		mov ah, BAD_NEC
	p_fdd_rw_abn:
		or disq_status, ah
		call p_fdd_cvt ; quantos setores transferidos?
	p_fdd_rw_reserror: 
		ret
	p_fdd_rw_interror:
		call p_fdd_res
		ret
	p_fdd_rw_ok:
		call p_fdd_cvt ; quantos setores transferidos?
		xor ah, ah			 ; sem erro
		ret 

;------------------------------------------------------
; 9. p_fdd_cvt
;    calcula o número de setores que foram transferidos
;	 entra:
;     CH = pista 
;     CL = setor
;    sai:
;     al = número de setores transferidos
;------------------------------------------------------
p_fdd_cvt:
	mov al, nec_status+3 ; al = pista
	cmp al, ch			 ; 
	mov al, nec_status+5 ; al = último setor
	je p_fdd_sametrack  ; reside na mesma pista? 
				
						 ; senão reside na mesma pista,

	mov bx, 8 
	call p_fdd_getparam ; EOT
	mov al, ah			 ; al = setor final duma pista
	inc al				 ; al = número de setores/pista

	p_fdd_sametrack:    ; se reside na mesma pista
		sub al, cl		 ; simplesmente subrair final-inicial
		ret				 ; senão (EOT+1)-setor
	
	; <!> aqui se nec_status diz que não reside na mesma pista 
	; onde começou, só se devolve o número de setores
	; que resta até ao fim da pista (e não até nec_status+5)
	; no momento que escrevo isto não me lembro se se pode
	; cruzar pistas?

	
;------------------------------------------------------
; 10. p_fdd_seek
;------------------------------------------------------
p_fdd_seek:
	mov al, 1
	push cx
	mov cl, dl 
	rol al, cl		; AL=bit com a unidade 
	pop cx
	test al, seek_status ; se zero, recalibrar!
	jnz p_fdd_seek_norecal

	or seek_status, al	 ; deixar bit que já 
						 ; fez calibração

	mov ah, FDC_CMD_RECALIBRATE 
	call p_fdd_nec_out
	mov ah, dl
	call p_fdd_nec_out
	call p_fdd_intack
	jc p_fdd_seek_error

	; recalibração está feita 
p_fdd_seek_norecal:
	mov ah, FDC_CMD_SEEK
	call p_fdd_nec_out
	mov ah, dl					; unidade
	call p_fdd_nec_out
	mov ah, ch					; pista
	call p_fdd_nec_out
	call p_fdd_intack

	; aguardar que a cabeça se estabilize
	pushf
	mov bx, 18
	call p_fdd_getparam  ;  ah=espera
	push cx

p_fdd_seek_loop1:
	mov cx, 550			; 1 ms
	or ah, ah
	jz p_fdd_seek_ready
	p_fdd_seek_loop2:
		loop p_fdd_seek_loop2
		dec ah
		jmp p_fdd_seek_loop1

p_fdd_seek_ready:
	pop cx
	popf 
p_fdd_seek_error:
	ret 
	
	
;------------------------------------------------------
; 11. p_fdd_dmasetup
;     entra: 
;      AH = modo DMA
;------------------------------------------------------
p_fdd_dmasetup:
	push cx
	cli
	out DMA_REG16_RESET, al
	push ax
	pop ax
	out DMA_MODES, al	; definir MODO

				; apanhar em ES:BX
				; e dividir o endereço
				; para a latch de bits altos
				; e para o contador DMA
	mov ax, es 
	mov cl, 4 
	rol ax, cl   ; ax = es<<4
	mov ch, al	 ; ch tem os bits altos
	and al, 0F0h ; ax tem o endereço fisico de 16bits
	add ax, bx   ; somar o offset
	jnc p_fdd_dmasetup_nc ; se houver carry, 
				 ; tem de se incrementar 
				 ; os bits altos
	inc ch

	p_fdd_dmasetup_nc:
		push ax		; guardar end. fisico baixo final
					; emitir a base
		out DMA_CHAN2_ADDR, al
		mov al, ah
		out DMA_CHAN2_ADDR, al

					; emitir os bits de pagina
		mov al, ch
		and al, 0Fh
		out DMA_CHAN2_PAGE, al 

		mov ah, dh		; ah = dh = número de setores
		xor al, al		;      
		shr ax, 1		; ax = nº * 256 / 2 = nº 128
		push ax			; guardar

		mov bx, 6       ; apanhar o parametro de bytes/setor
		call p_fdd_getparam	
		mov cl, ah      ; ah = tem o deslocamento 
		
		pop ax			; recuperar

		shl ax, cl		; agora corrigir, ax=nº bytes
		dec ax			; decrementar 1, como manda o DMA
		push ax         ; guardar o contador DMA
		out DMA_CHAN2_CNT, al
		mov al, ah 
		out DMA_CHAN2_CNT, al 
		sti				; ligar interrupções
		pop cx			; recuperar o contador DMA
		pop ax			; recuperar endereço fisico baixo
		add ax, cx		; SOMAR os DOIS se CARRY 
		pop cx			; DOIS ultrapassou um bloco 64KB!
		mov al, 2
		out DMA_MASK_CHAN, al ; ativar canal
		ret 
		

;------------------------------------------------------
; 12. p_fdd_getparam
;     entra: bx = indice do byte * 2 a apanhar
;		          se bx impar (MSB=1), ESSE valor
;                 é enviado logo
;      sai: ah = byte
;------------------------------------------------------
p_fdd_getparam:
	push ds
	xor ax, ax
	mov ds, ax
	assume ds:ABS0
			; o que houver em disq_pointer
			; está agora em DS:SI
		lds si, disq_pointer 
		shr bx, 1			; dividir
		mov ah, ds:[si+bx]	; colocar
		pop ds
	assume ds:DATA
		call p_fdd_nec_out ; enviar, se bx impar
		ret
		
;------------------------------------------------------
; 13. p_fdd_intack
;     confirma a interrupção
;------------------------------------------------------
p_fdd_intack:
	M_push2 si,di
	call p_fdd_waitint
	jc p_fdd_intack_badflg
	mov ah, FDC_CMD_SENSE	; comando de confirmar interrupção
	call p_fdd_nec_out1
	jc p_fdd_intack_badflg	
	call p_fdd_res			; resultados..
	jc p_fdd_intack_exit
	and al, 060h
	cmp al, 060h
	jz p_fdd_intack_badflg
	clc
	jmp p_fdd_intack_exit
		
	p_fdd_intack_badflg:
		or disq_status, BAD_SEEK
		stc
	p_fdd_intack_exit:
		;IF BUILD_BIOS_FDD_DEBUG  EQ 1
		;	call tty_err
		;ENDIF
		M_pop2 si,di
		ret
		
;------------------------------------------------------
; 14. p_fdd_waitint
;     espera pela interrupção
;------------------------------------------------------
p_fdd_waitint:
	sti
	M_push2 cx, bx
	mov bl, 2		; este ciclo espera pela INT
	xor cx, cx		; que vai ser indicada em 
					; seek_status; em qq caso
					; cx+bl fazem um timeout de ~2sec
	p_fdd_waitint_loop1:	
		test seek_status, INT_FLAG
		jnz p_fdd_waitint_exit_ok	; se for 1 sair
		loop p_fdd_waitint_loop1	; dec cx

		dec bl						; dec bl
		jnz p_fdd_waitint_loop1
		or disq_status, TIME_OUT	; passou o timeout: 
		stc							; indicar em seek_status
									; e carry
	p_fdd_waitint_exit_ok:
		clc
	p_fdd_waitint_exit:
		pushf
			and seek_status, NOT INT_FLAG 
			; retirar a indicação de interrupção
		popf 
		M_pop2 cx, bx
		ret
		
;------------------------------------------------------
; 15. p_fdd_res
;     lê todos os registos de STATUS depois duma operação
;	  sai: 
;       CF = falha
;------------------------------------------------------
p_fdd_res:
	cld
	mov di, offset nec_status
	M_push3 cx, dx, bx 
	mov bl, 7   ; máximo de bytes de estado
	p_fdd_res_sloop1:
		xor cx, cx				; para timeout
		mov dx, FDC_STATUS
	p_fdd_res_loop1:
		in al, dx
		test al, 80h			; registo de dados?
		jnz p_fdd_res_ok1
		loop p_fdd_res_loop1
		or disq_status, TIME_OUT
	p_fdd_res_error:
		stc 
		M_pop3 cx, dx, bx
		ret 
	p_fdd_res_ok1:
		in al, dx
		test al, 040h			; direção 1? CPU<<FDC?
		jnz p_fdd_res_ok2
	p_fdd_res_notready:
		or disq_status, BAD_NEC
		jmp p_fdd_res_error
	p_fdd_res_ok2:
		inc dx					; agora dx=registo de DADOS 
		in al, dx
		mov [di], al			; ds:[di] tem o byte
		inc di					; próxima posição		
		mov cx, 10
	p_fdd_res_loop3:			; fazer tempo...
		loop p_fdd_res_loop3
		dec dx					; repor no registo de STATUS
		in al, dx
		test al, 010h			; algum comando a decorrer ainda
		jz p_fdd_res_done		; (existem mais resultados?)
		dec bl
		jnz p_fdd_res_sloop1	; se algum comando a decorrer
		jmp p_fdd_res_notready	; e já passaram 7 bytes, erro
	p_fdd_res_done:
		M_pop3 cx, dx, bx
		ret
		
;------------------------------------------------------
; 16. p_fdd_nec_out
;     manda uma byte depois de testar se
;     o sentido é CPU>>FDC e o registo de dados
;     pode aceitar dados
;     entra:
;      AH = byte a enviar
;     sai:
;      CF = falha
;------------------------------------------------------
p_fdd_nec_out:
	M_push2 cx, dx
	mov dx, FDC_STATUS
	xor cx, cx				; para timeout
	p_fdd_nout_loop1:
		in al, dx
		test al, 040h		; sentido 0? (CPU>>FDC)?
		jz p_fdd_nout_ok
		loop p_fdd_nout_loop1	; se sentido 1 ficar no loop1

	p_fdd_nout_timeout:
		or disq_status, TIME_OUT
		M_pop2 cx, dx
		pop ax				; <!!!!> cortar endereço de regresso
		ret

	p_fdd_nout_ok:
		xor cx, cx			; para timeout

	p_fdd_nout_loop2:
		in al, dx
		test al, 80h		; se ocupado ficar no loop2
		jnz p_fdd_nout_ok2
		loop p_fdd_nout_loop2
		jmp p_fdd_nout_timeout

	p_fdd_nout_ok2:
		mov al, ah
		mov dx, FDC_DATA	; enviar comando
		out dx, al        
		M_pop2 cx, dx
		ret 
							; wrapper porque p_fdd_nec_out
p_fdd_nec_out1:				; pode retroceder uma funcao por stack
	call p_fdd_nec_out
ret

p_fdd_nec_outs:		; manda o que houver em es:[di] 
	M_push4 ax, cx, si, di
		xor ch, ch
		mov cl, es:[di] ; tamanho a enviar
		inc di

		p_fdd_nec_outs_:
			mov ah, es:[di]
			call p_fdd_nec_out1
			jc p_fdd_nec_outs_exit
			inc di
		loop p_fdd_nec_outs_
		clc
		p_fdd_nec_outs_exit:
	M_pop4 ax, cx, si, di
ret
		
;------------------------------------------------------
; 17. p_fdd_int
;     tratador da interrupção de hardware - disquete
;     ORG (0EF57H-08000h)
;------------------------------------------------------
p_fdd_int:
	sti
	push ds
	push ax
	call ds2data	
	assume DS:DATA
	or seek_status, INT_FLAG  ; deixar apenas a flag
	mov al, PIC_CMD_EOI      ; de interrupção, eoi
	out PIC0_L0, al
	assume DS:CODE
	pop ax
	pop ds
	iret

;---------------------------------------------------------------
; 18. p_fdd_waitSeek
p_fdd_waitSeek:
	mov dx, FDC_STATUS
	in al, dx
	test al, 1
	jnz p_fdd_waitSeek
ret 

	nec_cfg0    db 4, FDC_CMD_CONFIG,0,FDC_CONFIG,0
	nec_cfg1    db 3, FDC_CMD_SPEC0, FDC_CMD_SPEC1, FDC_CMD_SPEC2
	nec_sense   db 1, FDC_CMD_SENSE
	nec_recal0  db 2, FDC_CMD_RECALIBRATE, 0
	nec_recal1  db 2, FDC_CMD_RECALIBRATE, 1
	nec_repos0  db 3, FDC_CMD_SEEK, 0, 55
	nec_repos1  db 3, FDC_CMD_SEEK, 1, 55
	nec_end0	db 3, FDC_CMD_SEEK, 0, 0
	nec_end1	db 3, FDC_CMD_SEEK, 1, 0
	nec_startup dw offset nec_recal0, offset nec_repos0,  offset nec_end0,  offset nec_recal1, offset nec_repos1, offset nec_end1  


	FDC_DEBUGSTEP MACRO CHAR_
		IF BUILD_BIOS_FDD_DEBUG  EQ 1
			mov al, '0'+CHAR_
			call tty_char
		ENDIF
		call tty_dot
	ENDM



;---------------------------------------------------------------
; 19. p_fdd_detect_start
p_fdd_detect_start:
; imprimir FDDx
push ds
push si
	call tty_crlf
	call tty_sp
	call ds2cs
	mov si, offset str_fdd
	call tty_str
	mov al, cl
	call tty_bit
	mov al, ':'
	call tty_char
	call tty_saveCur
pop si
pop ds
push cx
	mov dx, FDC_DOR		   ; ligar motor/sem dma
	mov al, FDC_DOR_ENABLE ; 4 ms são precisos até ligar
	inc cl
	shl cl, 4				; ligar o motor em CL
	or al, cl
	out dx, al 
	call longlongWait
	call longlongWait
	;mov al, FDC_DOR_ENABLE 
	out dx, al 
pop cx
ret

;---------------------------------------------------------------
; 20. p_fdd_detectFail
p_fdd_detectFail:
	call tty_restoreCur
	call tty_crlf
	call tty_cleanLineFromCur
	call ds2cs
	mov si, offset str_fdd
	call tty_str
	mov al, ':'
	call tty_char
	call tty_sp
	mov si, offset str_nocontroller
	call tty_str
ret
;---------------------------------------------------------------
; 21. p_fdd_detect
p_fdd_detect:
	mov ax, BIOS_SEG
	mov es, ax
	assume DS:DATA
	call ds2data
	mov fdd_number, 0
	assume DS:CODE
	
	call tty_saveCur
	call tty_crlf
	call tty_sp

	sti
	M_IRQ_ON FDC_IRQ

	; INICIALIZAÇÃO ----------------------------------------
	; 1.RESET > 2.CCR/DSR > 3. WAIT INT, SENSEINT > 4.CONFIG > 5.SPECIFY

	FDC_DEBUGSTEP 1

	; 1. RESET ----------------------------------------------
	mov dx, FDC_DOR			
	mov al, FDC_DOR_RESET		; reset, sem motores/sem dma
	out dx, al 
	mov al, FDC_DOR_ENABLE		; ligar, sem motores/sem dma
	out dx, al 
	call smallWait

	; 2.CCR/DSR --------------------------------------------
	FDC_DEBUGSTEP 2
	
	mov dx, FDC_DSR			; taxa de dados 
	mov al, FDC_DATARATE
	out dx, al 

	; 3. WAIT INT, SENSEINT --------------------------------
	FDC_DEBUGSTEP 3
	call p_fdd_intack
	jc p_fdd_detectFail
	call smallWait
	
	; 4.CONFIG ---------------------------------------------
							; reposicionamento auto.
							; usar pooling, usar FIFO, 
							; interromper aos 15 bytes
	FDC_DEBUGSTEP 4
	mov di, offset nec_cfg0
	call p_fdd_nec_outs
	jc p_fdd_detectFail

	; 5.SPECIFY --------------------------------------------
						    ; especificar, nao usar DMA e   
						    ; tempos para a montagem da cabeça
	FDC_DEBUGSTEP 5
	mov di, offset nec_cfg1
	call p_fdd_nec_outs
	jnc p_fdd_detect1 
	
		jmp p_fdd_detectFail
	 
	p_fdd_detect1:
	call tty_restoreCur
	call tty_crlf
	call tty_cleanLineFromCur
	call tty_restoreCur
	
	; INICIALIZAÇÃO FEITA ----------------------------------
	mov cx, 0
	mov si, offset nec_startup
	p_fdd_ddrives:
	push cx

		call p_fdd_detect_start
		
		FDC_DEBUGSTEP 6
		mov di, offset es:[si]				 ; recalibrar
		call p_fdd_nec_outs
		jc p_fdd_ddrive_no
		call p_fdd_intack
		jc p_fdd_ddrive_no

		call p_fdd_waitseek
		
		FDC_DEBUGSTEP 7
		mov di, offset es:[si+2]			; reposição
		call p_fdd_nec_outs
		jc p_fdd_ddrive_no
		call p_fdd_intack
		jc p_fdd_ddrive_no
		
		call p_fdd_waitseek
		
		FDC_DEBUGSTEP 8
		mov di, offset es:[si+4]			; reposição
		call p_fdd_nec_outs
		jc p_fdd_ddrive_no
		call p_fdd_intack
		jc p_fdd_ddrive_no

		call tty_restoreCur
		call tty_cleanLineFromCur
		mov ax, DISQ_SECTORS
		mov cx, DISQ_CILS
		mov dx, DISQ_HEADS
		call tty_CHS_normal
		pop cx
		push cx
			assume DS:DATA
			call ds2data
			inc fdd_number
				inc cl
				or drives_map, cl
			assume DS:CODE
			
		p_fdd_ddrive_next:
			pop cx
			add si, 6
			inc cx
			cmp cx, 2
			jne p_fdd_ddrives
		jmp p_fdd_ddrives_end

		p_fdd_ddrive_no:
			push si
				call tty_restoreCur
				call tty_cleanLineFromCur
				call ds2cs
				call tty_sp
				mov si, offset str_notdetected
				call tty_str
			pop si
			jmp p_fdd_ddrive_next

	p_fdd_ddrives_end:
	mov dx, FDC_DOR			; reset, sem motor/sem dma
	mov al, 0
ret

;---------------------------------------------------------------
; 22. FDD_PARAMS
FDD_PARAMS	LABEL	BYTE
	DB	11001111B	; SRT=C, HD UNLOAD=0F - 1ST SPECIFY BYTE
	DB	2			; HD LOAD=1, MODE=DMA - 2ND SPECIFY BYTE
	DB	MOTOR_WAIT	; WAIT AFTER OPN TIL MOTOR OFF
	DB	2			; 512 BYTES/SECTOR
	DB	17			; EOT (LAST SECTOR ON TRACK)
	DB	27			; GAP LENGTH
	DB	0FFh		; DTL
	DB	108			; GAP LENGTH FOR FORMAT
	DB	0F6h		; FILL BYTE FOR FORMAT
	DB	25			; HEAD SETTLE TIME (MILLISECONDS)
	DB	4			; MOTOR START TIME (1/8 SECONDS)