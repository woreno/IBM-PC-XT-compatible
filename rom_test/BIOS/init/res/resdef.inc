
RESOURCES_IN_FILE EQU 0 
RESOURCES_IN_ROM  EQU 1 

; para obter o segmento dum recurso chamar a 
RES_ID_CGA0_DRAW  EQU 0
RES_ID_CGA1_DRAW  EQU 1
RES_ID_VOICES     EQU 2

SND_DECOMPRESSION_SEG EQU 7C00h

; se DOS os recursos estão em ficheiro

	; estas funções metem DS no recurso pretendido
	; resource_init
IF RESOURCES_IN_FILE EQ 1
	M_getResourceSeg MACRO DSTSEG_, RES_INDEX_
		push ax
		push bx
			; BX = segmento inicial ALOCADO EM DOS
			mov bx, word ptr cs:[offset res_segAlloc]
			mov DSTSEG_, bx

			; este paragrafo tem os "inicios" em paragrafos
			; para os "recursos"; indexar
			
			mov ax, word ptr DSTSEG_:[RES_INDEX_*2]
			; AX = tem o paragrafo relativo 
			add ax, bx					; adicionar segAlloc
			mov DSTSEG_, ax	
		pop bx
		pop ax
	ENDM
ENDIF

