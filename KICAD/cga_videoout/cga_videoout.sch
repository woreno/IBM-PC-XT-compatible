EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 17525 -6095 2    80   Italic 16
|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n
Text Notes 575  875  0    200  Italic 40
CGA: Saída de Video
$Comp
L 74xx:74LS244 U9
U 1 1 6234935B
P 8175 3325
F 0 "U9" H 7750 4100 50  0000 C CNN
F 1 "74LS244" H 7875 4025 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 8175 3325 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74ls244.pdf" H 8175 3325 50  0001 C CNN
	1    8175 3325
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 6234ADAB
P 8825 3225
F 0 "R7" V 8775 3400 50  0000 C CNN
F 1 "3.3K" V 8825 3150 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8755 3225 50  0001 C CNN
F 3 "~" H 8825 3225 50  0001 C CNN
	1    8825 3225
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 6235AA01
P 8825 3325
F 0 "R8" V 8775 3500 50  0000 C CNN
F 1 "13K" V 8825 3250 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8755 3325 50  0001 C CNN
F 3 "~" H 8825 3325 50  0001 C CNN
	1    8825 3325
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 6235B108
P 8825 3425
F 0 "R9" V 8775 3600 50  0000 C CNN
F 1 "5.6K" V 8825 3350 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8755 3425 50  0001 C CNN
F 3 "~" H 8825 3425 50  0001 C CNN
	1    8825 3425
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 6235B76D
P 8825 3525
F 0 "R10" V 8775 3700 50  0000 C CNN
F 1 "2.2K" V 8825 3450 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8755 3525 50  0001 C CNN
F 3 "~" H 8825 3525 50  0001 C CNN
	1    8825 3525
	0    1    1    0   
$EndComp
Wire Wire Line
	8975 3225 9250 3225
Wire Wire Line
	9250 3325 8975 3325
Wire Wire Line
	9250 3225 9250 3325
Connection ~ 9250 3325
Wire Wire Line
	9250 3325 9250 3425
Wire Wire Line
	8975 3425 9250 3425
Connection ~ 9250 3425
Wire Wire Line
	9250 3425 9250 3525
Wire Wire Line
	8975 3525 9250 3525
Connection ~ 9250 3525
$Comp
L Transistor_BJT:2N3904 Q1
U 1 1 623608CA
P 9450 3525
F 0 "Q1" H 9640 3571 50  0000 L CNN
F 1 "2N3904" H 9640 3480 50  0000 L CNN
F 2 "GERAL:TO-92_Inline" H 9650 3450 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 9450 3525 50  0001 L CNN
	1    9450 3525
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 623622AD
P 9550 3875
F 0 "R11" H 9450 3875 50  0000 C CNN
F 1 "33" V 9550 3800 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 9480 3875 50  0001 C CNN
F 3 "~" H 9550 3875 50  0001 C CNN
	1    9550 3875
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 62363AD1
P 9550 4175
F 0 "R12" H 9450 4175 50  0000 C CNN
F 1 "100" V 9550 4100 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 9480 4175 50  0001 C CNN
F 3 "~" H 9550 4175 50  0001 C CNN
	1    9550 4175
	1    0    0    -1  
$EndComp
Text Label 9550 3325 0    50   Italic 10
5V
$Comp
L Connector:Jack-DC J1
U 1 1 623689AB
P 10100 4125
F 0 "J1" H 9870 4083 50  0000 R CNN
F 1 "Jack-DC" H 9870 4174 50  0000 R CNN
F 2 "" H 10150 4085 50  0001 C CNN
F 3 "~" H 10150 4085 50  0001 C CNN
	1    10100 4125
	-1   0    0    1   
$EndComp
Wire Wire Line
	9800 4225 9775 4225
Wire Wire Line
	9775 4225 9775 4325
Wire Wire Line
	9775 4325 9550 4325
Wire Wire Line
	9800 4025 9550 4025
