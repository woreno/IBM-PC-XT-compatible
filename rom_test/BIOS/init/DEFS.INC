;=============================================================
; Definicoes
; 1. Mneumonicas para unidades
; 2. Códigos para erros 
; 3. Constantes e Macros para FLAGS do 8088/86 
; 4. Flags de Equipamento
; 5. IRQ integrados
; 6. Portas I/O, IRQ e constantes dos integrados
;	6.1. NMI
;	6.2. DMA: Direct Memory Access Controller
;	6.3. PIC: Programable Interruption Controller
;	6.4. PPI: Peripheral Port Interface + scancodes teclado
;	6.5. PIT: Programable Interval Timer 
; 7. Portas IO, IRQs e constantes das placas de expansão
;	7.1. Adlib
;	7.2. Digitos e Leds
;	7.3. Paralela, Série e Joystick
;	7.5. RTC: Real Time Clock
;	7.6. MCRT + CGA
;	7.7. Controlador FDD
;=============================================================
;=============================================================
; 1. Mneumonicas para unidades
;=============================================================
UNIT_1K			 EQU 0400h
UNIT_2K			 EQU 0800h
UNIT_4K			 EQU 1000h
UNIT_8K			 EQU 2000h
UNIT_16K		 EQU 4000h
UNIT_32K		 EQU 8000h

UNIT_IN_SEGS_32B  EQU   02h
UNIT_IN_SEGS_1K   EQU  040h
UNIT_IN_SEGS_2K   EQU  080h
UNIT_IN_SEGS_4K   EQU  100h
UNIT_IN_SEGS_8K   EQU  200h
UNIT_IN_SEGS_16K  EQU  400h
UNIT_IN_SEGS_32K  EQU  800h
UNIT_IN_SEGS_64K  EQU 1000h
UNIT_IN_SEGS_128K EQU 2000h
UNIT_IN_SEGS_256K EQU 4000h

;=============================================================
; 2. Códigos para erros 
;=============================================================
ERROR_CPU		 EQU 0		; jumps condicionais do CPU
ERROR_XSUM		 EQU 1		; checksum da ROM
ERROR_RAM		 EQU 2		; RAM
ERROR_RAM_DMA	 EQU 3		; RAM com DMA refreshes
ERROR_VRAM		 EQU 4		; 16KB da CGA
ERROR_STACK		 EQU 5		; pequeno teste à STACK
ERROR_DMA_TEST	 EQU 0Ah	; escrita/leitura dos registos
ERROR_DMA_CFG	 EQU 0Bh	; se a configuração nao ficou igual
ERROR_DMA_TC	 EQU 0Ch	; erro se TC não foi emitido após 1 segundo
ERROR_DIVIDE	 EQU 0Dh	; divide error
ERROR_NMI		 EQU 0Eh	; nmi

;=============================================================
; 3. Constantes e Macros para FLAGS do 8088/86 
;=============================================================
x86bit_CF EQU 0
x86bit_PF EQU 2
x86bit_AF EQU 4
x86bit_ZF EQU 6
x86bit_SF EQU 7
x86bit_TF EQU 8
x86bit_IF EQU 9
x86bit_DF EQU 10
x86bit_OF EQU 11

x86_CF EQU (1 SHL x86bit_CF)
x86_PF EQU (1 SHL x86bit_PF)
x86_AF EQU (1 SHL x86bit_AF)
x86_ZF EQU (1 SHL x86bit_ZF)
x86_SF EQU (1 SHL x86bit_SF)
x86_TF EQU (1 SHL x86bit_TF)
x86_IF EQU (1 SHL x86bit_IF)
x86_DF EQU (1 SHL x86bit_DF)
x86_OF EQU (1 SHL x86bit_OF)

M_x86_enableFlags MACRO ARG_
	pushf			; F na STACK
	pop ax			; AX=F
	or ax, ARG_		; AX=F | ...
	push ax			; AX na STACK
	popf			; pop, F=AX 
ENDM
M_x86_disableFlags MACRO ARG_
	pushf			; F na STACK
	pop ax			; AX=F
	and ax, NOT(ARG_)	; AX=F & ...
	push ax			; AX na STACK
	popf			; pop, F=AX 
ENDM

