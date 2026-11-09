
M_Bresenham MACRO X0_, X1_, Y0_, Y1_
	mov ax, X0_
	mov bx, Y0_
	mov cx, X1_
	mov dx, Y1_
	push word ptr 200h
	call cga_bres
	pop ax
	pop ax
ENDM


 IF DEBUG_BRESENHAM EQ 0
 	mov ax, 4
 	INT 10h
 	mov ax, 0FFh
 	call cga_fill
 ENDIf

			---X---	  ----Y----
M_Bresenham 190, 20,  10, 100
hlt

IF DEBUG_BRESENHAM EQ 1
	call tty_crlf
	M_tty_wrStr "asdas"
ENDIF

test_pixels:
	mov dl, 3
	mov ax, 0
	mov bx, 0
	loopY:
		loopX:
			call cga_putpixel
		inc ax
		cmp ax, 320
		jne loopX
	mov ax,0 
	inc bx
	cmp bx, 200
	jle loopY
	