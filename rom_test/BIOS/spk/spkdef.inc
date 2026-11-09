
	; "este segmento pode ser trocado"
	; e tem algumas coisas do som que 
	; está a tocar

	; amostras do som (em KHZ) 
SPK_SAMPLERATE_KHZ		EQU 1  ; frequencia do SOM em KHZ
SPK_DONT_INTERPOLATE	EQU 1  ; não fazer interpolação entre amostras

SPK_PARAMS_SEG SEGMENT AT SPK_PARAMS_SEG_
	spk_param_prevIRQHandler		DW  ?,? ; anterior tratador IRQ0
	spk_param_currSample			DB  ?   ; valor amostra currente
	spk_param_nextSample			DB  ?   ; valor amostra seguinte
	spk_param_cnt					DB  ?   ; contador de amostras
	spk_param_seg					DW	?   ; segmento da descompressão
SPK_PARAMS_SEG ENDS							    