Connection ~ 9550 4025
$Comp
L power:Earth #PWR0124
U 1 1 62371291
P 8175 4125
F 0 "#PWR0124" H 8175 3875 50  0001 C CNN
F 1 "Earth" H 8175 3975 50  0001 C CNN
F 2 "" H 8175 4125 50  0001 C CNN
F 3 "~" H 8175 4125 50  0001 C CNN
	1    8175 4125
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0125
U 1 1 62371E5D
P 9550 4325
F 0 "#PWR0125" H 9550 4075 50  0001 C CNN
F 1 "Earth" H 9550 4175 50  0001 C CNN
F 2 "" H 9550 4325 50  0001 C CNN
F 3 "~" H 9550 4325 50  0001 C CNN
	1    9550 4325
	1    0    0    -1  
$EndComp
Text Notes 9750 3875 0    80   Italic 16
Video composto
Text Notes 7425 1200 0    80   Italic 16
Conetor IBM RGB 
Text Label 5825 2525 2    50   Italic 10
VIDEO_R
Text Label 5825 2425 2    50   Italic 10
VIDEO_G
Text Label 7675 2825 2    50   Italic 10
VIDEO_B
Text Label 7675 3125 2    50   Italic 10
VIDEO_I
Text Label 8675 2825 0    50   Italic 10
VIDEO_TTL_R
Text Label 8675 2925 0    50   Italic 10
VIDEO_TTL_G
Text Label 8675 3025 0    50   Italic 10
VIDEO_TTL_B
Text Label 8675 3125 0    50   Italic 10
VIDEO_TTL_I
$Comp
L Connector:DB9_Female J2
U 1 1 623797F6
P 8500 1875
F 0 "J2" H 8725 2250 50  0000 L CNN
F 1 "DB9_Female" H 8725 2175 50  0000 L CNN
F 2 "" H 8500 1875 50  0001 C CNN
F 3 " ~" H 8500 1875 50  0001 C CNN
	1    8500 1875
	1    0    0    -1  
$EndComp
Text Label 8200 1875 2    50   Italic 10
VIDEO_TTL_R
Text Label 8200 2075 2    50   Italic 10
VIDEO_TTL_G
Text Label 8200 2275 2    50   Italic 10
VIDEO_TTL_B
Text Label 8200 1575 2    50   Italic 10
VIDEO_TTL_I
Text Label 8200 1675 2    50   Italic 10
GND
Text Label 8200 1475 2    50   Italic 10
GND
Text Label 8200 1975 2    50   Italic 10
HSYNC'
Text Label 8200 2175 2    50   Italic 10
VSYNC'
Connection ~ 9550 4325
$Comp
L power:Earth #PWR0126
U 1 1 623487F2
P 5025 4625
F 0 "#PWR0126" H 5025 4375 50  0001 C CNN
F 1 "Earth" H 5025 4475 50  0001 C CNN
F 2 "" H 5025 4625 50  0001 C CNN
F 3 "~" H 5025 4625 50  0001 C CNN
	1    5025 4625
	1    0    0    -1  
$EndComp
Text Label 6025 4325 0    50   Italic 10
Q_GREEN
Text Label 6025 4225 0    50   Italic 10
Q_MAGENTA
Text Label 6025 4125 0    50   Italic 10
Q_RED
Text Label 6025 4025 0    50   Italic 10
Q_CYAN
Text Label 6025 3925 0    50   Italic 10
Q_BLUE
Text Label 6025 3825 0    50   Italic 10
Q_YELLOW
Text Label 5525 3525 0    50   Italic 10
5V
$Comp
L power:Earth #PWR0127
U 1 1 62343033
P 5525 4825
F 0 "#PWR0127" H 5525 4575 50  0001 C CNN
F 1 "Earth" H 5525 4675 50  0001 C CNN
F 2 "" H 5525 4825 50  0001 C CNN
F 3 "~" H 5525 4825 50  0001 C CNN
	1    5525 4825
	1    0    0    -1  
