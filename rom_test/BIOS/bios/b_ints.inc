;====================================================================
;== AUXILIARES/REFERENCIAS PARA INTERRUPÇÕES ========================
;====================================================================

IF BUILD_SSTEP EQ 1
	checkInt:
		push si
		push ds
			assume ds:DATA
			mov ax, DATA
			mov ds, ax			
			mov ax, SINGLE_STEP_CS
			mov si, SINGLE_STEP_IP
			mov ds, ax			; DS = seg. de código
			mov al, ds:[si]		; AL = OPCODE
			cmp al, 0CDh		; a instrução é INT?
			jne notAndInt		; não é saltar
			mov al, ds:[si+1]	; AL = Nº da INT
			clc
			jmp checkIntEnd
	notAndInt:
		stc
	checkIntEnd:
		pop ds
		pop si
		ret
ENDIF
	
	
IF (INT13_LOG+BUILD_SSTEP) NE 0
							; imprimir 8 codes
printOpcodes:
push ax
push cx
push ds
push si
	mov ax, DATA
	mov ds, ax
	IF TTY_MODE EQ 0
		call tty_sp
	ENDIF
	mov cx, 16
	assume ds:DATA
	mov si, SINGLE_STEP_IP	; DS:SI=CS:IP <instrução currente>
	mov ax, SINGLE_STEP_CS
	assume ds:CODE
	mov ds, ax				; DS:SI = opcode
	sub si, 8
	p_ss_opcodeprn:
		cmp cx, 8
		jne p_ss_opcodeprn_nxt
			IF TTY_MODE EQ 0
				call tty_crlf
			ENDIF
			mov al, '>'
			call tty_char
		p_ss_opcodeprn_nxt:
		lodsb
		call tty_Hex8
		call tty_sp
	loop p_ss_opcodeprn
	call tty_crlf
pop si
pop ds
pop cx
pop ax
ret

		; AL = OPCODE
		; AH = FUNÇÃO

printInt:
	push di
	push si
	push ds
	push es
	push ax
		call tty_crlf					; apagar linhas
		mov ax, 17
		call tty_clearLines
	pop ax
	push ax
		call ds2cs
		M_tty_ChgAtt 01Fh
		mov si, offset str_int			; imprimir AL=INT Nº
		call tty_str
		call tty_hex8
		
	IF BUILD_INTBIBLE EQ 1

		push ax
			mov al, 10h
			call tty_char
		pop ax
		
			cmp al, 13h						; INT13?					
			jne printInt_next5
				mov di, offset BIOS13h_INTs
				jmp printInt_find

			printInt_next5:
			cmp al, 20h						; INT <20h
			jge printInt_others				; OUTRAS
				mov di, offset CPUBIOS_INTs	; lookup:
				mov ah, al					; AH=nº da INT
				jmp printInt_find

			printInt_others:				; INT >=20h

			printInt_next0:
			cmp al, 20h						; INT=20h
			jne printInt_next1
				mov di, offset DOS20h_INTs
				jmp printInt_find

			printInt_next1:
			cmp al, 21h						; INT=21h
			jne printInt_next2
				mov di, offset DOS21h_INTs
				jmp printInt_find

			printInt_next2:
			cmp al, 2Fh						; INT=2Fh
			jne printInt_next3
				mov di, offset DOS2Fh_INTs
				jmp printInt_find

			printInt_next3:
			cmp al, 2Ah						;INT=2Ah
			jne printInt_notFound
				mov di, offset DOS2Ah_INTs
				jmp printInt_find

		printInt_find:		; di=offset em CS para strings
			push cx			; AH=lookup
			cld
			mov cx, BIOS_SEG
			mov ds, cx
			mov es, cx		; ds:di = es:si
			mov si, di		; para imprimir 1ª string
			call tty_str
			mov cx,0FFFFh
			push ax
				xor al, al			; encontrar zero	
				repne scasb			; es:di = 1º código
			pop ax					;       (depois do zero
									;        encontrado)
			mov al, ':'
			call tty_char

			findCode:
				mov al, es:[di]		; AL = es:[di] = código
				mov cl, es:[di+1]   ; 1º byte da string
				or cl, cl
				jz printInt_notFound

					inc di			; es:[di] = 1º byte string
				cmp al, ah			; código AH=AL?
				jne findCodeNoMatch
									; sim, ds:di = es:si
					mov si, di		; para imprimir string
					call tty_str	; e sair
					clc
					jmp printInt_found

				findCodeNoMatch:	; AH!=AL
					mov cx,0FFFFh	
					push ax
						xor al, al
						repne scasb	; es:[di] = proximo codigo
					pop ax
					jmp findCode


		printInt_notFound:
			mov al, '?'
			call tty_char
		printInt_found:
			pop cx 
	ENDIF
	M_tty_ChgAtt 007h
	clc
	
	pop ax 
	pop es
	pop ds
	pop si
	pop di
ret
ENDIF

