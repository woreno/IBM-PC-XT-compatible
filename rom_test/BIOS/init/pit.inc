
 DELAY_SOUND          EQU 0CFFh
; --------------------------------------------------
; PIT - PIT_REG_CTRL - registo de controlo: 
;      bit 7:6            bit 5:4               bit 3:1           bit 0
;    [ id contador | forma de acesso   | modo contagem | codificao contador]
;                      1: apenas byte L   <vários>        0: binária
;                      2: apenas byte H                   1: bcd
;                      3: L depois H


; PIT contador 0: IRQ0 (temporização) <gate ativa>
; PIT contador 1: atualização DMA     <gate ativa>
; PIT contador 2: speaker

; --------------------------------------------------

; PIT contador 2, LSB+MSB, modo 3, binário
; modo 3: começa logo escrito o contador, periodico
;         inverte sinal a chegar metade do contador
mov al, 0B6h              ;10 | 11 | 011 | 0 -> B6
out PIT_REG_CTRL, al  

; PPI ativar som
M_SoundOn

; Varia frequencia 
mov dx, 02A6h
mov bx, 6
sound_test:
	mov ax, dx
	mov al, al             
	out PIT_REG_SPK, al
	mov al, ah
	out PIT_REG_SPK, al
	M_Countdown cx, DELAY_SOUND
	shl dx, 1
dec bx
jnz sound_test

mov ax, 02A6h
out PIT_REG_SPK, al
mov al, ah
out PIT_REG_SPK, al

M_SoundOff