$EndComp
Text Label 5025 3925 2    50   Italic 10
~BW
Text Label 5025 3825 2    50   Italic 10
3.58MHz
Text Label 5025 3725 2    50   Italic 10
14MHz
$Comp
L Logic_Programmable:PAL16L8 PAL5
U 1 1 6233C41F
P 5525 4225
F 0 "PAL5" H 5150 4925 50  0000 C CNN
F 1 "ATF16V8B" H 5250 4850 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 5525 4225 50  0001 C CNN
F 3 "" H 5525 4225 50  0001 C CNN
	1    5525 4225
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0128
U 1 1 623B1A98
P 7675 3825
F 0 "#PWR0128" H 7675 3575 50  0001 C CNN
F 1 "Earth" H 7675 3675 50  0001 C CNN
F 2 "" H 7675 3825 50  0001 C CNN
F 3 "~" H 7675 3825 50  0001 C CNN
	1    7675 3825
	1    0    0    -1  
$EndComp
Wire Wire Line
	7675 3725 7675 3825
Connection ~ 7675 3825
Text Label 7675 3425 2    50   Italic 10
VIDEO_I
Text Label 8175 2525 0    50   Italic 10
5V
Text Notes 4625 3300 0    80   Italic 16
Gerador de várias ondas\nde várias fases a 3.58MHz\ne seletor duma dessas
Text Notes 5650 3725 0    200  Italic 40
*
Text Label 5025 4025 2    50   Italic 10
VIDEO_B
Text Label 5025 4125 2    50   Italic 10
VIDEO_G
Text Label 5025 4225 2    50   Italic 10
VIDEO_R
Text Label 5025 4525 2    50   Italic 10
VSYNC_DL
Text Label 6025 4425 0    50   Italic 10
VIDEO_OUT
Text Label 5025 4325 2    50   Italic 10
HSYNC_SHIFT_QG
Text Label 5025 4425 2    50   Italic 10
HSYNC_SHIFT_Q
$Comp
L 74xx:74LS175 U1
U 1 1 6227A8B2
P 1700 2150
F 0 "U1" H 1400 2800 50  0000 C CNN
F 1 "74LS175" H 1400 2725 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 1700 2150 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS175" H 1700 2150 50  0001 C CNN
	1    1700 2150
	1    0    0    -1  
$EndComp
Text Label 1200 2650 2    50   Italic 10
VSYNC_DLY
Wire Wire Line
	2200 1750 1575 1750
Wire Wire Line
	1575 1750 1575 1950
Wire Wire Line
	1575 1950 1200 1950
Wire Wire Line
	2200 1950 1675 1950
Wire Wire Line
	1675 1950 1675 2150
Wire Wire Line
	1675 2150 1200 2150
Wire Wire Line
	2200 2150 1725 2150
Wire Wire Line
	1725 2150 1725 2350
Wire Wire Line
	1725 2350 1200 2350
Text Notes 1025 1325 0    80   Italic 16
Quando VSYNCDL, gera uma onda \nde 4T
Text Label 900  1750 2    50   Italic 10
5V
$Comp
L Device:R R1
U 1 1 622901A2
P 1050 1750
F 0 "R1" V 843 1750 50  0000 C CNN
F 1 "R" V 934 1750 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 980 1750 50  0001 C CNN
F 3 "~" H 1050 1750 50  0001 C CNN
	1    1050 1750
	0    1    1    0   
$EndComp
Text Label 2350 3100 2    50   Italic 10
5V
Text Label 3750 2350 2    50   Italic 10
MCLK
$Comp
L GERAL:74LS164 U4
U 1 1 622B3750
P 3150 3525
F 0 "U4" H 2725 4400 50  0000 C CNN
F 1 "74LS164" H 2825 4325 50  0000 C CNN
F 2 "GERAL:standard_DIP_14pins" H 3150 3525 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/sn74ls165a.pdf" H 3150 3525 50  0001 C CNN
	1    3150 3525
	1    0    0    -1  