IF BUILD_INTBIBLE EQ 1
	DOS20h_INTs:
	db "DOS Term", 0
	db 00h, "Program terminate", 0
	db 0,0

	DOS21h_INTs:
	db "DOS API", 0
	db 000h, "Program terminate", 0
	db 001h, "Keyboard input with echo", 0
	db 002h, "Display output", 0
	db 003h, "Wait for auxiliary device input", 0
	db 004h, "Auxiliary output", 0
	db 005h, "Printer output", 0
	db 006h, "Direct console I/O", 0
	db 007h, "Wait for direct con input without echo", 0
	db 008h, "Wait for con input without echo", 0
	db 009h, "Print string", 0
	db 00Ah, "Buffered keyboard input", 0
	db 00Bh, "Check standard input status", 0
	db 00Ch, "Clear keyboard buffer", 0
	db 00Dh, "Disk reset", 0
	db 00Eh, "Select disk", 0
	db 00Fh, "FCB: Open file", 0
	db 010h, "FCB: Close file", 0
	db 011h, "FCB: First entry", 0
	db 012h, "FCB: Next entry", 0
	db 013h, "FCB: Delete file", 0
	db 014h, "FCB: Seq. Read", 0
	db 015h, "FCB: Seq. Write", 0
	db 016h, "FCB: Create a file", 0
	db 017h, "FCB: Rename file", 0
	;db 018h, "dummy", 0
	db 019h, "Get current default drive", 0
	db 01Ah, "Set DTA", 0
	db 01Bh, "Get alloc table info", 0
	db 01Ch, "Get alloc table info for dev", 0
	;db 01Dh, "dummy", 0
	;db 01Eh, "dummy", 0
	db 01Fh, "Get ptr to def drive param tab",0
	db 021h, "FCB: Random read", 0
	db 022h, "FCB: Random write", 0
	db 023h, "FCB: Get file size", 0
	db 024h, "FCB: Set relative record field", 0
	db 025h, "Set interrupt", 0
	db 026h, "Create new program seg", 0
	db 027h, "FCB: Block read", 0
	db 028h, "FCB: Block write", 0
	db 029h, "FCB: Parse filename", 0
	db 02Ah, "Get date", 0
	db 02Bh, "Set date", 0
	db 02Ch, "Get time", 0
	db 02Dh, "Set time", 0
	db 02Eh, "Set/reset verify switch", 0
	db 02Fh, "Get DTA", 0
	db 030h, "Get DOS version", 0
	db 031h, "TSR", 0
	db 032h, "Get ptr to drive param table", 0
	db 033h, "Get/set Ctrl-Break & get boot drive", 0
	db 034h, "Get ptr to DOS critical flag", 0
	db 035h, "Get vector", 0
	db 036h, "Get disk free space", 0
	db 037h, "Get/set switch character", 0
	db 038h, "Get/set country info", 0
	db 039h, "Create subdir", 0
	db 03Ah, "Remove subdir", 0
	db 03Bh, "Change current subdir", 0
	db 03Ch, "Create file with handle", 0
	db 03Dh, "Open file with handle", 0
	db 03Eh, "Close file with handle", 0
	db 03Fh, "Read file/device with handle", 0
	db 040h, "Write file/device with handle", 0
	db 041h, "Delete file", 0
	db 042h, "Move file pointer with handle", 0
	db 043h, "Change file mode", 0
	db 044h, "IOCTL", 0
	db 045h, "Duplicate file handle", 0
	db 046h, "Force duplicate file handle", 0
	db 047h, "Get current directory", 0
	db 048h, "Allocate memory", 0
	db 049h, "Free allocation", 0
	db 04Ah, "Modify allocation", 0
	db 04Bh, "Load & execute program", 0
	db 04Ch, "Terminate process with return code", 0
	db 04Dh, "Get return code of a sub-process", 0
	db 04Eh, "Find first matching file", 0
	db 04Fh, "Find next matching file", 0
	db 050h, "Set current process id", 0
	db 051h, "Get current process id", 0
	db 052h, "Get ptr to DOS INVARS", 0
	db 053h, "Gen drive param table", 0
	db 054h, "Get verify setting", 0
	db 055h, "Create PSP", 0
	db 056h, "Rename file", 0
	db 057h, "Get/set file date/time using handle", 0
	db 058h, "Get/set memory allocation strategy", 0
	db 059h, "Get extended error info", 0
	db 05Ah, "Create temp file", 0
	db 05Bh, "Create new file", 0
	db 05Ch, "Lock/unlock file access", 0
	db 05Dh, "Critical error info", 0
	db 05Eh, "Network services", 0
	db 05Fh, "Network redirection", 0
	db 060h, "Get fully qualified file name", 0
	db 062h, "Get address of program segment prefix", 0
	db 063h, "Get system lead byte table", 0
	db 064h, "Set device driver look ahead", 0
	db 065h, "Get extended country info", 0
	db 066h, "Get/set global code page", 0
	db 067h, "Set handle count", 0
	db 068h, "Flush buffer", 0
	db 069h, "Get/set disk serial number", 0
	db 06Ah, "DOS reserved", 0
	db 06Bh, "DOS reserved", 0
	db 06Ch, "Extended open/create", 0
	db 0F8h, "Set INT 21 (OEM)", 0
	db 0,0


	DOS2Ah_INTs:
	db "DOS Net/Critical/Netbios",0
	db 00h, "Network install query", 0
	db 01h, "Exec NETBIOS request (no retry)", 0
	db 02h, "Set network printer mode", 0
	db 03h, "Check if direct I/O allowed", 0
	db 04h, "Exec NETBIOS request", 0
	db 05h, "Get network resource info", 0
	db 06h, "Network print stream control", 0
	db 80h, "Begin DOS critical section", 0
	db 81h, "End DOS critical section", 0
	db 82h, "End DOS critical sections 0:7", 0
	db 84h, "Keyboard busy loop", 0
	db 87h, "Critical section", 0
	db 0,0

	DOS2FH_INTS:
	db "DOS Mux",0
	db 00h  , "Get installed state", 0
	db 01h  , "Submit file", 0  
	db 02h  , "Cancel file", 0  
	db 03h  , "Cancel all files", 0  
	db 04h  , "Pause/return status", 0 
	db 05h  , "End of status", 0
	db 06h  , "Assign", 0
	db 08h  , "DRIVER.SYS support", 0	 
	db 10h  , "Share", 0	 
	db 11h  , "Redirection", 0	 
	db 12h  , "Internal", 0	 
	db 02Eh , "Error translation tab" , 0
	db 0,0

	CPUBIOS_INTs:
	db "CPU/BIOS", 0
	;db 01h, "Divide by zero", 0 	
	;db 02h, "Trap", 0 						
	;db 03h, "???", 0 	
	;db 03h, "Overflow", 0 	
	;db 04h, "???", 0 				 
	;db 05h, "Printer", 0 			
	;db 06h, "???", 0 
	;db 07h, "???", 0 
	;db 08h, "IRQ0", 0 
	;db 09h, "IRQ1", 0 
	;db 0Ah, "IRQ2", 0 
	;db 0Bh, "IRQ3", 0 
	;db 0Ch, "IRQ4", 0 
	;db 0Dh, "IRQ5", 0 
	;db 0Eh, "IRQ6", 0 
	;db 0Fh, "IRQ7", 0 
	db 10h, "Video io", 0 		
	db 11h, "Equipment", 0 		
	db 12h, "Memsize det", 0 		
	db 13h, "Disk_io", 0 			
	db 14h, "Rs232_io", 0  		
	db 15h, "Services_io", 0 		
	db 16h, "Keyb_io", 0 			
	db 17h, "Printer_io", 0 		
	db 18h, "Basic", 0 
	db 19h, "Bootstrap", 0 
	db 1Ah, "Timer_io", 0 			
	db 1Bh, "Ctrl-break", 0 
	;db 1Ch, "Timer user proc", 0 
	;db 1Dh, "Video params", 0 
	;db 1Eh, "Disquete params", 0
	;db 1Fh, "Video ext", 0
	db 0,0

	BIOS13h_INTS:
	db "BIOS disk", 0
	db 00h,"Reset", 0
	db 01h,"Get status", 0
	db 02h,"Read sectors", 0
	db 03h,"Write sectors", 0
	db 04h,"Verify sectors", 0
	db 05h,"Format track", 0
	db 06h,"Format & bad sector flag", 0
	db 07h,"Format the drive at track", 0
	db 08h,"Get drive params", 0
	db 09h,"Initialize fixed DBT", 0
	db 0Ah,"Read long sector", 0
	db 0Bh,"Write long sector", 0
	db 0Ch,"Seek to cylinder", 0
	db 0Dh,"Alternate disk reset", 0
	db 0Eh,"Read sector buffer", 0
	db 0Fh,"Write sector buffer", 0
	db 10h,"Test for drive ready", 0
	db 11h,"Recalibrate drive", 0
	db 12h,"Controller ram diagnostic", 0
	db 13h,"Drive diagnostic", 0
	db 14h,"Controller diagnostic", 0
	db 15h,"Read disk type/DASD type", 0
	db 16h,"Disk change line status", 0
	db 17h,"Set DASD type for format", 0
	db 18h,"Set media type for format", 0
	db 19h,"Park fixed disk heads", 0
	db 1Ah,"Format ESDI drive unit", 0
	db 0,0
ENDIF

IF INT13_DRAWMEM EQ 1
drawMemory:
	push ax
		mov al, 2
		call cga_setVideo
		call cga_clearZero
		call ram_drawMemory
		wait
		call makeShortBeep
		mov al, 0
		call cga_setVideo
		call cga_clear
	pop ax
ret
ENDIF
