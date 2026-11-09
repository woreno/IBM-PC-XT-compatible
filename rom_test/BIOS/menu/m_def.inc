; 1. Definições TECLADO
B_MENU_BACKCOLOR      EQU   1Fh
B_MENU_INPUTCOLOR	  EQU   70h
B_MENU_SELCOLOR		  EQU   2Fh
B_MENU_TITLECOLOR	  EQU   070h

B_OPTION_CALL		 EQU   0
B_OPTION_ENUM		 EQU   1
B_OPTION_BOOL		 EQU   2
p_menu_prnCalls: dw p_menu_prnCall,  p_menu_prnEnum, p_menu_prnBool

;-------------------------------------------------------------------------------
; 2. Definições de registos, por página
;   
;   1 pagina -> N registos
;		1 registo por ser CALL, BOOL ou ENUM
;   <tipo>, <nome>, 0, 
;	SE tipo...
;        B_OPTION_CALL: <chamada para impressao>, <chamada para abrir>
;        B_OPTION_BOOL: <numero do registo RTC>
;        B_OPTION_ENUM: <numero do registo RTC>, <N=numero de opcoes>
;						<enum1>,0,<enum2>,0,...<enumN>,0
; 
;	SE 80h+<tipo> faz uma linha de separação 
; 

;--------------------------------------------------------------------------------------
; PAGINA 1
bm01	db	B_OPTION_CALL, "Data/hora", 0 
		dw					offset rtc_printClock, offset p_menu_setClock 

bm02	db  B_OPTION_BOOL,		"Mostrar boneco no arranque", 0, RTC_RAM_BOOTSCREEN
bm03	db  B_OPTION_ENUM,		"Falar horas", 0, RTC_RAM_TALKINGCLOCK, 3
		db						"Nunca", 0,"No boneco de arranque", 0, "No resumo do sistema", 0, 0
bm04	db  B_OPTION_BOOL,		"Mostrar no visor I/O de unidades", 0, RTC_RAM_INT13_SHOWUP
bm05	db  B_OPTION_ENUM,		"Mudancas nos potenciometros (IRQ15)", 0, RTC_RAM_DIAL_SHOWUP, 4
		db						"Desativar", 0, "Ativar", 0
bm06	db  B_OPTION_ENUM,		"Funcao do botao 1 (NMI)", 0, RTC_RAM_IRQ14_BUTTON, 3
		db						"Nenhuma", 0, "Single-Step", 0, "Debugger por COM1", 0
bm08	db  B_OPTION_ENUM,		"IRQ do sinal periodico do RTC", 0, RTC_RAM_IRQ, 5
		db						"Nenhum", 0,  "IRQ12", 0, "IRQ11", 0, "IRQ10", 0, "IRQ8", 0

;bm09	db  B_OPTION_CALL,	"COM1 sobre WIFI/ESP8266", 0 
;		dw						 offset p_menu_prnWifi, offset p_menu_setWifi 

bm10	db  B_OPTION_ENUM,	"COM1 - I/O", 0, RTC_RAM_COM1_IO, 3
		db						"Nenhuma", 0, "3F8h (tipico)", 0,"2F8h", 0
bm11	db  B_OPTION_ENUM,		"COM1 - IRQ", 0, RTC_RAM_COM1_IRQ, 3
		db						"Nenhum", 0, "IRQ4 (tipico)", 0, "IRQ3", 0
		
bm12	db  B_OPTION_ENUM,	"COM2 - I/O", 0, RTC_RAM_COM2_IO, 3
		db						"Nenhuma", 0, "3F8h", 0,"2F8h (tipico)", 0
bm13	db  B_OPTION_ENUM,		"COM2 - IRQ", 0, RTC_RAM_COM2_IRQ, 3
		db						"Nenhum", 0, "IRQ4", 0, "IRQ3 (tipico)", 0

bm14	db  B_OPTION_ENUM,		"LPT1 - I/O", 0, RTC_RAM_LPT1_IO, 3
		db						"Nenhuma", 0, "378h (tipico)", 0, "278h", 0
bm15	db  B_OPTION_ENUM,		"LPT1 - IRQ", 0, RTC_RAM_LPT1_IRQ, 3
		db						"Nenhuma", 0, "IRQ7 (tipico)", 0, "IRQ5", 0
bm_ops:
	dw  bm01, bm02, bm03, bm04, bm05, bm06
	dw  bm08 ;bm09
	dw  bm10, bm11, bm12, bm13, bm14, bm15, 0
bm_opsSZ EQU (($ - offset bm_ops)-2)/2 ; (total)

;--------------------------------------------------------------------------------------
; PAGINA 2
bn01	db	B_OPTION_ENUM,		"Unidade FDD0", 0, RTC_RAM_FDD0, 4
		db						"Desativada", 0, "3''25",0, "5''25", 0, "Emulacao por COM1", 0

bn02	db	B_OPTION_ENUM,		"Unidade FDD1",0, RTC_RAM_FDD1, 4
		db						"Desativada", 0, "3''25", 0,  "5''25", 0, "Emulacao por COM1", 0
		
bn03	db	B_OPTION_BOOL,		"Teste reposicionar FDD0",0, RTC_RAM_FDD0R
bn04	db	B_OPTION_BOOL,		"Teste reposicionar FDD1",0, RTC_RAM_FDD1R

bn05	db	B_OPTION_ENUM,		"Unidade HDD0",0, RTC_RAM_HDD0, 3
		db						"Desativada", 0, "(IDE) Auto-detetar",0, "Emulacao por COM1", 0

bn06	db	B_OPTION_ENUM,		"Unidade HDD1",0, RTC_RAM_HDD1, 3
		db						"Desativada", 0, "(IDE) Auto-detetar",0, "Emulacao por COM1", 0

bn07	db	B_OPTION_CALL,		"Prioridade de arranque", 0 
		dw						 offset p_menu_prnBP, offset p_menu_setBP
		
bn_ops:
	dw  bn01, bn02, bn03, bn04, bn05, bn06, bn07, 0
bn_opsSZ EQU (($ - offset bn_ops)-2)/2 ; (total)
;--------------------------------------------------------------------------------------
; PAGINA 3
bk01	db	B_OPTION_ENUM,	"Testar Memoria no arranque",0, RTC_RAM_MEMTEST, 3
		db					"Desativar", 0, "Apenas I/O de 1KB em 1KB", 0, "Teste intensivo", 0
		
bk02	db	B_OPTION_CALL,	"Testar Memoria RAM", 0
		dw					 0, offset p_jmp_test_mem

bk03	db	B_OPTION_CALL,	"Testar Portas IO", 0
		dw					 0, offset p_jmp_test_io

bk04	db	B_OPTION_CALL,	"Testar Memoria CGA", 0
		dw					0, offset p_jmp_test_cga

bk05	db	B_OPTION_CALL,	"Testar Porta Serie", 0
		dw					 0, offset p_jmp_test_serial

bk06	db	B_OPTION_CALL,	"Testar Porta Paralela", 0
		dw					 0, offset p_jmp_test_plel
		
bk07	db	B_OPTION_CALL,	"Testar Porta Joystick", 0
		dw					 0, offset p_jmp_test_joy
		
bk08	db	B_OPTION_CALL,	"Atualizar a BIOS por ficheiro numa das UNIDADES", 0
		dw					 0, p_upd_file

bk09	db	B_OPTION_CALL,	"Atualizar a BIOS pela porta COM1", 0
		dw						 0, p_upd_serial


bk_ops:
	dw  bk01, bk02, bk03, bk04, bk05, bk06, bk07, bk08, bk09, 0
bk_opsSZ EQU (($ - offset bk_ops)-2)/2 ; (total)
;--------------------------------------------------------------------------------------
; PAGINA 4
bj01	db	B_OPTION_CALL,		"Processador",0 
		dw						 p_menu_prnCpu, 0
		
bj02	db	B_OPTION_CALL,		"Memoria", 0
		dw						 tty_prnMem, 0

bj_ops:
	dw  bj01, bj02, 0
bj_opsSZ EQU (($ - offset bj_ops)-2)/2 ; (total)
;--------------------------------------------------------------------------------------
; PAGINAS
bpages:      dw bm_ops, bm_opsSZ, bn_ops, bn_opsSZ, bk_ops, bk_opsSZ 
			 dw	bj_ops, bj_opsSZ
bpagesNum    EQU ($-offset bpages)/4
bpagesStrs:  db "Dispositivos", 0, "Unidades", 0, "Outros", 0, "Info", 0 
bpagesStrsSz EQU $ -offset bpagesStrs-(bpagesNum)