$EndComp
Text Label 3125 2600 0    50   Italic 10
5V
Text Label 2650 3750 2    50   Italic 10
GND
$Comp
L Device:R R3
U 1 1 622CE3DC
P 2500 3100
F 0 "R3" V 2600 3100 50  0000 C CNN
F 1 "R" V 2500 3025 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 2430 3100 50  0001 C CNN
F 3 "~" H 2500 3100 50  0001 C CNN
	1    2500 3100
	0    1    1    0   
$EndComp
Text Notes 6975 4700 0    80   Italic 16
Componentes e SCART
$Comp
L 74xx:74LS174 U7
U 1 1 622E441C
P 4250 2050
F 0 "U7" H 3850 2725 50  0000 C CNN
F 1 "74LS174" H 3975 2625 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 4250 2050 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS174" H 4250 2050 50  0001 C CNN
	1    4250 2050
	1    0    0    -1  
$EndComp
Text Notes 3325 1200 0    80   Italic 16
Sincroniza os sinais com MCLK
Text Label 3750 1650 2    50   Italic 10
CURSOR
Text Label 4750 1750 0    50   Italic 10
CURSOR_DL
Text Label 4750 1950 0    50   Italic 10
DISP_DL
Text Label 3750 1850 2    50   Italic 10
DISP_EN
Text Label 3750 2050 2    50   Italic 10
HSYNC
Text Label 4750 2050 0    50   Italic 10
HSYNC_DL
Text Label 3750 2150 2    50   Italic 10
VSYNC
Text Label 4750 2150 0    50   Italic 10
VSYNC_DL
Text Label 3450 2550 2    50   Italic 10
5V
$Comp
L Device:R R4
U 1 1 622FA3DC
P 3600 2550
F 0 "R4" V 3700 2550 50  0000 C CNN
F 1 "R" V 3600 2475 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 3530 2550 50  0001 C CNN
F 3 "~" H 3600 2550 50  0001 C CNN
	1    3600 2550
	0    1    1    0   
$EndComp
$Comp
L power:Earth #PWR0135
U 1 1 62300AA6
P 1700 2950
F 0 "#PWR0135" H 1700 2700 50  0001 C CNN
F 1 "Earth" H 1700 2800 50  0001 C CNN
F 2 "" H 1700 2950 50  0001 C CNN
F 3 "~" H 1700 2950 50  0001 C CNN
	1    1700 2950
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0136
U 1 1 62301ECD
P 4250 2850
F 0 "#PWR0136" H 4250 2600 50  0001 C CNN
F 1 "Earth" H 4250 2700 50  0001 C CNN
F 2 "" H 4250 2850 50  0001 C CNN
F 3 "~" H 4250 2850 50  0001 C CNN
	1    4250 2850
	1    0    0    -1  
$EndComp
Text Label 3900 4075 0    50   Italic 10
LCLK
Wire Wire Line
	3500 3750 3575 3750
Wire Wire Line
	3575 4250 3400 4250
Wire Wire Line
	3575 3750 3575 4075
Wire Wire Line
	3100 3950 3850 3950
Wire Wire Line
	3850 3950 3850 3775
Wire Wire Line
	3850 3625 3500 3625
Wire Wire Line
	3850 3775 3900 3775
Connection ~ 3850 3775
Wire Wire Line
	3850 3775 3850 3625
Wire Wire Line
	4750 1650 4300 1650
Wire Wire Line
	4300 1650 4300 1750
Wire Wire Line
	4300 1750 3750 1750
Wire Wire Line
	4750 1850 4300 1850
Wire Wire Line
	4300 1850 4300 1950
Wire Wire Line
	4300 1950 3750 1950
