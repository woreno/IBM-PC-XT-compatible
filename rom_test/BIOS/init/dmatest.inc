
; escrever os registos de contadores
; e de endereço base (apenas 8-bit)
; do DMA, com todos os valores desde 
; FF a 0; os registos são lidos
; e confrontados com o valor escrito..
out DMA_RESET, al	; reset ao DMA
M_WaitDMA

; na BIOS original só faz FF.
; for (i=padrao) 
; {
;		BX=AL|AL
;       CX=8
;       DX=1º reg
;    for (j=registo)
;		
xor al, al
xor dh, dh
p_dma_nextPatt:
	mov bl, al		; padrao = BX=[AL|AL]
	mov bh, al
	mov cx, 8
	mov dl, DMA_CHAN0_ADDR
	p_dma_nextReg:
	
		out DMA_REG16_RESET, al
		M_WaitDMA
		out dx, al		 ; dx=BAIXO
		M_WaitDMA		 ; dummy
		out dx, al		 ; dx=ALTO (igual)
		M_WaitDMA		 ; dummy
		
		out DMA_REG16_RESET, al
		M_WaitDMA
		xor al, al
		in al, dx		
		mov ah, al	  
		xor al, al
		in al, dx		  
	
		cmp bx, ax		; se padrao igual
		je p_dma_cont

		mov cx, ERROR_DMA_TEST
		call p_dma_error
			
		p_dma_cont:
					
		inc dl			; próximo reg dma		
		loop p_dma_nextReg

						; se acabaram os registos
						; proximo padrao
		; senao incrementa o padrao
		; se chegou a zero tudo OK
	inc al
	jnz p_dma_nextPatt

call p_dma_ok


	; cx: entra o código de erro
p_dma_error:
	mov bp, dx
	mov si, ax
	mov di, bx
	mov sp, cx
	;M_dispERRORStr 0, "Erro leitura/escrita registos DMA"
p_dma_error_:
	mov cx, sp
	M_dispERROR cl
	M_DispLed 0
	mov ax, bp
	M_DispDigits16 ax, dx ; porta
	wait
	M_DispLed 1 
	mov ax, di
	M_DispDigits16 ax, dx ; padrao escrito
	M_Countdown cx
	wait
	M_DispLed 2 
	mov ax, si
	M_DispDigits16 ax, dx ; padrao lido
	M_Countdown cx
	wait
	p_dma_error1:
		mov cx, sp
		M_dispERROR cl
	call p_dma_error_
p_dma_ok:
