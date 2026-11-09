; isto é área de dados original da BIOS
; (a ser incluido no seu segmento)

	rs232_base    DW    4 DUP(?)	; endereços das COM
	printer_base  DW    4 DUP(?)	; endereços das LPT
	equip_flag0   DB    ?		    ; hardware instalado
	equip_flag1   DB    ?		    ; hardware instalado
	mfg_tst       DB    ?		    ; flags de inicialização
	memory_size   DW    ?		    ; tamanho da memória em KBs
	mfg_err_flag  DB    ?		    ; scratch
    error_codes   DB    ?		    ; códigos de erro
	;-Teclado--------------------------------
	kb_flag 	  DB    ?
	kb_flag_1	  DB	?	; status (2º byte)
	alt_input	  DB	?	; keypad (reg. alternativo)
	buffer_head	  DW	?	; cabeça do buffer (se HEAD=TAIL está vazio)
	buffer_tail	  DW	?	; cauda do buffer
	kb_buffer	  DW	16 DUP(?)	; buffer
	kb_buffer_end	LABEL	WORD
	;-Disquetes------------------------------
	seek_status	DB	?	; estado da recalibração: 1º nibble:
						; se o bit a 0, a unidade respetiva precisa
						; de recalibrar antes de posicionar
	motor_status DB	?	; estado - 1º nibble: se bit, a unidade está
						; ocupada; bit 7: 1 se a operação é uma escrita
						; precisa de um atraso, ou 0 se leitura 
	motor_count  DB	?	; contador que ao rodar, apaga os motores
	disq_status  DB	?	; código de retorno
	nec_status   DB	7 DUP(?)	; array do estado pelo o controlador
	;-Video----------------------------------
	crt_mode	 DB	?		; modo de video currente
	crt_cols	 DW	?		; número de colunas
	crt_len 	 DW	?		; tamanho em bytes da área de texto
	crt_start	 DW	?		; endereço inicial na área de texto
	cursor_posn	 DW	8 DUP(?); onde está o cursor (até 8 páginas)
	cursor_mode	 DW	?		; modo do cursor
	active_page	 DB	?		; página currente de texto a exibir
	addr_6845	 DW	?		; endereço base para a placa de video ativa
	crt_mode_set DB	?		; confg. currente do registo 
	crt_palette	 DB	?		; palette currente na placa de video
	;-Post-----------------------------------
	io_rom_init	 DW	?		; preenchido na inicialização 
	io_rom_seg	 DW	?		; da ROM duma placa
	intr_flag	 DB	?		; flag que indica que houve interrupção 
	;-Horas----------------------------------
	time_lo		 DW	?		; contador para estabelecer
	time_hi		 DW	?		; a hora/dia
	time_ovf	 DB	?		; flag que indica que o contador rodou
	;-Timer----------------------------------
	bios_break	DB	?		; se bit 7, uma tecla for LARGADA (break)
	reset_flag	DW	?		; está a 1234H se veio de RESET
							
	;-Disco duros----------------------------
	hdd_op0    db	?		
	hdd_number db	?		
	hdd_op1    db	?		
	hdd_op2    db	?		
	;-LPT e COM------------------------------
	print_tim_out	DB	4 DUP(?)  ; timeouts
	rs232_tim_out	DB	4 DUP(?)
	;-Teclado (2º área)----------------------
	buffer_start	DW	?
	buffer_end		DW	?