;=============================================================
; 4. Flags de Equipamento
;=============================================================
 
	EQUIP_BITFLAG_IPLDISK		EQU 0	; existem disquetes
	EQUIP_BITFLAG_NPU			EQU 1
	EQUIP_BITFLAG_BASERAM		EQU 2
		; 0: 16KB, 1:32KB, 2: 16KB, 3: 64KB ou mais (normal)
	EQUIP_BITFLAG_VIDEOMODE		EQU 5
		; 0: nao usado, 1:40x25 cores, 2: 80x25 cores, 3: 80x25 mono
	EQUIP_BITFLAG_DISKDRIVES	EQU 6	; 2 bits
		; 0: 1 unidade, 1: 2 unidades...
	EQUIP_BITFLAG_DMA			EQU 8	; 0 se instalado
	EQUIP_BITFLAG_SERIALPORTS	EQU 9	; 3 bits
	EQUIP_BITFLAG_JOYSTICKPORT	EQU 12
	EQUIP_BITFLAG_PLELPORTS		EQU 14

;=============================================================
; 5. IRQ integrados
;=============================================================

	TIMER_IRQ		 EQU 0
	KEYB_IRQ		 EQU 1
	CASCATE_IRQ		 EQU 2

;=============================================================
; 6. Portas I/O, IRQ e constantes dos integrados 
;=============================================================

;-------------------------------------------------------------
; 6.1. NMI: Non Maskable Interruption Switch
;-------------------------------------------------------------
	NMI_REG			   EQU 0C0h ; (originalmente A0)
	
;-------------------------------------------------------------
; 6.2. DMA: Direct Memory Access Controller
;-------------------------------------------------------------
	DMA_CHAN0_PAGE     EQU 083h ; apenas XT
	DMA_CHAN1_PAGE     EQU 083h
	DMA_CHAN2_PAGE     EQU 081h
	DMA_CHAN3_PAGE     EQU 082h

	DMA_CHAN0_ADDR		EQU 0
	DMA_CHAN0_CNT		EQU 1
	DMA_CHAN1_ADDR		EQU 2

	DMA_CHAN1_CNT		EQU 3
	DMA_CHAN2_ADDR		EQU 4
	DMA_CHAN2_CNT		EQU 5
	DMA_CHAN3_ADDR		EQU 6
	DMA_CHAN3_CNT		EQU 7

	; portas para todos os canais
	DMA_STATUS      EQU 8
	DMA_CMD		    EQU 8	; config. de um canal
	DMA_REQUEST	    EQU 9	; request DMA manual
	DMA_MASK_CHAN   EQU 10	; inibir um canal
	DMA_MODES       EQU 11	; todas as opções
	DMA_REG16_RESET EQU 12	; antes de escrever/ler registo 16-bit
						   
	DMA_RESET       EQU 13
	DMA_ERASEMASK   EQU 14
	DMA_MASK        EQU 15 ; inibir vários canais
						  ; (a mesma função que DMA_MASK_CHAN)
						  ; mas permite def. por bits
						  ; os canais a inibir
						  
;-------------------------------------------------------------
; 6.3. PIC: Programable Interruption Controller
;-------------------------------------------------------------
PIC_CMD_EOI EQU 020h
PIC0_L0     EQU 020h
PIC0_L1     EQU 021h
PIC1_L0     EQU 0A0h
PIC1_L1     EQU 0A1h
		    
; leitura
	; A = 0
PIC0_IRR    EQU PIC0_L0
PIC0_ISR    EQU PIC0_L0
PIC1_IRR    EQU PIC1_L0
PIC1_ISR    EQU PIC1_L0
	; A = 1
PIC0_IMR    EQU PIC0_L1
PIC1_IMR    EQU PIC1_L1

; escrita
	; A = 0
PIC0_ICW1    EQU PIC0_L0
PIC0_OCW2    EQU PIC0_L0
PIC0_OCW3    EQU PIC0_L0

PIC1_ICW1    EQU PIC1_L0
PIC1_OCW2    EQU PIC1_L0
PIC1_OCW3    EQU PIC1_L0

	; A = 1
