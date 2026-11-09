assume cs:code, ds:DATA

;  1. p_disk_io			- tratador interrupção software 13

p_disk_io:
	test dl, 80h
	jz p_disk_io_isFDD
		IF BUILD_BIOS_HDD EQ 1
			M_FarJump BIOS_SEG, p_hdd_start
		ELSE
			iret
		ENDIF
	p_disk_io_isFDD:
		IF BUILD_BIOS_FDD EQ 1
			M_FarJump BIOS_SEG, p_fdd_start
		ELSE
			iret
		ENDIF

;------------------------------------------------------
; 1. p_disk_io (serviço INT 13)
;  entrada:
;    AH = 0 reset 
;    AH = 1 ler para AL o resultado 
;         da última operação <diskette_status>
;	 AH = 2 ler setores
;    AH = 3 escrever setores 
;	 AH = 4 verificar 
;    AH = 5 formatar pista 
;	 AH = 6 formatar pista (com flags de BAD SECTOR)
; 
;   * Para formatar ES:BX aponta um conjunto de entradas
;     4 bytes na forma de <C,H,R,N> (N:bytes/setor se 2: 512)  
;	  é então obrigatório uma entrada por setor na pista. 
;   * Para formatar indicando BAD_SECTOR 
;	* Para as operações ler/escrever/verificar/formatar
;	  DL = unidade
;     DH = cabeça
;     CH = pista
;     CL = setor
;     AL = número de setores <máx. 8, não usada em formatar>
;     ES:BX = buffer origem/destino <não usada em verificar>
;
		  
BIOS_DBT_SPEC1		  EQU 0 ; nibble alto: taxa do step; nibble baixo: tempo desmontar a cabeca
BIOS_DBT_SPEC2		  EQU 1 ; bit 1=DMA ativo, restante tempo montar cabeca 
BIOS_DBT_WAITMOTOR	  EQU 2 ; ticks a esperar antes de parar o motor
BIOS_DBT_BYTESPERSECT EQU 3 ; bytes/setor  0=128 1=256 2=512 3=1024
BIOS_DBT_SECTPERCIL   EQU 4 ; setores/cilindro
BIOS_DBT_GAPLENGTH    EQU 5 ; bytes entre setores
BIOS_DBT_DATALENGTH   EQU 6 ; tamanho do setor (se dif. do anterior)
BIOS_DBT_FGAPLENGTH   EQU 7 ; tam. byte entre setores formatados
BIOS_DBT_FILLBYTE     EQU 8 ; para setores formatados
BIOS_DBT_HEADSETTLE   EQU 9 ; em ms. tempo de estabilizacao da cabeca
BIOS_DBT_MOTORSTART   EQU 10 ; em s. tempo de estabilizacao do motor ao arrancar
; 
BIOS_DBT2_START		   EQU (11+0)
BIOS_DBT2_CILS		   EQU (11+0)
BIOS_DBT2_HEADS        EQU (11+2)
BIOS_DBT2_RWC_CIL      EQU (11+3) ; em que pista começa a redução de currente para escrita
BIOS_DBT2_WPRC_CIL     EQU (11+5) ; em que pista começa a precompensação de escrita
BIOS_DBT2_ECC_MAXBURST EQU (11+7) ; em que pista começa a precompensação 
BIOS_DBT2_CTRLBYTE     EQU (11+8)
	; bit 7: desligar tentativas de acesso
	; bit 6: desligar ECC
	; bit 5: sempre a zero
	; bit 3: opção da drive


DISK_TEST_SEGMENT     EQU 07C0h	; o mesmo que o setor de arranque
str_ide_error				db "IDE erro", 0
str_ide_error_busy			db "timeout busy",0
str_ide_error_dataready     db "timeout data ready",0
str_ide_error_mem			db "erro mem",0
str_ide_error_r				db "leitura",0
str_ide_error_w				db "escrita",0
str_ide_error_invalid_func  db "funcao invalida",0
str_ide_error_notimpl       db "nao implementado",0

ide_error_strs		  dw offset str_ide_error_busy 
					  dw offset str_ide_error_dataready
					  dw offset str_ide_error_mem 
					  dw offset str_ide_error_r 
					  dw offset str_ide_error_w
					  dw offset str_ide_error_invalid_func
					  dw offset str_ide_error_notimpl

ide_error_busy           EQU 0
ide_error_dataready      EQU 1
ide_error_mem            EQU 2
ide_error_r				 EQU 3
ide_error_w				 EQU 4
ide_error_invalid_func   EQU 5
ide_error_notimpl        EQU 6


; portas I/O segundo "Chuck Mode"
IDE_BASE      EQU 1F0h

IDE_DATA	   EQU (IDE_BASE+00h)	; registo de 16-bit
IDE_SECTCOUNT  EQU (IDE_BASE+02h)	; outros de 8-bit
IDE_LBA_M      EQU (IDE_BASE+04h)
IDE_DRIVEHEAD  EQU (IDE_BASE+06h)
IDE_ERROR      EQU (IDE_BASE+08h)
IDE_LBA_L      EQU (IDE_BASE+0Ah)
IDE_LBA_H      EQU (IDE_BASE+0Ch)
IDE_CMD        EQU (IDE_BASE+0Eh)
IDE_SECTOR     EQU IDE_LBA_L
IDE_CIL_L      EQU IDE_LBA_M
IDE_CIL_H	   EQU IDE_LBA_H
IDE_FEATURES   EQU IDE_ERROR
IDE_STATUS     EQU IDE_CMD

IDE_INFOWRD_CIL				 EQU  (54*2)
IDE_INFOWRD_HEADS			 EQU  (55*2)
IDE_INFOWRD_SECTPERCIL		 EQU  (56*2)
IDE_INFODWRD_TOTALSECTS		 EQU  (57*2)
IDE_INFODWRD_TOTALLBASECTS   EQU  (60*2)
IDE_INFODWRD_FEATURES        EQU  (49*2)

;                                bit retry disable (em alguns)
;                                |    |
;                                V    V
IDE_CMD_CALIBRATE    EQU  00010000b
IDE_CMD_READ         EQU  00100000b ; x
IDE_CMD_READ_wECC    EQU  00100010b ; x
IDE_CMD_WRITE        EQU  00110000b ; x
IDE_CMD_WRITE_wECC   EQU  00110010b ; x
IDE_CMD_VERIFY       EQU  01000000b ; x
IDE_CMD_FORMATTRACK  EQU  01010000b
IDE_CMD_SEEK         EQU  01110000b
IDE_CMD_DIAGNOSTIC   EQU  10010000b
IDE_CMD_SETPARAM     EQU  10010001b
IDE_CMD_IDENTIFY     EQU  11101100b

;                         76543210  
IDE_ERROR_NDM		 EQU  10000000b ; data address mark not found
IDE_ERROR_NT0		 EQU  00100000b ; track 0 not found
IDE_ERROR_ABT		 EQU  00100000b ; abort
IDE_ERROR_NID		 EQU  00001000b ; id mark not found
IDE_ERROR_SLF		 EQU  10000000b ; error of slave (diagnostics) 
IDE_ERROR_MASTDIAG	 EQU  01111111b ; error of master(diagnostics) 
IDE_STATUS_BSY		 EQU  10000000b
IDE_DRVHD_DRIVE		 EQU  00010000b 
IDE_STATUS_RDY		 EQU  01000000b
IDE_STATUS_DRQ		 EQU  00001000b
IDE_STATUS_ERR		 EQU  00000001b