Text Label 4750 1850 0    50   Italic 10
DISP_DL_Q2
Text Label 5825 1625 2    50   Italic 10
DISP_DL
Text Label 5825 1725 2    50   Italic 10
DISP_DL_Q2
Text Label 5825 1825 2    50   Italic 10
VIDEO_EN
Text Label 6825 2325 0    50   Italic 10
CLR_SR
$Comp
L Logic_Programmable:PAL16L8 PAL7
U 1 1 62343AFB
P 6325 2125
F 0 "PAL7" H 5950 2825 50  0000 C CNN
F 1 "ATF16V8B" H 6050 2750 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 6325 2125 50  0001 C CNN
F 3 "" H 6325 2125 50  0001 C CNN
	1    6325 2125
	1    0    0    -1  
$EndComp
Text Label 5825 2125 2    50   Italic 10
H_QH
Text Label 5825 2025 2    50   Italic 10
H_QG
Text Label 5825 2225 2    50   Italic 10
H_QI
Text Label 6825 2125 0    50   Italic 10
VIDEO_R_MOD
Text Label 6825 2225 0    50   Italic 10
VIDEO_G_MOD
Text Label 7675 3025 2    50   Italic 10
VIDEO_R_MOD
Text Label 7675 2925 2    50   Italic 10
VIDEO_G_MOD
Text Label 6825 1625 0    50   Italic 10
~DISP_EN
Text Label 6825 1725 0    50   Italic 10
~CURSOR
Text Label 6825 1925 0    50   Italic 10
DISP_EN
Text Label 6825 1825 0    50   Italic 10
CURSOR
Connection ~ 3575 4075
Wire Wire Line
	3575 4075 3575 4250
Wire Wire Line
	3575 4075 3900 4075
Text Label 3500 3200 0    50   Italic 10
H_QH
Text Label 2800 4350 2    50   Italic 10
H_QI
Text Label 2650 3575 2    50   Italic 10
H_QD
Text Label 3500 3300 0    50   Italic 10
H_QG
Wire Wire Line
	2650 3375 700  3375
Wire Wire Line
	700  3375 700  2550
Wire Wire Line
	700  2550 1200 2550
Text Label 5825 1925 2    50   Italic 10
H_QD
Text Label 5825 2325 2    50   Italic 10
VSYNC_DL
Text Label 2175 1750 0    50   Italic 10
VSYNC_DLY_Q1
Text Label 2200 2450 0    50   Italic 10
VSYNC_DLY_Q4_
Text Label 2650 3375 2    50   Italic 10
H_QB
$Comp
L 74xx:74LS161 U3
U 1 1 623F6C89
P 2025 5400
F 0 "U3" H 1850 6200 50  0000 C CNN
F 1 "74LS161" H 1750 6100 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 2025 5400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS161" H 2025 5400 50  0001 C CNN
	1    2025 5400
	1    0    0    -1  
$EndComp
Text Label 1525 5700 2    50   Italic 10
VSYNC_DL
Text Label 750  5400 2    50   Italic 10
5V
$Comp
L Device:R R2
U 1 1 623FECA3
P 900 5400
F 0 "R2" V 1000 5400 50  0000 C CNN
F 1 "R" V 900 5325 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 830 5400 50  0001 C CNN
F 3 "~" H 900 5400 50  0001 C CNN
	1    900  5400
	0    1    1    0   
$EndComp
Text Label 2525 5200 0    50   Italic 10
CURSOR_BLINK
Wire Wire Line
	2525 5200 2775 5200
Wire Wire Line
	2775 5200 2775 5725
Wire Wire Line
	2775 5725 3050 5725
Wire Wire Line
	3650 5825 3900 5825
Wire Wire Line
	3900 5825 3900 5275
Wire Wire Line
	3900 5275 2925 5275
Wire Wire Line
	2925 5275 2925 5625
Wire Wire Line
	2925 5625 3050 5625
Text Label 3650 5625 0    50   Italic 10
BLINK
Wire Wire Line
	3350 5425 3425 5425
Wire Wire Line
	3425 5425 3425 6025
Wire Wire Line
	3425 6025 3350 6025
Wire Wire Line
	3425 6025 3525 6025
Connection ~ 3425 6025
Text Label 3050 4625 2    50   Italic 10
~BW
Wire Wire Line
	3050 4625 3100 4625
Wire Wire Line
	3100 4625 3100 4550
Wire Wire Line
	1525 5200 1525 5100
Wire Wire Line
	1525 5000 1525 4900
Wire Wire Line
	1525 5000 1525 5100
Connection ~ 1525 5000
Connection ~ 1525 5100
$Comp
L power:Earth #PWR0137
U 1 1 6243468C
P 2025 6200
F 0 "#PWR0137" H 2025 5950 50  0001 C CNN
F 1 "Earth" H 2025 6050 50  0001 C CNN
F 2 "" H 2025 6200 50  0001 C CNN
F 3 "~" H 2025 6200 50  0001 C CNN
	1    2025 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 5400 1525 5400
Wire Wire Line
	1050 5400 1050 5500
Wire Wire Line
	1050 5900 1525 5900
Connection ~ 1050 5400
Wire Wire Line
	1525 5600 1050 5600
Connection ~ 1050 5600
Wire Wire Line
	1050 5600 1050 5900
Wire Wire Line
	1525 5500 1050 5500
Connection ~ 1050 5500
Wire Wire Line
	1050 5500 1050 5600
Text Label 7675 3525 2    50   Italic 10
VIDEO_OUT
Text Label 7675 3225 2    50   Italic 10
HSYNC_PULSE
Text Label 7675 3325 2    50   Italic 10
VIDEO_R
Text Label 4250 1350 0    50   Italic 10
5V
Text Label 6325 1425 0    50   Italic 10
5V
Text Notes 7175 1975 2    50   Italic 10
-----
Text Label 2025 4600 0    50   Italic 10
5V
$Comp
L power:Earth #PWR0139
U 1 1 626E1DE7
P 6325 2725
F 0 "#PWR0139" H 6325 2475 50  0001 C CNN
F 1 "Earth" H 6325 2575 50  0001 C CNN
F 2 "" H 6325 2725 50  0001 C CNN
F 3 "~" H 6325 2725 50  0001 C CNN
	1    6325 2725
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS74 ff2
U 1 1 622B9046
P 3100 4250
F 0 "ff2" H 2800 4600 50  0000 C CNN
F 1 "74LS74" H 2875 4525 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 3100 4250 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 3100 4250 50  0001 C CNN
	1    3100 4250
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS74 ff2
U 2 1 623F8CCF
P 3350 5725
F 0 "ff2" H 3050 6075 50  0000 C CNN
F 1 "74LS74" H 3125 6000 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 3350 5725 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 3350 5725 50  0001 C CNN
	2    3350 5725
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS74 ff2
U 3 1 62742466
P 900 4125
F 0 "ff2" H 1175 4575 50  0000 C CNN
F 1 "74LS74" H 1100 4500 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 900 4125 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 900 4125 50  0001 C CNN
	3    900  4125
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0141
U 1 1 6274596A
P 900 4525
F 0 "#PWR0141" H 900 4275 50  0001 C CNN
F 1 "Earth" H 900 4375 50  0001 C CNN
F 2 "" H 900 4525 50  0001 C CNN
F 3 "~" H 900 4525 50  0001 C CNN
	1    900  4525
	1    0    0    -1  
$EndComp
Text Label 900  3725 2    50   Italic 10
5V
Text Label 2650 2975 2    50   Italic 10
H_AIN
Text Label 6825 2025 0    50   Italic 10
H_AIN
Text Label 8200 1775 2    50   Italic 10
GND
$Comp
L Device:R R18
U 1 1 62425B06
P 7475 4875
F 0 "R18" V 7425 5050 50  0000 C CNN
F 1 "680" V 7475 4800 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7405 4875 50  0001 C CNN
F 3 "~" H 7475 4875 50  0001 C CNN
	1    7475 4875
	0    1    1    0   
$EndComp
$Comp
L Device:R R19
U 1 1 62434CBB
P 7475 4975
F 0 "R19" V 7425 5150 50  0000 C CNN
F 1 "680" V 7475 4900 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7405 4975 50  0001 C CNN
F 3 "~" H 7475 4975 50  0001 C CNN
	1    7475 4975
	0    1    1    0   
$EndComp
$Comp
L Device:R R20
U 1 1 624384EE
P 7475 5075
F 0 "R20" V 7425 5250 50  0000 C CNN
F 1 "680" V 7475 5000 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7405 5075 50  0001 C CNN
F 3 "~" H 7475 5075 50  0001 C CNN
	1    7475 5075
	0    1    1    0   
$EndComp
$Comp
L Device:R R21
U 1 1 6245E837
P 7675 5350
F 0 "R21" V 7625 5525 50  0000 C CNN
F 1 "1.5K" V 7675 5275 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7605 5350 50  0001 C CNN
F 3 "~" H 7675 5350 50  0001 C CNN
	1    7675 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 624669B6
P 7800 5350
F 0 "R22" V 7750 5525 50  0000 C CNN
F 1 "1.5K" V 7800 5275 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7730 5350 50  0001 C CNN
F 3 "~" H 7800 5350 50  0001 C CNN
	1    7800 5350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 6246A46A
P 7925 5350
F 0 "R24" V 7875 5525 50  0000 C CNN
F 1 "1.5K" V 7925 5275 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7855 5350 50  0001 C CNN
F 3 "~" H 7925 5350 50  0001 C CNN
	1    7925 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7625 4875 7925 4875
Wire Wire Line
	7675 5200 7675 5075
Wire Wire Line
	7625 5075 7675 5075
Connection ~ 7675 5075
Wire Wire Line
	7625 4975 7800 4975
Wire Wire Line
	7800 5200 7800 4975
Connection ~ 7800 4975
Wire Wire Line
	7925 5200 7925 4875
Connection ~ 7925 4875
Text Label 7325 4875 2    50   Italic 10
VIDEO_TTL_R
Text Label 7325 4975 2    50   Italic 10
VIDEO_TTL_G
Text Label 7325 5075 2    50   Italic 10
VIDEO_TTL_B
Text Label 7325 5500 2    50   Italic 10
VIDEO_TTL_I
$Comp
L Transistor_FET:BS170 Q3
U 1 1 624E0C58
P 8000 5750
F 0 "Q3" H 8204 5796 50  0000 L CNN
F 1 "BS170" H 8204 5705 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 8200 5675 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BS170-D.PDF" H 8000 5750 50  0001 L CNN
	1    8000 5750
	1    0    0    -1  
$EndComp
Text Label 7100 5700 2    50   Italic 10
HSYNC'
Text Label 7100 5800 2    50   Italic 10
VSYNC'
$Comp
L Device:R R16
U 1 1 624F06CD
P 7250 5700
F 0 "R16" V 7200 5875 50  0000 C CNN
F 1 "470" V 7250 5625 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7180 5700 50  0001 C CNN
F 3 "~" H 7250 5700 50  0001 C CNN
	1    7250 5700
	0    1    1    0   
$EndComp
$Comp
L Device:R R17
U 1 1 624F4936
P 7250 5800
F 0 "R17" V 7200 5975 50  0000 C CNN
F 1 "470" V 7250 5725 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7180 5800 50  0001 C CNN
F 3 "~" H 7250 5800 50  0001 C CNN
	1    7250 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	7400 5700 7575 5700
Wire Wire Line
	7575 5800 7400 5800
Wire Wire Line
	7325 5500 7675 5500
Wire Wire Line
	7675 5500 7800 5500
Connection ~ 7675 5500
Wire Wire Line
	7925 5500 7800 5500