PIC0_ICW2    EQU PIC0_L1
PIC0_ICW3    EQU PIC0_L1
PIC0_ICW4    EQU PIC0_L1
PIC0_OCW1    EQU PIC0_L1

PIC1_ICW2    EQU PIC1_L1
PIC1_ICW3    EQU PIC1_L1
PIC1_ICW4    EQU PIC1_L1
PIC1_OCW1    EQU PIC1_L1

;-------------------------------------------------------------
; 6.4. PPI: Peripheral Port Interface + scancodes teclado
;-------------------------------------------------------------
PPI_REG_CTRL       EQU 063h ; W		PORT_CTRL
PPI_REG_KEYB       EQU 060h ; R		PORT_A
PPI_REG_OPTS       EQU 061h ; R/W	PORT_B
PPI_REG_INFO       EQU 062h ; R		PORT_C

KEY_SCANCODE_ZERO    EQU 0Bh
KEY_SCANCODE_DIGIT   EQU 02h
KEY_SCANCODE_ESCAPED EQU 0E0h

KEY_SCANCODE_MINUS EQU   4Ah
KEY_SCANCODE_PLUS  EQU   4Eh
KEY_SCANCODE_UP    EQU   48h
KEY_SCANCODE_DOWN  EQU   50h
KEY_SCANCODE_ESC   EQU   01h
KEY_SCANCODE_ENTER EQU   1Ch

KEY_SCANCODE_M     EQU 32h
KEY_SCANCODE_V     EQU 2Fh
KEY_SCANCODE_L     EQU 26h
KEY_SCANCODE_C     EQU 2Eh
KEY_SCANCODE_I     EQU 17h
KEY_SCANCODE_J     EQU 24h
KEY_SCANCODE_P     EQU 19h


;-------------------------------------------------------------
; 6.5. PIT: Programable Interval Timer
;-------------------------------------------------------------

PIT_REG_COUNT0     EQU 040h ; W
PIT_REG_COUNT1     EQU 041h ; W
PIT_REG_COUNT2     EQU 042h ; W
PIT_REG_CTRL       EQU 043h ; W
PIT_REG_SPK        EQU PIT_REG_COUNT2

;=============================================================
; 7. Portas IO, IRQs e constantes das placas de expansão
;=============================================================

;-------------------------------------------------------------
; 7.1. Adlib
;-------------------------------------------------------------
ADLIB_ADDR		   EQU 388h
ADLIB_DATA		   EQU 389h

;-------------------------------------------------------------
; 7.2. Digitos e Leds
;-------------------------------------------------------------
; IRQ ao rodar os potenciometros
VOLADC_IRQ		   EQU  15
VOLADC_READ		   EQU  170h

; Display 4 x 7 segmentos
DISP_REG_0         EQU 2F0h
DISP_REG_1         EQU 2F1h
DISP_REG_2         EQU 2F2h
DISP_REG_3         EQU 2F3h

; Display 8 leds
LEDS_REG           EQU 2F7h
 
;-------------------------------------------------------------
; 7.3. Paralela, Série e Joystick
;-------------------------------------------------------------


; Paralela						    ; na board...
LPT1_IO			  EQU 378h			; botão ESQUERDA (parte de cima)  
LPT1_IRQ          EQU 5			    ; botão ESQUERDA (parte de baixo)

LPT_BASE          EQU LPT1_IO		
LPT_DATA          EQU (LPT_BASE+0)  ;R/W
LPT_STATUS        EQU (LPT_BASE+1)  ;R
LPT_CTRL          EQU (LPT_BASE+2)  ;R/W


									; na board...
COM1_IO			EQU 3F8h			; botão ESQUERDA (parte de cima)
COM1_IRQ		EQU 4				; botão DIREITA (parte de baixo)
COM2_IO			EQU 2F8h
COM2_IRQ		EQU 3

; Série
SERIAL_IRQ         EQU COM1_IRQ	

SERIAL_BASE	       EQU COM1_IO	
	SERIAL_FREQDIV_LO EQU (+0)
	SERIAL_FREQDIV_HI EQU (+1)
	SERIAL_RXTX	      EQU (+0)
	SERIAL_INTCFG     EQU (+1)
	SERIAL_FIFOCTL    EQU (+2)
	SERIAL_INTCAUSE   EQU (+2)
	SERIAL_DATAFMT    EQU (+3)
	SERIAL_MODEMCTL   EQU (+4)
	SERIAL_STATUS     EQU (+5)
	SERIAL_MSTATUS    EQU (+6)

;-------------------------------------------------------------
; 7.4. RTC: Real Time Clock
;-------------------------------------------------------------

RTC_ADDR          EQU 70h
RTC_DATA          EQU 71h

RTC_SEC					EQU   0    ;  |
RTC_AL_SEC				EQU   1    ;  |
RTC_MIN					EQU   2    ;  |
RTC_AL_MIN				EQU   3    ;  | ao escrever nestes registos
RTC_HOUR				EQU   4    ;  | usar decimal 
RTC_AL_HOUR				EQU   5    ;  > codificado
RTC_WDAY				EQU   6    ;  | em binário (BCD)
RTC_MDAY				EQU   7    ;  | 
RTC_MONTH				EQU   8    ;  | 
RTC_YEAR				EQU   9    ;  |
RTC_CENTURY				EQU   32h  ;  |
RTC_STATUS_A			EQU   0Ah
RTC_STATUS_B			EQU   0Bh
RTC_STATUS_C			EQU   0Ch
RTC_STATUS_D			EQU   0Dh
RTC_RAM					EQU	  0Eh
RTC_RAM_FIRST			EQU   RTC_RAM
RTC_RAM_LAST			EQU   3Fh
RTC_RAM_BOOT_PRI		EQU   RTC_RAM_FIRST+16
RTC_RAM_BOOTSCREEN		EQU   RTC_RAM_FIRST+17
RTC_RAM_TALKINGCLOCK	EQU   RTC_RAM_FIRST+18
RTC_RAM_IRQ14_BUTTON    EQU   RTC_RAM_FIRST+19
RTC_RAM_IRQ15_BUTTON    EQU   RTC_RAM_FIRST+20
RTC_RAM_DIAL_SHOWUP	    EQU   RTC_RAM_FIRST+21
RTC_RAM_IRQ				EQU   RTC_RAM_FIRST+22
RTC_RAM_INT13_SHOWUP	EQU   RTC_RAM_FIRST+23
RTC_RAM_COM1_IO			EQU   RTC_RAM_FIRST+24
RTC_RAM_COM1_IRQ		EQU   RTC_RAM_FIRST+25
RTC_RAM_COM2_IO			EQU   RTC_RAM_FIRST+26
RTC_RAM_COM2_IRQ		EQU   RTC_RAM_FIRST+27
RTC_RAM_LPT1_IO			EQU   RTC_RAM_FIRST+28
RTC_RAM_LPT1_IRQ		EQU   RTC_RAM_FIRST+29

RTC_RAM_FDD0			EQU   RTC_RAM_FIRST+30
RTC_RAM_FDD1			EQU   RTC_RAM_FIRST+31
RTC_RAM_FDD0R			EQU   RTC_RAM_FIRST+32
RTC_RAM_FDD1R			EQU   RTC_RAM_FIRST+33
RTC_RAM_HDD0			EQU   RTC_RAM_FIRST+34
RTC_RAM_HDD1			EQU   RTC_RAM_FIRST+35
RTC_RAM_MEMTEST			EQU   RTC_RAM_FIRST+36
RTC_RAM_MEMCPU			EQU   RTC_RAM_FIRST+37
; 
RTC_RAM_COOKIE1			EQU   RTC_RAM_FIRST+40
RTC_RAM_COOKIE2			EQU   RTC_RAM_FIRST+41
RTC_RAM_COOKIE3			EQU   RTC_RAM_FIRST+42
RTC_RAM_COOKIE4			EQU   RTC_RAM_FIRST+43
RTC_RAM_ROMISNEW		EQU   RTC_RAM_FIRST+44

RTC_RAM_WIFI_SSID		EQU   RTC_RAM_FIRST+45
RTC_RAM_WIFI_SSIDSz     EQU   (32+1)
 
RTC_RAM_WIFI_KEY		EQU   RTC_RAM_WIFI_SSID+RTC_RAM_WIFI_SSIDSz
RTC_RAM_WIFI_KEYSz		EQU	  (13+1)

RTC_RAM_WIFI_SERVER		EQU   RTC_RAM_WIFI_KEY+RTC_RAM_WIFI_KEYSz
RTC_RAM_WIFI_ISSET		EQU   RTC_RAM_WIFI_SERVER+4
;
RTC_RAM_COOKIE_SIGN1	EQU   0AA55h
RTC_RAM_COOKIE_SIGN2	EQU   01234h
;
RTC_STATUS_A_CFG		EQU  2Fh ; ativar RTC; 2Hz
RTC_STATUS_B_CFG		EQU  4Ah ; pino onda quadrada, BCD, entre outros

;-------------------------------------------------------------
; 7.6. MCRT + CGA
;-------------------------------------------------------------

MCRT_INDEXREG      EQU 3D0h
MCRT_DATAREG       EQU 3D1h
CGA_MODE           EQU 3D8h
CGA_PALETTE        EQU 3D9h
CGA_STATUS         EQU 3DAh
CGA_LPEN           EQU 3DBh
CGA_LPEN_RESET     EQU 3DCh

MDA_MCRT_INDEX     EQU 3B0h
MDA_MCRT_DATA      EQU 3B1h
MDA_MODE           EQU 3B8h
MDA_STATUS         EQU 3BAh
 
; Joystick
JOY_READ          EQU 201h ; R
JOY_RESET         EQU 201h ; W

; M6845
CRT_ADDRESS       EQU 3D0h
CRT_DATA          EQU 3D1h
TXT_SEG           EQU 0B800h

; CGA
CGA_MODE           EQU 3D8h
CGA_PALETTE        EQU 3D9h
CGA_SEG_ODD        EQU 0B800h
CGA_SEG_EVEN       EQU 0BA00h

;-------------------------------------------------------------
; 7.7. Controlador FDD
;-------------------------------------------------------------

DISQ_HEADS		   EQU 2
DISQ_CILS		   EQU 80
DISQ_SECTORS	   EQU 18

FDC_IRQ            EQU 6
FDC_STATUS_A       EQU 3F0h
FDC_STATUS_B       EQU 3F1h
FDC_DOR            EQU 3F2h
FDC_STATUS         EQU 3F4h ;R
FDC_DSR            EQU 3F4h	;W
FDC_DATA           EQU 3F5h
FDC_DIR            EQU 3F7h ;R
FDC_CCR            EQU 3F7h ;W
	; comandos
	FDC_CMD_RECALIBRATE EQU 7    ; mandar para a pista 0 = recalibrar
	FDC_CMD_CONFIG      EQU 13h  ; configurar	
		FDC_CONFIG      EQU 48h  
				; auto-seek, FIFO
				; pooling
				; interromper aos 8 bytes
	FDC_CMD_SENSE       EQU 8    ; para confirmar de interrupção
	FDC_CMD_SEEK        EQU 0Fh  ; reposicionar pista
	FDC_CMD_RSEEK_IN    EQU 0CFh ; reposicionar para dentro
	FDC_CMD_RSEEK_OUT   EQU 08Fh  ; reposicionar para fora
	FDC_CMD_RD          EQU 46h  ; leitura/MFM
	FDC_CMD_WR          EQU 45h  ; escrita/MFM
		FDC_GAP3_BYTES      EQU 1Bh
		FDC_SECT_BYTES      EQU 2   ; 2 = equivale a 512bytes
		FDC_SECTS_PER_TRK   EQU 18  ; setores por pista   |
		FDC_HEADS           EQU 2   ; lados			       > floppy HD
		FDC_TRACKS			EQU 80  ; pistas			  |
		;
	FDC_CMD_SPEC0       EQU 3 
	FDC_CMD_SPEC1       EQU 080h ;((STR<<4)|(HUT))
	FDC_CMD_SPEC2       EQU 0Bh  ;((HLT<<1)|(USES_DMA))
		;
	FDC_DOR_RESET     EQU  0Ch     ; desligar motores, com DMA
	FDC_DOR_ENABLE    EQU  0Ch     ; ligado, com DMA


	;FDC_DATARATE	  EQU  2       ; 720KB (250KBit/s)
	FDC_DATARATE	  EQU  0       ; 1.44MB (500KBit/s)

