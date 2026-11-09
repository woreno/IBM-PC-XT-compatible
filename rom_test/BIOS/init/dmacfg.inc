; registo de páginas a zero
xor al, al
out DMA_CHAN0_PAGE, al
out DMA_CHAN1_PAGE, al
out DMA_CHAN2_PAGE, al
out DMA_CHAN3_PAGE, al

; confirmar que DMA ligado
mov al, 4
out DMA_CMD, al
M_WaitDMA
	
; reset ao DMA
out DMA_RESET, al
M_WaitDMA
	
; canal 0:
	
	; <endereço base: 0>
	xor al, al
	out DMA_REG16_RESET, al
	M_WaitDMA
	out DMA_CHAN0_ADDR, al 
	M_WaitDMA
	out DMA_CHAN0_ADDR, al 
	M_WaitDMA

	; <a transferir: número de bytes-1>
	; o endereço efetivo (A0:19) se divide em Col Address (CAS)
	; e Row Address (RAS); Row Address é composto por 9 ou 8 linhas
	; dependendo se é uma coluna de chips de 256KBit ou de 64KBit.
	; no entanto os chips de 256KBit ao atualizar a 8ª linha 
	; atualizam a 9ª, pelo que basta iterar A0:A7 
	; (que se traduz iterar RA0:RA7 - 8 linhas de Row Address), 
	; assim sendo define-se uma transferencia total de 256 bytes 
	; (FF+1) desde a base 0 o que basta para atualizar todas os
	; chips (640KB)
	mov al, 0FFh
	out DMA_REG16_RESET, al
	M_WaitDMA
	out DMA_CHAN0_CNT, al
	M_WaitDMA
	out DMA_CHAN0_CNT, al
	M_WaitDMA
	
	; definir modo (geral)
	; simples (byte-a-byte)
	; incrementar endereço e voltar repetir
	; leitura (memória para dispositivo)
	mov al, 058h
	out DMA_MODES, al
	M_WaitDMA

	; inibir todos os canais
	; à expceção do canal 0 (atualização de memória)
	mov al, 11111110b
	out DMA_MASK, al
	M_WaitDMA
	xor al, al				; a mesma coisa
	out DMA_MASK_CHAN, al
	M_WaitDMA
	
	mov al, 0			; ligar e prioridade fixa
	out DMA_CMD, al
	M_WaitDMA

	call skip_dma_writetest			; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	IF BUILD_BIOS_DMATEST EQ 1

	; verificar se os valores escritos = lidos
		mov dx, DMA_CHAN0_ADDR
		mov bx, 0
		xor al, al
		out DMA_REG16_RESET, al
		in al, DMA_CHAN0_ADDR
		mov ah, al
		in al, DMA_CHAN0_ADDR
		xchg al, ah
		cmp bx, ax
		je dmacfg_next1
		
		mov cx, ERROR_DMA_CFG
		call p_dma_error 
	
	dmacfg_next1: 
		mov dx, DMA_CHAN0_CNT
		mov bx, 0FFFFh
		out DMA_REG16_RESET, al
		in al, DMA_CHAN0_CNT
		mov ah, al
		in al, DMA_CHAN0_CNT
		xchg al, ah

	cmp bx, ax
	je dmacfg_next2

		mov cx, ERROR_DMA_CFG
		call p_dma_error 

	dmacfg_next2:
ENDIF
	
skip_dma_writetest:			; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	

; PIT: definir contador 1, MSB, modo 0, binário
mov al, 054H 
out PIT_REG_CTRL, al  
;mov al, 9h	;
mov al, 12h	; 12h e o valor original / 66.7 KHz
;mov al, 16h	
out PIT_REG_COUNT1, al	


call skip_dma_TCtest			; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


; pool ao status
	M_Countdown cx		; esperar cerca de 1 segundo
	M_Countdown cx
	M_Countdown cx

	in al, DMA_STATUS
	test al, 1			; foi emitido TC0?
	jnz dmacfg_next3
		
		dmacfg_error3:
			M_dispERROR ERROR_DMA_TC
		jmp dmacfg_error3

dmacfg_next3:

skip_dma_TCtest:





