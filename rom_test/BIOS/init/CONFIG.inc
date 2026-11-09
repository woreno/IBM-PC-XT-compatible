
;////////////////////////////////////////////////////////////////////
; CONFIG GERAL

JUST_TESTS			 EQU 0  ; não faz o arranque e executa TESTS.INC
BUILD_BIOS_NOVIDEO 	 EQU 0	; se a 1, nem INT10 nem consola integrada
;
BUILD_BIOS_BOOT		 EQU 1  ; depois do resumo arranca o SO, ou faz HALT
BUILD_BIOS_CPUTEST	 EQU 0	; teste de FLAGS
BUILD_BIOS_DMATEST	 EQU 1	; teste de leitura/escrita nos 8 registos base e contador
BUILD_BIOS_PIC		 EQU 1	; Nº de PICs a configurar - se 0 nenhum
BUILD_BIOS_PORTS     EQU 1	; configura as portas COM, LPT e JOYSTICK
BUILD_BIOS_KEYB		 EQU 1	; deteta e instala teclado (INT16)
BUILD_BIOS_RTC		 EQU 1	; é possível testar RTC
	BUILD_BIOS_RTC_INFO  EQU 1  ; ... em caso de erro mostra os registos do RTC
BUILD_BIOS_MENU		 EQU 1	; é possivel entrar em MENU
BUILD_BIOS_SCANEXP	 EQU 0	; rastrea a expansão
BUILD_BIOS_VIDEO	 EQU 1	; INT10 (video) é instalada, caso contrário só básico (mais lento)
BUILD_BIOS_DISPLAY   EQU 1  ; os leds e digitos funcionam
BUILD_BIOS_IRQ0		 EQU 1  ; testar e instalar o temporizador (IRQ0)
BUILD_BIOS_FDD		 EQU 0	; é possível usar floppy
	BUILD_BIOS_FDD_DEBUG EQU 1
BUILD_BIOS_HDD		 EQU 1	; é possível usar discos IDE
BUILD_BIOS_SERIALINT EQU 1	; a INT14h é instalada
BUILD_BIOS_PLELINT   EQU 0	; a INT17h é instalada
BUILD_BIOS_USEVSYNC  EQU 1	; VSYNC é usado 
BUILD_BIOS_ADCVOL	 EQU 1  ; instalar o leitor dos potenciometros (IRQ15)

;////////////////////////////////////////////////////////////////////
; CHOCOLATE NO ARRANQUE

BUILD_BOOTSCR_DRAW      EQU 0 ; desenho no ecrã de boot
BUILD_BOOTSCR_CLOCK     EQU 0 ; desenha o relogio no ecrã de boot
BUILD_BOOTSCR_ANIM      EQU 0 ; animação no ecrã de boot
	BOOTSCR_CLOCK_ITS		EQU 4
	BOOTSCR_CLOCK_X			EQU 258
	BOOTSCR_CLOCK_Y			EQU 77
	BOOTSCR_CLOCK_BKCOLOR	EQU 3
	BOOTSCR_TALKING_CLOCK   EQU 0 ; fala a hora no boot screen
BUILD_TALKING_CLOCK	    EQU 0 ; fala a hora em modo de texto
	SPK_PARAMS_SEG_			EQU 50h	; onde alocar os parametros para reproduzir SOM das HORAS
BUILD_CHIME				EQU 0 ; toca um som no inicio

;////////////////////////////////////////////////////////////////////
; TESTES DE MEMÓRIA

 ; VRAM: teste
 TEST_VRAM               EQU 0
 ; RAM: teste, NO_DMA    significa que a atualizaçao DMA ainda não está em curso
 TEST_RAM_TOTAL			 EQU 512	; 256 512 576 640
 TEST_RAM_LINES          EQU 0		; testa RAS/CAS0,1,2,3
 TEST_RAM_NO_DMA         EQU 1      ; (sem DMA) testa escrita/verificacao imediata (DIP6)
 TEST_RAM_ENABLE_DMA     EQU 1      ; configura DMA refreshes (necessario as seguintes)
	TEST_RAM_DMA_PAUSE    EQU 0      ; quando ligar DMA refreshes, aguarda (DIP7)
					     
 TEST_RAM                EQU 1      ; (com DMA) escreve tudo, aguarda e verifica (DIP6)
	 TEST_RAM_USEBUTTON  EQU 0      ; nos passos anteriores, depois de escrever
						  		    ; esperar por carregar no botão em vez de aguardar
	 TEST_RAM_WAITTIME   EQU 16     ; (com DMA) quanto tempo esperar entre a escrita e leitura
 TEST_RAM_BYTE_BYTE      EQU 0      ; (com DMA) escreve tudo, aguarda e, 
                                    ;  ...mostra os bytes escritos passo-a-passo


	; até que coluna testar a RAM (um dos segmentos na array decima) 
IF TEST_RAM_TOTAL EQ 256 
	TEST_RAM_TOPSEG         EQU 04000h 
ENDIF
IF TEST_RAM_TOTAL EQ 512
	TEST_RAM_TOPSEG         EQU 08000h 
ENDIF
IF TEST_RAM_TOTAL EQ 576 
	TEST_RAM_TOPSEG         EQU 09000h 
ENDIF
IF TEST_RAM_TOTAL EQ 640 
	TEST_RAM_TOPSEG         EQU 0A000h 
ENDIF

;////////////////////////////////////////////////////////////////////
; TESTE IRQS

TEST_IRQ				EQU 0		; instalar tratadores para os IRQ, mostrando no visor o nº do IRQ
TEST_IRQ_ADDRESS		EQU 0		; ...mostra logo a seguir o endereço da tratador
									; (tem de se conhecer no código o endereço de cada tratador)

;////////////////////////////////////////////////////////////////////
; CONSOLA

TTY_MODE			    EQU 1 ; 0:40 / 1:80 colunas
TTY_MAKE_ROLLSOUND      EQU 0 ; som ao rolar TTY
TTY_ISCGA			    EQU 1 ; em DOSBOX=0

;////////////////////////////////////////////////////////////////////
; STEP

BUILD_SSTEP			 EQU 0	; se DIP0, ir passo-a-passo 
	BUILD_SSTEP_ONELINE	 EQU 1	; ..mostra na mesma linha
	BUILD_SSTEP_SKIP_DOS EQU 0  ; passar À frente da tentativa
								; do DOS de redefinir a INT1
BUILD_INTBIBLE		 EQU 0  ; as strings para as INTs
 
;////////////////////////////////////////////////////////////////////
; CHAMADA À INT13

INT13_ERRORS_ARE_FATAL EQU 1	
INT13_XSUMS			   EQU 0
INT13_TESTHDD		   EQU 0
INT13_LOG			   EQU 0
INT13_PEEKBUFFER	   EQU 0
INT13_STEP_AT          EQU 0FFFFh
INT13_STEP_WAIT		   EQU 0
INT13_DRAWMEM		   EQU 0
INT13_PRINT_ERRORS     EQU 1
INT13_VERBOSE_STATS	   EQU 0	; no registo de status
INT13_DEBUG_CHS        EQU 0    ; imprime CHS na operação
INT13_DEBUG_PARAMS	   EQU 0	; imprime a 1ª partição (MBR) tabelas DBT e INT13/AH=8
INT13_DEBUG_VISOR    EQU 1  ; poem no VISOR os setores q faltam

;////////////////////////////////////////////////////////////////////
; OUTROS

COM_DEF_TIMEOUT			 EQU 1	; timeouts para serie e paralela nas chamadas da BIOS para IO
LPT_DEF_TIMEOUT			 EQU 20	
RESOURCES_USES_FILE		 EQU 0  ; em DOSBOX=1 os recursos estão na ROM (não num ficheiro)
TEST_MENU                EQU 0	; em DOSBOX=1 apenas testar menus
RTC_DEBUG				 EQU 0  ; em DOSBOX=1 substitui RAM do RTC por outra RAM