Connection ~ 7800 5500
Wire Wire Line
	7575 5800 7575 5750
Wire Wire Line
	7575 5750 7800 5750
Connection ~ 7575 5750
Wire Wire Line
	7575 5750 7575 5700
$Comp
L Device:R R25
U 1 1 6257D748
P 8250 5325
F 0 "R25" V 8175 5275 50  0000 C CNN
F 1 "470" V 8250 5250 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8180 5325 50  0001 C CNN
F 3 "~" H 8250 5325 50  0001 C CNN
	1    8250 5325
	0    1    1    0   
$EndComp
$Comp
L Device:R R26
U 1 1 62582167
P 8650 5325
F 0 "R26" V 8575 5300 50  0000 C CNN
F 1 "60" V 8650 5250 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 8580 5325 50  0001 C CNN
F 3 "~" H 8650 5325 50  0001 C CNN
	1    8650 5325
	0    1    1    0   
$EndComp
Text Label 8500 5175 2    50   Italic 10
5V
Wire Wire Line
	8400 5325 8500 5325
Wire Wire Line
	8500 5175 8500 5325
Connection ~ 8500 5325
$Comp
L Device:R R23
U 1 1 6259A799
P 7800 5900
F 0 "R23" V 7700 5950 50  0000 C CNN
F 1 "1M" V 7800 5825 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 7730 5900 50  0001 C CNN
F 3 "~" H 7800 5900 50  0001 C CNN
	1    7800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 6050 7950 6050
Wire Wire Line
	8100 6050 8100 5950
Wire Wire Line
	7950 6050 7950 6075
Connection ~ 7950 6050
$Comp
L power:Earth #PWR0144
U 1 1 625AB428
P 7950 6075
F 0 "#PWR0144" H 7950 5825 50  0001 C CNN
F 1 "Earth" H 7950 5925 50  0001 C CNN
F 2 "" H 7950 6075 50  0001 C CNN
F 3 "~" H 7950 6075 50  0001 C CNN
	1    7950 6075
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 6050 8100 6050
Connection ~ 8100 6050
Wire Wire Line
	8100 6050 8800 6050
Text Label 8850 4875 0    50   Italic 10
COMPONENT_R
Text Label 8850 5325 0    50   Italic 10
SCART_SW
Text Label 8850 4975 0    50   Italic 10
COMPONENT_G
Text Label 8850 5075 0    50   Italic 10
COMPONENT_R
Text Label 8850 5450 0    50   Italic 10
COMPONENT_SYNC
Text Label 8850 5600 0    50   Italic 10
COMPONENT_GND
Wire Wire Line
	8800 5600 8800 6050
Connection ~ 7800 5750
Wire Wire Line
	7675 5075 9725 5075
Wire Wire Line
	7800 4975 9725 4975
Wire Wire Line
	7925 4875 9725 4875
Wire Wire Line
	10275 4975 10225 4975
Wire Wire Line
	10225 4875 10325 4875
Wire Wire Line
	8800 5600 10325 5600
$Comp
L Connector_Generic:Conn_02x03_Counter_Clockwise J3
U 1 1 62719435
P 9925 4975
F 0 "J3" H 9975 5292 50  0000 C CNN
F 1 "Conn_02x03" H 9975 5201 50  0000 C CNN
F 2 "GERAL:jumper_set_3" H 9925 4975 50  0001 C CNN
F 3 "~" H 9925 4975 50  0001 C CNN
	1    9925 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	10275 4975 10275 5450
Wire Wire Line
	10325 4875 10325 5600
Wire Wire Line
	8100 5325 8100 5450
Connection ~ 8100 5450
Wire Wire Line
	8100 5450 8100 5550
Wire Wire Line
	8100 5450 10275 5450
Wire Wire Line
	10225 5075 10225 5325
Wire Wire Line
	8800 5325 10225 5325
Text Label 3900 3775 0    50   Italic 10
HSYNC_DL
$EndSCHEMATC
