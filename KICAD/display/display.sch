EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A2 23386 16535
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
Text Label 1675 1725 0    315  Italic 63
CHIP_SELECT
$Comp
L power:Earth #PWR0109
U 1 1 6A20980D
P 2175 975
F 0 "#PWR0109" H 2175 725 50  0001 C CNN
F 1 "Earth" H 2175 825 50  0001 C CNN
F 2 "" H 2175 975 50  0001 C CNN
F 3 "~" H 2175 975 50  0001 C CNN
	1    2175 975 
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0102
U 1 1 61948B0A
P 1475 2950
F 0 "#PWR0102" H 1475 2700 50  0001 C CNN
F 1 "Earth" H 1475 2800 50  0001 C CNN
F 2 "" H 1475 2950 50  0001 C CNN
F 3 "~" H 1475 2950 50  0001 C CNN
	1    1475 2950
	1    0    0    -1  
$EndComp
Text Label 1875 2950 2    50   Italic 10
GND
Text Label 3300 3750 0    50   Italic 10
D0
Text Label 3300 3650 0    50   Italic 10
D1
Text Label 3300 3550 0    50   Italic 10
D2
Text Label 3300 3450 0    50   Italic 10
D3
Text Label 3300 3350 0    50   Italic 10
D4
Text Label 3300 3250 0    50   Italic 10
D5
Text Label 3300 3150 0    50   Italic 10
D6
Text Label 3300 3050 0    50   Italic 10
D7
Text Label 1900 3050 2    50   Italic 10
RESET
Wire Wire Line
	1475 2950 1900 2950
Text Label 2175 975  0    50   ~ 0
GND
Text Label 1900 4250 2    50   Italic 10
~IOR
Text Label 1900 4150 2    50   Italic 10
~IOW
Text Label 1900 5650 2    50   Italic 10
ALE
Text Label 3300 5250 0    50   Italic 10
A7
Text Label 3300 5350 0    50   Italic 10
A6
Text Label 3300 5450 0    50   Italic 10
A5
Text Label 3300 5550 0    50   Italic 10
A4
Text Label 3300 5650 0    50   Italic 10
A3
Text Label 3300 5750 0    50   Italic 10
A2
Text Label 3300 5850 0    50   Italic 10
A1
Text Label 3300 5950 0    50   Italic 10
A0
Text Label 3300 5150 0    50   Italic 10
A8
Text Label 3300 5050 0    50   Italic 10
A9
Text Label 3300 4950 0    50   Italic 10
A10
Text Label 3300 3950 0    50   Italic 10
AEN
Text Label 1900 3150 2    50   Italic 10
5V
Text Label 1900 5950 2    50   Italic 10
GND
$Comp
L GERAL:ISA8-Edge U7
U 1 1 61DAF975
P 2600 3100
F 0 "U7" H 2600 3517 50  0000 C CNN
F 1 "ISA8-Edge" H 2600 3426 50  0000 C CNN
F 2 "GERAL:ISA_8bit_edge" H 2600 3550 50  0001 C CNN
F 3 "" H 2600 3550 50  0001 C CNN
	1    2600 3100
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS670 U13
U 1 1 61A0C06F
P 14425 4550
F 0 "U13" H 14525 5325 50  0000 C CNN
F 1 "74LS670" H 14650 5225 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 14425 4550 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS670" H 14425 4550 50  0001 C CNN
	1    14425 4550
	1    0    0    -1  
$EndComp
Text Label 9475 1425 0    315  Italic 63
SEGMENT-DISPLAY
$Comp
L power:Earth #PWR0108
U 1 1 61A294A1
P 10550 5825
F 0 "#PWR0108" H 10550 5575 50  0001 C CNN
F 1 "Earth" H 10550 5675 50  0001 C CNN
F 2 "" H 10550 5825 50  0001 C CNN
F 3 "~" H 10550 5825 50  0001 C CNN
	1    10550 5825
	1    0    0    -1  
$EndComp
Wire Wire Line
	10350 5775 10550 5775
Wire Wire Line
	10550 5775 10550 5825
Wire Wire Line
	13925 4950 13800 4950
$Comp
L GERAL:74LS139 U14
U 1 1 61A95A91
P 15375 4975
F 0 "U14" H 15400 5550 50  0000 C CNN
F 1 "74LS139" H 15400 5459 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 15425 5525 50  0001 C CNN
F 3 "" H 15425 5525 50  0001 C CNN
	1    15375 4975
	1    0    0    -1  
$EndComp
Wire Wire Line
	13800 4950 13800 5550
Wire Wire Line
	13800 5550 14875 5550
Wire Wire Line
	14875 5550 14875 4875
Wire Wire Line
	14875 4875 15025 4875
Connection ~ 13800 4950
Wire Wire Line
	13850 5050 13850 5650
Wire Wire Line
	13850 5650 14925 5650
Wire Wire Line
	14925 5650 14925 4975
Wire Wire Line
	14925 4975 15025 4975
Connection ~ 13850 5050
Wire Wire Line
	13850 5050 13925 5050
$Comp
L Display_Character:CC56-12SRWA U16
U 1 1 61AAAC35
P 18925 5150
F 0 "U16" H 18925 5817 50  0000 C CNN
F 1 "CC56-12SRWA" H 18925 5726 50  0000 C CNN
F 2 "Display_7Segment:CA56-12SRWA" H 18925 4550 50  0001 C CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/CC56-12SRWA.pdf" H 18495 5180 50  0001 C CNN
	1    18925 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	14925 4050 14925 2850
Wire Wire Line
	14925 2850 15175 2850
Wire Wire Line
	14925 4150 15000 4150
Wire Wire Line
	15000 4150 15000 2950
Wire Wire Line
	15000 2950 15175 2950
Wire Wire Line
	15175 3050 15050 3050
Wire Wire Line
	15050 3050 15050 4250
Wire Wire Line
	15050 4250 14925 4250
Wire Wire Line
	14925 4350 15100 4350
Wire Wire Line
	15100 4350 15100 3150
Wire Wire Line
	15100 3150 15175 3150
$Comp
L power:Earth #PWR0110
U 1 1 61AF6C50
P 14425 5450
F 0 "#PWR0110" H 14425 5200 50  0001 C CNN
F 1 "Earth" H 14425 5300 50  0001 C CNN
F 2 "" H 14425 5450 50  0001 C CNN
F 3 "~" H 14425 5450 50  0001 C CNN
	1    14425 5450
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0111
U 1 1 61AFB631
P 15025 5675
F 0 "#PWR0111" H 15025 5425 50  0001 C CNN
F 1 "Earth" H 15025 5525 50  0001 C CNN
F 2 "" H 15025 5675 50  0001 C CNN
F 3 "~" H 15025 5675 50  0001 C CNN
	1    15025 5675
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0112
U 1 1 61AECBEE
P 12950 5750
F 0 "#PWR0112" H 12950 5500 50  0001 C CNN
F 1 "Earth" H 12950 5600 50  0001 C CNN
F 2 "" H 12950 5750 50  0001 C CNN
F 3 "~" H 12950 5750 50  0001 C CNN
	1    12950 5750
	1    0    0    -1  
$EndComp
Text Label 13400 5050 0    50   Italic 10
dispC1
Text Label 13400 4950 0    50   Italic 10
dispC0
Text Label 12450 5400 2    50   Italic 10
dispCLK~
Wire Wire Line
	13400 5050 13850 5050
Wire Wire Line
	13800 4950 13400 4950
$Comp
L GERAL:74LS161 U9
U 1 1 61A164C2
P 12950 5100
F 0 "U9" H 13125 4575 50  0000 C CNN
F 1 "74LS161" H 13150 4500 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 12950 4900 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS161" H 12950 4900 50  0001 C CNN
	1    12950 5100
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0113
U 1 1 61B047E9
P 15775 3950
F 0 "#PWR0113" H 15775 3700 50  0001 C CNN
F 1 "Earth" H 15775 3800 50  0001 C CNN
F 2 "" H 15775 3950 50  0001 C CNN
F 3 "~" H 15775 3950 50  0001 C CNN
	1    15775 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61BA7B0C
P 11850 3725
F 0 "R3" V 11950 3725 50  0000 C CNN
F 1 "R" V 12025 3725 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 11780 3725 50  0001 C CNN
F 3 "~" H 11850 3725 50  0001 C CNN
	1    11850 3725
	0    1    1    0   
$EndComp
Text Label 12450 5300 2    50   ~ 0
5V
Text Label 11700 3725 2    50   ~ 0
5V
Text Label 15775 4625 0    50   ~ 0
5V
Text Label 15775 2400 0    50   ~ 0
5V
$Comp
L power:Earth #PWR0114
U 1 1 61CF4E01
P 17825 5550
F 0 "#PWR0114" H 17825 5300 50  0001 C CNN
F 1 "Earth" H 17825 5400 50  0001 C CNN
F 2 "" H 17825 5550 50  0001 C CNN
F 3 "~" H 17825 5550 50  0001 C CNN
	1    17825 5550
	1    0    0    -1  
$EndComp
Text Label 13925 4050 2    50   Italic 10
D0
Text Label 13925 4150 2    50   Italic 10
D1
Text Label 13925 4250 2    50   Italic 10
D2
Text Label 13925 4350 2    50   Italic 10
D3
Wire Wire Line
	14425 5450 13925 5450
Wire Wire Line
	13925 5450 13925 5150
Connection ~ 14425 5450
Text Label 13925 4550 2    50   Italic 10
A0
Text Label 13925 4650 2    50   Italic 10
A1
Text Label 13925 4750 2    50   Italic 10
~dispCS
Wire Wire Line
	10925 5400 12075 5400
Wire Wire Line
	10550 5475 11725 5475
Wire Wire Line
	10350 4950 10925 4950
Wire Wire Line
	10550 5050 10925 5050
Text Label 10925 5150 2    50   Italic 10
dispCLK~
Wire Wire Line
	11725 4675 11725 4750
Text Label 11725 4675 0    50   ~ 0
5V
Connection ~ 11725 4750
Wire Wire Line
	11575 4750 11725 4750
Wire Wire Line
	11575 4950 11575 4750
Wire Wire Line
	12075 4750 11725 4750
Wire Wire Line
	12075 5400 12075 4750
Wire Wire Line
	10925 5250 10925 5400
Wire Wire Line
	10350 4950 10350 5775
Connection ~ 10550 5475
$Comp
L Device:C C1
U 1 1 61A27CEB
P 10550 5625
F 0 "C1" H 10665 5671 50  0000 L CNN
F 1 "C" H 10665 5580 50  0000 L CNN
F 2 "GERAL:standard_2pins_half" H 10588 5475 50  0001 C CNN
F 3 "~" H 10550 5625 50  0001 C CNN
	1    10550 5625
	1    0    0    -1  
$EndComp
Connection ~ 10925 5050
Wire Wire Line
	10550 5475 10550 5050
Wire Wire Line
	11725 5350 11725 5475
Connection ~ 11725 5050
Wire Wire Line
	11575 5050 11725 5050
$Comp
L Device:R R2
U 1 1 61A21CB7
P 11725 5200
F 0 "R2" H 11795 5246 50  0000 L CNN
F 1 "R" H 11795 5155 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 11655 5200 50  0001 C CNN
F 3 "~" H 11725 5200 50  0001 C CNN
	1    11725 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61A21745
P 11725 4900
F 0 "R1" H 11795 4946 50  0000 L CNN
F 1 "R" H 11795 4855 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 11655 4900 50  0001 C CNN
F 3 "~" H 11725 4900 50  0001 C CNN
	1    11725 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	11300 5150 11575 5150
Wire Wire Line
	11300 5050 11300 5150
Wire Wire Line
	10925 5050 11300 5050
$Comp
L GERAL:555 U8
U 1 1 61A0F85B
P 11275 5100
F 0 "U8" H 11250 5515 50  0000 C CNN
F 1 "555" H 11250 5424 50  0000 C CNN
F 2 "GERAL:standard_DIP_8pins" H 11275 5500 50  0001 C CNN
F 3 "" H 11275 5500 50  0001 C CNN
	1    11275 5100
	1    0    0    -1  
$EndComp
Connection ~ 10550 5775
Text Notes 10500 4200 0    50   Italic 10
montagem sinal periódico:\nfaz um clk de baixa frequência\npara atualizar os digitos
Text Notes 11950 4250 0    50   Italic 10
o contador incrementa, para\nse selecionar o registo de saída \n(00,01,10,10) a cada clk
Text Notes 12950 3725 0    50   Italic 10
esta presa guarda 4 digitos por A0,A1 e DADOS\npermite escrever se\ndescodificado o seu endereço\ncom ~IOW
Text Label 14425 3750 0    50   ~ 0
5V
Text Notes 15825 5900 0    50   Italic 10
o demux dado (00,01,10,11)\npoem a GND a respetiva linha,\npara selecionar o digito
Text Label 17825 4850 2    50   Italic 10
dispA_11
Text Label 17825 4950 2    50   Italic 10
dispB_7
Text Label 17825 5050 2    50   Italic 10
dispC_4
Text Label 17825 5150 2    50   Italic 10
dispD_2
Text Label 17825 5250 2    50   Italic 10
dispE_1
Text Label 17825 5350 2    50   Italic 10
dispF_10
Text Label 17825 5450 2    50   Italic 10
dispG_5
Text Label 20025 5550 0    50   Italic 10
dispDigit0_12
Text Label 20025 5450 0    50   Italic 10
dispDigit1_9
Text Label 20025 5350 0    50   Italic 10
dispDigit2_8
Text Label 20025 5250 0    50   Italic 10
dispDigit3_6
Text Label 17975 3550 0    50   Italic 10
dispA_11
Text Label 17975 3950 0    50   Italic 10
dispB_7
Text Label 17475 3750 2    50   Italic 10
dispC_4
Text Label 17475 3550 2    50   Italic 10
dispD_2
Text Label 17975 3650 0    50   Italic 10
dispF_10
Text Label 17475 3850 2    50   Italic 10
dispG_5
Text Label 17975 3750 0    50   Italic 10
dispDigit1_9
Text Label 17975 3850 0    50   Italic 10
dispDigit2_8
Text Label 17475 3950 2    50   Italic 10
dispDigit3_6
Text Label 17475 3450 2    50   Italic 10
dispE_1
Wire Wire Line
	15025 4775 14925 4775
Wire Wire Line
	14925 4775 14925 4750
Wire Wire Line
	14925 4750 14825 4750
Text Label 15775 5525 0    50   Italic 10
dispDigit3_6
Text Label 15775 5425 0    50   Italic 10
dispDigit2_8
Text Label 15775 5325 0    50   Italic 10
dispDigit1_9
$Comp
L power:Earth #PWR0116
U 1 1 62131727
P 12250 5150
F 0 "#PWR0116" H 12250 4900 50  0001 C CNN
F 1 "Earth" H 12250 5000 50  0001 C CNN
F 2 "" H 12250 5150 50  0001 C CNN
F 3 "~" H 12250 5150 50  0001 C CNN
	1    12250 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	12450 5200 12350 5200
Wire Wire Line
	12350 5200 12350 5150
Wire Wire Line
	12350 5150 12250 5150
Wire Wire Line
	12450 4800 12450 4900
Wire Wire Line
	12450 4900 12450 5000
Connection ~ 12450 4900
Wire Wire Line
	12450 5000 12450 5100
Connection ~ 12450 5000
Wire Wire Line
	12450 5100 12350 5100
Wire Wire Line
	12350 5100 12350 5150
Connection ~ 12450 5100
Connection ~ 12350 5150
Wire Wire Line
	16375 3650 16375 3825
Wire Wire Line
	16375 3825 15550 3825
Wire Wire Line
	15175 3825 15175 3650
Text Label 12000 3725 0    50   Italic 10
dispPullup
Text Label 12450 5500 2    50   Italic 10
dispPullup
Text Label 15550 3950 2    50   Italic 10
dispPullup
Wire Wire Line
	15550 3950 15550 3825
Connection ~ 15550 3825
Wire Wire Line
	15550 3825 15175 3825
Text Label 5300 3700 2    50   Italic 10
A3
Text Label 5300 3500 2    50   Italic 10
~IOW
$Comp
L 74xx:74LS32 U18
U 1 1 620E5866
P 5600 3600
F 0 "U18" H 5600 3325 50  0000 C CNN
F 1 "74LS32" H 5600 3400 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 5600 3600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 5600 3600 50  0001 C CNN
	1    5600 3600
	1    0    0    -1  
$EndComp
Text Notes 5600 3375 0    50   Italic 10
0x2Fx
$Comp
L 74xx:74LS32 U18
U 2 1 6208E429
P 6200 3500
F 0 "U18" H 6200 3825 50  0000 C CNN
F 1 "74LS32" H 6200 3734 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6200 3500 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 6200 3500 50  0001 C CNN
	2    6200 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5525 3400 5900 3400
Wire Wire Line
	5525 2950 5525 3400
Wire Wire Line
	5425 2950 5525 2950
Text Notes 5425 2750 0    50   Italic 10
0x1Fx
Text Label 3300 3850 0    50   Italic 10
IO_READY
NoConn ~ 5425 3050
NoConn ~ 5425 2850
NoConn ~ 5425 2550
Text Label 4925 2150 0    50   ~ 0
5V
$Comp
L power:Earth #PWR0146
U 1 1 6A24C577
P 4925 3450
F 0 "#PWR0146" H 4925 3200 50  0001 C CNN
F 1 "Earth" H 4925 3300 50  0001 C CNN
F 2 "" H 4925 3450 50  0001 C CNN
F 3 "~" H 4925 3450 50  0001 C CNN
	1    4925 3450
	1    0    0    -1  
$EndComp
Text Label 3825 3150 2    50   ~ 0
A6
Text Label 3825 2950 2    50   ~ 0
A5
Text Label 4425 2950 2    50   ~ 0
A4
Text Label 4425 2450 2    50   ~ 0
A7
Text Label 4425 2550 2    50   ~ 0
A8
Text Label 4425 2650 2    50   ~ 0
A9
$Comp
L 74xx:74LS138 U102
U 1 1 692B91C5
P 4925 2750
F 0 "U102" H 5225 3275 50  0000 C CNN
F 1 "74LS138" H 5150 3200 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 4925 2750 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS138" H 4925 2750 50  0001 C CNN
	1    4925 2750
	1    0    0    -1  
$EndComp
Text Notes 4700 1975 0    50   Italic 10
\n
Text Notes 15175 2200 0    50   Italic 10
o descodificador de caracteres 7-seg\nrecebe os dados da presa\naqui o LS247 foi trocado por DM9368 (fairchild)
Text Label 16375 3450 0    50   Italic 10
dispG_5
Text Label 16375 3350 0    50   Italic 10
dispF_10
Text Label 16375 3250 0    50   Italic 10
dispE_1
Text Label 16375 3150 0    50   Italic 10
dispD_2
Text Label 16375 3050 0    50   Italic 10
dispC_4
Text Label 16375 2950 0    50   Italic 10
dispB_7
Text Label 16375 2850 0    50   Italic 10
dispA_11
Wire Wire Line
	15775 2400 15775 2550
$Comp
L 74xx:74LS248 U15
U 1 1 61AC7F8D
P 15775 3250
F 0 "U15" H 15975 3925 50  0000 C CNN
F 1 "DM9368" H 15975 3825 50  0000 C CNN
F 2 "GERAL:standard_DIP_16OvalPins" H 15775 3250 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS248" H 15775 3250 50  0001 C CNN
	1    15775 3250
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0117
U 1 1 61B4E099
P 15175 3450
F 0 "#PWR0117" H 15175 3200 50  0001 C CNN
F 1 "Earth" H 15175 3300 50  0001 C CNN
F 2 "" H 15175 3450 50  0001 C CNN
F 3 "~" H 15175 3450 50  0001 C CNN
	1    15175 3450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS573 U17
U 1 1 61BA4887
P 12300 8100
F 0 "U17" H 12300 9081 50  0000 C CNN
F 1 "74LS573" H 12300 8990 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 12300 8100 50  0001 C CNN
F 3 "74xx/74hc573.pdf" H 12300 8100 50  0001 C CNN
	1    12300 8100
	1    0    0    -1  
$EndComp
Text Label 6500 3700 2    50   Italic 10
A2
Text Label 5925 4150 2    50   Italic 10
A2
Text Label 7850 4250 0    50   Italic 10
ledsLoad
$Comp
L 74xx:74LS00 U5
U 2 1 61C02706
P 6225 4250
F 0 "U5" H 6225 4575 50  0000 C CNN
F 1 "74LS00" H 6225 4484 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6225 4250 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls00" H 6225 4250 50  0001 C CNN
	2    6225 4250
	1    0    0    -1  
$EndComp
Text Label 5925 4350 2    50   Italic 10
A2
$Comp
L power:Earth #PWR0118
U 1 1 61C40018
P 11800 8600
F 0 "#PWR0118" H 11800 8350 50  0001 C CNN
F 1 "Earth" H 11800 8450 50  0001 C CNN
F 2 "" H 11800 8600 50  0001 C CNN
F 3 "~" H 11800 8600 50  0001 C CNN
	1    11800 8600
	1    0    0    -1  
$EndComp
Text Label 11800 7600 2    50   Italic 10
D0
Text Label 11800 7700 2    50   Italic 10
D1
Text Label 11800 7800 2    50   Italic 10
D2
Text Label 11800 7900 2    50   Italic 10
D3
Text Label 11800 8000 2    50   Italic 10
D4
Text Label 11800 8100 2    50   Italic 10
D5
Text Label 11800 8200 2    50   Italic 10
D6
Text Label 11800 8300 2    50   Italic 10
D7
Text Notes 6925 4025 0    50   Italic 10
0x2F4:7
Text Notes 6325 3375 0    50   Italic 10
0x2F0:7\n
Text Label 11800 8500 2    50   Italic 10
ledsLoad
$Comp
L GERAL:7407 U220
U 1 1 61D1E58A
P 13000 7600
F 0 "U220" H 13012 7865 50  0000 C CNN
F 1 "7407" H 13000 7775 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 7825 50  0001 C CNN
F 3 "" H 13600 7825 50  0001 C CNN
	1    13000 7600
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U220
U 2 1 61D22FC9
P 13000 7700
F 0 "U220" H 13012 7873 50  0001 C CNN
F 1 "7407" H 13012 7874 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 7925 50  0001 C CNN
F 3 "" H 13600 7925 50  0001 C CNN
	2    13000 7700
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U220
U 3 1 61D2FE9D
P 13000 7800
F 0 "U220" H 13012 7973 50  0001 C CNN
F 1 "7407" H 13012 7974 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8025 50  0001 C CNN
F 3 "" H 13600 8025 50  0001 C CNN
	3    13000 7800
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U220
U 4 1 61D3CE90
P 13000 7900
F 0 "U220" H 13012 8073 50  0001 C CNN
F 1 "7407" H 13012 8074 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8125 50  0001 C CNN
F 3 "" H 13600 8125 50  0001 C CNN
	4    13000 7900
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U220
U 5 1 61D42BE1
P 13000 8000
F 0 "U220" H 13012 8173 50  0001 C CNN
F 1 "7407" H 13012 8174 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8225 50  0001 C CNN
F 3 "" H 13600 8225 50  0001 C CNN
	5    13000 8000
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U220
U 6 1 61D49E20
P 13000 8100
F 0 "U220" H 13012 8273 50  0001 C CNN
F 1 "7407" H 13012 8274 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8325 50  0001 C CNN
F 3 "" H 13600 8325 50  0001 C CNN
	6    13000 8100
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U221
U 1 1 61D530B3
P 13000 8300
F 0 "U221" H 13012 8473 50  0001 C CNN
F 1 "7407" H 13012 8474 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8525 50  0001 C CNN
F 3 "" H 13600 8525 50  0001 C CNN
	1    13000 8300
	1    0    0    -1  
$EndComp
$Comp
L GERAL:7407 U221
U 2 1 61D60226
P 13000 8200
F 0 "U221" H 13012 8373 50  0001 C CNN
F 1 "7407" H 13012 8374 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13600 8425 50  0001 C CNN
F 3 "" H 13600 8425 50  0001 C CNN
	2    13000 8200
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS32 U18
U 4 1 61BEF9E0
P 6825 4150
F 0 "U18" H 6825 4475 50  0000 C CNN
F 1 "74LS32" H 6825 4384 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6825 4150 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 6825 4150 50  0001 C CNN
	4    6825 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x10_Male J4
U 1 1 623F97CE
P 13925 8100
F 0 "J4" H 14033 8681 50  0000 C CNN
F 1 "Conn_01x10_Male" H 14033 8590 50  0000 C CNN
F 2 "GERAL:Conector2x5_Oval_alternado" H 13925 8100 50  0001 C CNN
F 3 "~" H 13925 8100 50  0001 C CNN
	1    13925 8100
	-1   0    0    1   
$EndComp
Text Label 13725 8400 2    50   ~ 0
GND
Text Label 13725 8500 2    50   ~ 0
5V
Text Label 12300 7300 0    50   ~ 0
5V
$Comp
L power:Earth #PWR0119
U 1 1 6368BD57
P 12300 8900
F 0 "#PWR0119" H 12300 8650 50  0001 C CNN
F 1 "Earth" H 12300 8750 50  0001 C CNN
F 2 "" H 12300 8900 50  0001 C CNN
F 3 "~" H 12300 8900 50  0001 C CNN
	1    12300 8900
	1    0    0    -1  
$EndComp
Text Label 17475 3650 2    50   ~ 0
GND
Text Label 4900 5900 0    50   ~ 0
5V
Text Label 1900 5750 2    50   Italic 10
5V
$Comp
L GERAL:7407 U221
U 7 1 6370660F
P 13125 8750
F 0 "U221" H 13137 8923 50  0001 C CNN
F 1 "7407" H 13137 8924 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13725 8975 50  0001 C CNN
F 3 "" H 13725 8975 50  0001 C CNN
	7    13125 8750
	1    0    0    -1  
$EndComp
Text Label 12900 8750 2    50   ~ 0
GND
Text Label 13325 8750 0    50   ~ 0
5V
$Comp
L GERAL:7407 U220
U 7 1 63750C06
P 13125 8900
F 0 "U220" H 13137 9073 50  0001 C CNN
F 1 "7407" H 13137 9074 50  0001 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 13725 9125 50  0001 C CNN
F 3 "" H 13725 9125 50  0001 C CNN
	7    13125 8900
	1    0    0    -1  
$EndComp
Text Label 12900 8900 2    50   ~ 0
GND
Text Label 13325 8900 0    50   ~ 0
5V
Wire Wire Line
	13225 7600 13725 7600
Wire Wire Line
	13225 7700 13725 7700
Wire Wire Line
	13225 7800 13725 7800
Wire Wire Line
	13225 7900 13725 7900
Wire Wire Line
	13225 8000 13725 8000
Wire Wire Line
	13225 8100 13725 8100
Wire Wire Line
	13225 8200 13725 8200
Wire Wire Line
	13225 8300 13725 8300
Text Label 1900 5150 2    50   Italic 10
IRQ5
$Comp
L Device:C C4
U 1 1 61C36CAA
P 2875 9750
F 0 "C4" V 2623 9750 50  0000 C CNN
F 1 "150nF" V 2714 9750 50  0000 C CNN
F 2 "GERAL:standard_2pins_half" H 2913 9600 50  0001 C CNN
F 3 "~" H 2875 9750 50  0001 C CNN
	1    2875 9750
	0    1    1    0   
$EndComp
Wire Wire Line
	3925 9550 3925 9850
Wire Wire Line
	3825 9550 3925 9550
Wire Wire Line
	3525 9550 3325 9550
$Comp
L Device:R R8
U 1 1 61BF5A2F
P 3675 9550
F 0 "R8" V 3468 9550 50  0000 C CNN
F 1 "500K" V 3559 9550 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3605 9550 50  0001 C CNN
F 3 "~" H 3675 9550 50  0001 C CNN
	1    3675 9550
	0    1    1    0   
$EndComp
Wire Wire Line
	3325 9750 3325 9550
$Comp
L Device:R R6
U 1 1 61BBDC64
P 3175 9750
F 0 "R6" V 2968 9750 50  0000 C CNN
F 1 "R" V 3059 9750 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3105 9750 50  0001 C CNN
F 3 "~" H 3175 9750 50  0001 C CNN
	1    3175 9750
	0    1    1    0   
$EndComp
Connection ~ 3325 9750
Connection ~ 3925 9850
$Comp
L Amplifier_Operational:LM358 U300
U 1 1 61B991B8
P 3625 9850
F 0 "U300" H 3625 10217 50  0000 C CNN
F 1 "LM358" H 3625 10126 50  0000 C CNN
F 2 "GERAL:LM386" H 3625 9850 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 3625 9850 50  0001 C CNN
	1    3625 9850
	1    0    0    1   
$EndComp
Wire Wire Line
	3925 9850 4175 9850
$Comp
L Transistor_BJT:2N3904 Q3
U 1 1 61F2E6D3
P 4525 10150
F 0 "Q3" H 4715 10196 50  0000 L CNN
F 1 "2N3904" H 4715 10105 50  0000 L CNN
F 2 "GERAL:TO-92_Inline" H 4725 10075 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 4525 10150 50  0001 L CNN
	1    4525 10150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 61F2E6D9
P 4175 10000
F 0 "R20" V 3968 10000 50  0000 C CNN
F 1 "10K" V 4059 10000 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4105 10000 50  0001 C CNN
F 3 "~" H 4175 10000 50  0001 C CNN
	1    4175 10000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 61F2E6DF
P 4175 10300
F 0 "R21" V 3968 10300 50  0000 C CNN
F 1 "7K" V 4059 10300 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4105 10300 50  0001 C CNN
F 3 "~" H 4175 10300 50  0001 C CNN
	1    4175 10300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4175 10150 4325 10150
Connection ~ 4175 10150
$Comp
L power:Earth #PWR0124
U 1 1 61F5E49C
P 4175 10450
F 0 "#PWR0124" H 4175 10200 50  0001 C CNN
F 1 "Earth" H 4175 10300 50  0001 C CNN
F 2 "" H 4175 10450 50  0001 C CNN
F 3 "~" H 4175 10450 50  0001 C CNN
	1    4175 10450
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0125
U 1 1 61F676FA
P 4625 10350
F 0 "#PWR0125" H 4625 10100 50  0001 C CNN
F 1 "Earth" H 4625 10200 50  0001 C CNN
F 2 "" H 4625 10350 50  0001 C CNN
F 3 "~" H 4625 10350 50  0001 C CNN
	1    4625 10350
	1    0    0    -1  
$EndComp
Text Label 4625 9650 1    50   ~ 0
5V
$Comp
L Device:R R24
U 1 1 61F7DF24
P 4625 9800
F 0 "R24" V 4418 9800 50  0000 C CNN
F 1 "1K" V 4509 9800 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4555 9800 50  0001 C CNN
F 3 "~" H 4625 9800 50  0001 C CNN
	1    4625 9800
	1    0    0    -1  
$EndComp
Connection ~ 4625 9950
Text Label 2725 9750 2    50   Italic 10
toPotResistor0
Text Label 6275 10075 2    50   ~ 0
5V
$Comp
L Device:R R26
U 1 1 62401753
P 6425 10075
F 0 "R26" V 6218 10075 50  0000 C CNN
F 1 "R" V 6309 10075 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 6355 10075 50  0001 C CNN
F 3 "~" H 6425 10075 50  0001 C CNN
	1    6425 10075
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 9875 6650 10075
Connection ~ 6650 10075
Wire Wire Line
	6650 10075 6575 10075
Text Label 6950 8825 1    50   Italic 10
volIRQ0
Text Label 6900 11975 2    50   Italic 10
~volRead
Wire Wire Line
	6900 11975 6900 12075
Text Label 7900 11175 0    50   Italic 10
D1
Text Label 7900 11275 0    50   Italic 10
D2
Text Label 7900 11075 0    50   Italic 10
D0
$Comp
L power:Earth #PWR0120
U 1 1 61E4C51C
P 3175 10575
F 0 "#PWR0120" H 3175 10325 50  0001 C CNN
F 1 "Earth" H 3175 10425 50  0001 C CNN
F 2 "" H 3175 10575 50  0001 C CNN
F 3 "~" H 3175 10575 50  0001 C CNN
	1    3175 10575
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 61C41521
P 3175 10125
F 0 "R4" V 2968 10125 50  0000 C CNN
F 1 "2.2K" V 3059 10125 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3105 10125 50  0001 C CNN
F 3 "~" H 3175 10125 50  0001 C CNN
	1    3175 10125
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 61C41527
P 3175 10425
F 0 "R5" V 2968 10425 50  0000 C CNN
F 1 "2.2K" V 3059 10425 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3105 10425 50  0001 C CNN
F 3 "~" H 3175 10425 50  0001 C CNN
	1    3175 10425
	1    0    0    -1  
$EndComp
Wire Wire Line
	3175 10275 3325 10275
Connection ~ 3175 10275
Wire Wire Line
	3325 9950 3325 10275
Text Label 3175 9975 1    50   ~ 0
5V
Wire Wire Line
	4650 8625 4450 8625
$Comp
L Device:R R23
U 1 1 61CB3154
P 4800 8625
F 0 "R23" V 4593 8625 50  0000 C CNN
F 1 "R" V 4684 8625 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4730 8625 50  0001 C CNN
F 3 "~" H 4800 8625 50  0001 C CNN
	1    4800 8625
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 8825 4450 8625
$Comp
L Device:R R14
U 1 1 61CB315B
P 4300 8825
F 0 "R14" V 4093 8825 50  0000 C CNN
F 1 "R" V 4184 8825 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4230 8825 50  0001 C CNN
F 3 "~" H 4300 8825 50  0001 C CNN
	1    4300 8825
	0    1    1    0   
$EndComp
Connection ~ 4450 8825
$Comp
L Amplifier_Operational:LM358 U300
U 2 1 61CB3162
P 4750 8925
F 0 "U300" H 4750 9292 50  0000 C CNN
F 1 "LM358" H 4750 9201 50  0000 C CNN
F 2 "GERAL:LM386" H 4750 8925 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 4750 8925 50  0001 C CNN
	2    4750 8925
	1    0    0    1   
$EndComp
$Comp
L power:Earth #PWR0121
U 1 1 61CB3168
P 4300 9650
F 0 "#PWR0121" H 4300 9400 50  0001 C CNN
F 1 "Earth" H 4300 9500 50  0001 C CNN
F 2 "" H 4300 9650 50  0001 C CNN
F 3 "~" H 4300 9650 50  0001 C CNN
	1    4300 9650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 61CB316E
P 4300 9200
F 0 "R15" V 4093 9200 50  0000 C CNN
F 1 "2.2K" V 4184 9200 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4230 9200 50  0001 C CNN
F 3 "~" H 4300 9200 50  0001 C CNN
	1    4300 9200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 61CB3174
P 4300 9500
F 0 "R16" V 4093 9500 50  0000 C CNN
F 1 "2.2K" V 4184 9500 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4230 9500 50  0001 C CNN
F 3 "~" H 4300 9500 50  0001 C CNN
	1    4300 9500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 9350 4450 9350
Connection ~ 4300 9350
Wire Wire Line
	4450 9025 4450 9350
Text Label 4300 9050 1    50   ~ 0
5V
Wire Wire Line
	3925 9550 3925 9175
Wire Wire Line
	3925 8825 4150 8825
Connection ~ 3925 9550
Wire Wire Line
	5050 8925 5050 8625
Wire Wire Line
	5050 8625 4950 8625
$Comp
L Transistor_BJT:2N3904 Q2
U 1 1 61D094F3
P 5250 8925
F 0 "Q2" H 5440 8971 50  0000 L CNN
F 1 "2N3904" H 5440 8880 50  0000 L CNN
F 2 "GERAL:TO-92_Inline" H 5450 8850 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 5250 8925 50  0001 L CNN
	1    5250 8925
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0122
U 1 1 61D094F9
P 5350 9125
F 0 "#PWR0122" H 5350 8875 50  0001 C CNN
F 1 "Earth" H 5350 8975 50  0001 C CNN
F 2 "" H 5350 9125 50  0001 C CNN
F 3 "~" H 5350 9125 50  0001 C CNN
	1    5350 9125
	1    0    0    -1  
$EndComp
$Comp
L Device:R R27
U 1 1 61D094FF
P 5350 8575
F 0 "R27" V 5143 8575 50  0000 C CNN
F 1 "1K" V 5234 8575 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 5280 8575 50  0001 C CNN
F 3 "~" H 5350 8575 50  0001 C CNN
	1    5350 8575
	1    0    0    -1  
$EndComp
Connection ~ 5350 8725
Connection ~ 5050 8925
Wire Wire Line
	4625 9950 5800 9950
Text Label 5350 8425 1    50   ~ 0
5V
$Comp
L Device:C C2
U 1 1 61E0587B
P 2900 11825
F 0 "C2" V 2648 11825 50  0000 C CNN
F 1 "C" V 2739 11825 50  0000 C CNN
F 2 "GERAL:standard_2pins_half" H 2938 11675 50  0001 C CNN
F 3 "~" H 2900 11825 50  0001 C CNN
	1    2900 11825
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 11625 3950 11925
Wire Wire Line
	3850 11625 3950 11625
Wire Wire Line
	3550 11625 3350 11625
$Comp
L Device:R R11
U 1 1 61E05884
P 3700 11625
F 0 "R11" V 3493 11625 50  0000 C CNN
F 1 "R" V 3584 11625 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3630 11625 50  0001 C CNN
F 3 "~" H 3700 11625 50  0001 C CNN
	1    3700 11625
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 11825 3350 11625
$Comp
L Device:R R7
U 1 1 61E0588B
P 3200 11825
F 0 "R7" V 2993 11825 50  0000 C CNN
F 1 "R" V 3084 11825 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3130 11825 50  0001 C CNN
F 3 "~" H 3200 11825 50  0001 C CNN
	1    3200 11825
	0    1    1    0   
$EndComp
Connection ~ 3350 11825
Connection ~ 3950 11925
$Comp
L Amplifier_Operational:LM358 U400
U 1 1 61E05893
P 3650 11925
F 0 "U400" H 3650 12292 50  0000 C CNN
F 1 "LM358" H 3650 12201 50  0000 C CNN
F 2 "GERAL:LM386" H 3650 11925 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 3650 11925 50  0001 C CNN
	1    3650 11925
	1    0    0    1   
$EndComp
Wire Wire Line
	3950 11925 4200 11925
$Comp
L Transistor_BJT:2N3904 Q1
U 1 1 61E0589A
P 4550 12225
F 0 "Q1" H 4740 12271 50  0000 L CNN
F 1 "2N3904" H 4740 12180 50  0000 L CNN
F 2 "GERAL:TO-92_Inline" H 4750 12150 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 4550 12225 50  0001 L CNN
	1    4550 12225
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 61E058A0
P 4200 12075
F 0 "R12" V 3993 12075 50  0000 C CNN
F 1 "R" V 4084 12075 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4130 12075 50  0001 C CNN
F 3 "~" H 4200 12075 50  0001 C CNN
	1    4200 12075
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 61E058A6
P 4200 12375
F 0 "R13" V 3993 12375 50  0000 C CNN
F 1 "R" V 4084 12375 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4130 12375 50  0001 C CNN
F 3 "~" H 4200 12375 50  0001 C CNN
	1    4200 12375
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 12225 4350 12225
Connection ~ 4200 12225
$Comp
L power:Earth #PWR0123
U 1 1 61E058AE
P 4200 12525
F 0 "#PWR0123" H 4200 12275 50  0001 C CNN
F 1 "Earth" H 4200 12375 50  0001 C CNN
F 2 "" H 4200 12525 50  0001 C CNN
F 3 "~" H 4200 12525 50  0001 C CNN
	1    4200 12525
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0126
U 1 1 61E058B4
P 4650 12425
F 0 "#PWR0126" H 4650 12175 50  0001 C CNN
F 1 "Earth" H 4650 12275 50  0001 C CNN
F 2 "" H 4650 12425 50  0001 C CNN
F 3 "~" H 4650 12425 50  0001 C CNN
	1    4650 12425
	1    0    0    -1  
$EndComp
Text Label 4650 11725 1    50   ~ 0
5V
$Comp
L Device:R R22
U 1 1 61E058BB
P 4650 11875
F 0 "R22" V 4443 11875 50  0000 C CNN
F 1 "R" V 4534 11875 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4580 11875 50  0001 C CNN
F 3 "~" H 4650 11875 50  0001 C CNN
	1    4650 11875
	1    0    0    -1  
$EndComp
Connection ~ 4650 12025
Text Label 2750 11825 2    50   Italic 10
toPotResistor1
$Comp
L power:Earth #PWR0127
U 1 1 61E058C3
P 3200 12650
F 0 "#PWR0127" H 3200 12400 50  0001 C CNN
F 1 "Earth" H 3200 12500 50  0001 C CNN
F 2 "" H 3200 12650 50  0001 C CNN
F 3 "~" H 3200 12650 50  0001 C CNN
	1    3200 12650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 61E058C9
P 3200 12200
F 0 "R9" V 2993 12200 50  0000 C CNN
F 1 "R" V 3084 12200 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3130 12200 50  0001 C CNN
F 3 "~" H 3200 12200 50  0001 C CNN
	1    3200 12200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 61E058CF
P 3200 12500
F 0 "R10" V 2993 12500 50  0000 C CNN
F 1 "R" V 3084 12500 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 3130 12500 50  0001 C CNN
F 3 "~" H 3200 12500 50  0001 C CNN
	1    3200 12500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 12350 3350 12350
Connection ~ 3200 12350
Wire Wire Line
	3350 12025 3350 12350
Text Label 3200 12050 1    50   ~ 0
5V
Wire Wire Line
	4675 10700 4475 10700
$Comp
L Device:R R25
U 1 1 61E058DA
P 4825 10700
F 0 "R25" V 4618 10700 50  0000 C CNN
F 1 "R" V 4709 10700 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4755 10700 50  0001 C CNN
F 3 "~" H 4825 10700 50  0001 C CNN
	1    4825 10700
	0    1    1    0   
$EndComp
Wire Wire Line
	4475 10900 4475 10700
$Comp
L Device:R R17
U 1 1 61E058E1
P 4325 10900
F 0 "R17" V 4118 10900 50  0000 C CNN
F 1 "R" V 4209 10900 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4255 10900 50  0001 C CNN
F 3 "~" H 4325 10900 50  0001 C CNN
	1    4325 10900
	0    1    1    0   
$EndComp
Connection ~ 4475 10900
$Comp
L Amplifier_Operational:LM358 U400
U 2 1 61E058E8
P 4775 11000
F 0 "U400" H 4775 11367 50  0000 C CNN
F 1 "LM358" H 4775 11276 50  0000 C CNN
F 2 "GERAL:LM386" H 4775 11000 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 4775 11000 50  0001 C CNN
	2    4775 11000
	1    0    0    1   
$EndComp
$Comp
L power:Earth #PWR0128
U 1 1 61E058EE
P 4325 11725
F 0 "#PWR0128" H 4325 11475 50  0001 C CNN
F 1 "Earth" H 4325 11575 50  0001 C CNN
F 2 "" H 4325 11725 50  0001 C CNN
F 3 "~" H 4325 11725 50  0001 C CNN
	1    4325 11725
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 61E058F4
P 4325 11275
F 0 "R18" V 4118 11275 50  0000 C CNN
F 1 "R" V 4209 11275 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4255 11275 50  0001 C CNN
F 3 "~" H 4325 11275 50  0001 C CNN
	1    4325 11275
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 61E058FA
P 4325 11575
F 0 "R19" V 4118 11575 50  0000 C CNN
F 1 "R" V 4209 11575 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 4255 11575 50  0001 C CNN
F 3 "~" H 4325 11575 50  0001 C CNN
	1    4325 11575
	1    0    0    -1  
$EndComp
Wire Wire Line
	4325 11425 4475 11425
Connection ~ 4325 11425
Wire Wire Line
	4475 11100 4475 11425
Text Label 4325 11125 1    50   ~ 0
5V
Wire Wire Line
	3950 11625 3950 10900
Wire Wire Line
	3950 10900 4175 10900
Connection ~ 3950 11625
Wire Wire Line
	5075 11000 5075 10700
Wire Wire Line
	5075 10700 4975 10700
$Comp
L Transistor_BJT:2N3904 Q4
U 1 1 61E05909
P 5275 11000
F 0 "Q4" H 5465 11046 50  0000 L CNN
F 1 "2N3904" H 5465 10955 50  0000 L CNN
F 2 "GERAL:TO-92_Inline" H 5475 10925 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 5275 11000 50  0001 L CNN
	1    5275 11000
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0129
U 1 1 61E0590F
P 5375 11200
F 0 "#PWR0129" H 5375 10950 50  0001 C CNN
F 1 "Earth" H 5375 11050 50  0001 C CNN
F 2 "" H 5375 11200 50  0001 C CNN
F 3 "~" H 5375 11200 50  0001 C CNN
	1    5375 11200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R28
U 1 1 61E05915
P 5375 10650
F 0 "R28" V 5168 10650 50  0000 C CNN
F 1 "R" V 5259 10650 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 5305 10650 50  0001 C CNN
F 3 "~" H 5375 10650 50  0001 C CNN
	1    5375 10650
	1    0    0    -1  
$EndComp
Connection ~ 5375 10800
Connection ~ 5075 11000
Wire Wire Line
	5375 10800 5900 10800
Text Label 5375 10500 1    50   ~ 0
5V
Wire Wire Line
	5900 10800 5900 9225
Wire Wire Line
	5350 8725 6050 8725
Wire Wire Line
	6050 8925 5800 8925
Wire Wire Line
	5800 8925 5800 9950
Wire Wire Line
	4650 12025 5975 12025
Wire Wire Line
	5975 9425 5975 12025
Text Label 7900 11375 0    50   Italic 10
D3
Text Label 7900 11475 0    50   Italic 10
D4
Text Label 7900 11575 0    50   Italic 10
D5
Text Label 7900 11675 0    50   Italic 10
D6
Text Label 7900 11775 0    50   Italic 10
D7
Text Label 7250 9875 0    50   Italic 10
volID
Wire Wire Line
	6650 8825 6950 8825
Wire Wire Line
	7100 9325 6650 9325
Wire Wire Line
	7100 9125 6950 9125
Wire Wire Line
	6950 9125 6950 8825
$Comp
L GERAL:ADC0808 U25
U 1 1 6221B867
P 9650 10575
F 0 "U25" H 9600 10640 50  0000 C CNN
F 1 "ADC0808" H 9600 10549 50  0000 C CNN
F 2 "GERAL:standard_DIP_28pins_thin_doubleBody" H 9675 10625 50  0001 C CNN
F 3 "" H 9675 10625 50  0001 C CNN
	1    9650 10575
	1    0    0    -1  
$EndComp
Text Label 10025 10975 0    50   Italic 10
toPotResistor0
Text Label 10025 10875 0    50   Italic 10
toPotResistor1
Wire Wire Line
	9600 11275 9600 11375
Wire Wire Line
	9600 11375 10025 11375
Wire Wire Line
	9175 11275 9600 11275
Text Label 9175 12075 2    50   Italic 10
cD1
Text Label 10025 12075 0    50   Italic 10
cD2
Text Label 10025 11875 0    50   Italic 10
cD0
Text Label 9175 11475 2    50   Italic 10
cD3
Text Label 10025 11775 0    50   Italic 10
cD4
Text Label 10025 11675 0    50   Italic 10
cD5
Text Label 10025 11575 0    50   Italic 10
cD6
Text Label 10025 11475 0    50   Italic 10
cD7
Text Label 6900 11075 2    50   Italic 10
volID
Text Notes 10225 11475 0    50   Italic 10
2^-1 (maior fração 1/2)
Text Notes 10225 11875 0    50   Italic 10
2^-8 (menor fração 1/512)
Text Label 6900 11175 2    50   Italic 10
cD1
Text Label 6900 11275 2    50   Italic 10
cD2
Text Label 6900 11375 2    50   Italic 10
cD3
Text Label 6900 11475 2    50   Italic 10
cD4
Text Label 6900 11575 2    50   Italic 10
cD5
Text Label 6900 11675 2    50   Italic 10
cD6
Text Label 6900 11775 2    50   Italic 10
cD7
$Comp
L 74xx:74LS244 U26
U 1 1 6243F3C3
P 7400 11575
F 0 "U26" H 7400 12556 50  0000 C CNN
F 1 "74LS244" H 7400 12465 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 7400 11575 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74ls244.pdf" H 7400 11575 50  0001 C CNN
	1    7400 11575
	1    0    0    -1  
$EndComp
Text Label 8275 11925 2    50   Italic 10
~volRead
Wire Wire Line
	8875 12025 8875 11575
Wire Wire Line
	8875 11575 9175 11575
Text Label 9175 11875 2    50   Italic 10
5V
Text Label 9175 11975 2    50   Italic 10
GND
Wire Wire Line
	9175 11775 9175 11875
Text Label 10025 11975 0    50   Italic 10
GND
Text Label 7700 9225 0    50   Italic 10
volIRQ
Wire Wire Line
	9175 11675 8950 11675
Text Label 6650 9325 0    50   Italic 10
volIRQ1
Text Label 10025 11175 0    50   Italic 10
GND
Text Label 9175 11375 2    50   Italic 10
~volEOC
Text Notes 7600 10625 0    50   Italic 10
quando um dos dois botões interrompe,\no DAC faz a leitura da tensão em (IN0) ou (IN1)\n(segundo ADD_A=volIRQ1); acabada a conversão é\ngerada interrupção por EOC, e a BIOS tem de\ntratá-la lendo a porta - isto coloca o chip a saída ~OE~ \npara databus; o ID do botão vai no bit LSB\n(é ignorada a fração mais baixa do DAC)
Text Label 1900 4850 2    50   Italic 10
CLK88
Wire Wire Line
	8950 12375 8975 12375
Wire Wire Line
	8950 11675 8950 12375
Text Notes 17100 3125 0    50   Italic 10
no disp de segmento:\n diz-se "segmento" a um parte da forma do digito (existem 7 segmentos)\n e tem letras associadas de A a G\n\n dispA..G é o número segmento (ativar com pullup)\n dispDigit0..3 é o digito o selecionar a GND, restantes a VCC)\neste segmento é catodo comum \n(DIGITO está a GND)
Text Notes 1475 8500 0    50   Italic 10
cada potenciometro tem 2 pistas omicas (como os stereo)\numa das pistas dum potenciometro está ligada como divisor de tensão\n- uma montagem opamp diferencial monitoriza as mudanças\n- para que se aceitem mudanças num sentido ou noutro existe \n  uma montagem opamp inversora \n- os dois transistores ficam em curto quando existem então \n  mudanças num ou noutro sentido \n  e a interrupção é gerada após o OR destes sinais
$Comp
L power:Earth #PWR0130
U 1 1 61FCC8C4
P 8750 13800
F 0 "#PWR0130" H 8750 13550 50  0001 C CNN
F 1 "Earth" H 8750 13650 50  0001 C CNN
F 2 "" H 8750 13800 50  0001 C CNN
F 3 "~" H 8750 13800 50  0001 C CNN
	1    8750 13800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 13750 8750 13750
Wire Wire Line
	8750 13750 8750 13800
Wire Wire Line
	9125 13375 10275 13375
Wire Wire Line
	8750 13450 9925 13450
Wire Wire Line
	8550 12925 9125 12925
Wire Wire Line
	8750 13025 9125 13025
Text Label 9125 13125 2    50   Italic 10
adcCLK
Wire Wire Line
	9925 12650 9925 12725
Text Label 9925 12650 0    50   ~ 0
5V
Connection ~ 9925 12725
Wire Wire Line
	9775 12725 9925 12725
Wire Wire Line
	9775 12925 9775 12725
Wire Wire Line
	10275 12725 9925 12725
Wire Wire Line
	10275 13375 10275 12725
Wire Wire Line
	9125 13225 9125 13375
Wire Wire Line
	8550 12925 8550 13750
Connection ~ 8750 13450
$Comp
L Device:C C3
U 1 1 61FCC8DD
P 8750 13600
F 0 "C3" H 8865 13646 50  0000 L CNN
F 1 "C" H 8865 13555 50  0000 L CNN
F 2 "GERAL:standard_2pins_half" H 8788 13450 50  0001 C CNN
F 3 "~" H 8750 13600 50  0001 C CNN
	1    8750 13600
	1    0    0    -1  
$EndComp
Connection ~ 9125 13025
Wire Wire Line
	8750 13450 8750 13025
Wire Wire Line
	9925 13325 9925 13450
Connection ~ 9925 13025
Wire Wire Line
	9775 13025 9925 13025
$Comp
L Device:R R31
U 1 1 61FCC8E8
P 9925 13175
F 0 "R31" H 9995 13221 50  0000 L CNN
F 1 "R" H 9995 13130 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 9855 13175 50  0001 C CNN
F 3 "~" H 9925 13175 50  0001 C CNN
	1    9925 13175
	1    0    0    -1  
$EndComp
$Comp
L Device:R R30
U 1 1 61FCC8EE
P 9925 12875
F 0 "R30" H 9995 12921 50  0000 L CNN
F 1 "R" H 9995 12830 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 9855 12875 50  0001 C CNN
F 3 "~" H 9925 12875 50  0001 C CNN
	1    9925 12875
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 13125 9775 13125
Wire Wire Line
	9500 13025 9500 13125
Wire Wire Line
	9125 13025 9500 13025
$Comp
L GERAL:555 U23
U 1 1 61FCC8F7
P 9475 13075
F 0 "U23" H 9450 13490 50  0000 C CNN
F 1 "555" H 9450 13399 50  0000 C CNN
F 2 "GERAL:standard_DIP_8pins" H 9475 13475 50  0001 C CNN
F 3 "" H 9475 13475 50  0001 C CNN
	1    9475 13075
	1    0    0    -1  
$EndComp
Connection ~ 8750 13750
Text Label 8975 12375 0    50   Italic 10
adcCLK
$Comp
L Amplifier_Operational:LM358 U300
U 3 1 61F42BBF
P 1425 10800
F 0 "U300" H 1525 11225 50  0000 C CNN
F 1 "LM358" H 1525 11150 50  0000 C CNN
F 2 "GERAL:LM386" H 1425 10800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1425 10800 50  0001 C CNN
	3    1425 10800
	-1   0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:LM358 U400
U 3 1 61F76ED0
P 1725 10825
F 0 "U400" H 1775 11275 50  0000 C CNN
F 1 "LM358" H 1775 11200 50  0000 C CNN
F 2 "GERAL:LM386" H 1725 10825 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm2904-n.pdf" H 1725 10825 50  0001 C CNN
	3    1725 10825
	-1   0    0    -1  
$EndComp
Text Label 1525 10500 1    50   ~ 0
5V
Text Label 1825 10525 1    50   ~ 0
5V
Text Label 1525 11100 3    50   ~ 0
-5V
Text Label 1825 11125 3    50   ~ 0
-5V
Text Notes 14250 8125 0    50   ~ 10
5V\n |\no--RESIST---LED>|---PIN
Text Notes 14000 7725 0    50   Italic 10
conetar cada linha a \nresistência de 3xx-Ohm\ne aos 5V, outro lado a GND\no simbolo do 7407 à esquerda \ntem bola mas não é invertido!
Text Label 17975 3450 0    50   Italic 10
dispDigit0_12
Text Label 15775 5225 0    50   Italic 10
dispDigit0_12
Text Label 7350 2425 0    50   Italic 10
~volRead
$Comp
L power:Earth #PWR0131
U 1 1 621ABC41
P 7400 12375
F 0 "#PWR0131" H 7400 12125 50  0001 C CNN
F 1 "Earth" H 7400 12225 50  0001 C CNN
F 2 "" H 7400 12375 50  0001 C CNN
F 3 "~" H 7400 12375 50  0001 C CNN
	1    7400 12375
	1    0    0    -1  
$EndComp
Text Label 1900 3350 2    50   Italic 10
-5V
Text Label 7400 10775 0    50   ~ 0
5V
$Bitmap
Pos 6250 1625
Scale 2.000000
Data
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 00 00 01 1F 00 00 00 94 08 02 00 00 00 25 35 BA 
B5 00 00 00 03 73 42 49 54 08 08 08 DB E1 4F E0 00 00 00 09 70 48 59 73 00 00 0E 74 00 00 0E 74 
01 6B 24 B3 D6 00 00 17 64 49 44 41 54 78 9C ED 9D CF 8B E4 46 96 C7 9F 96 FD 0B 3C B8 1B 83 3D 
66 8C B2 30 45 7A 69 BA 0F 66 95 66 19 86 E9 DE C9 AC 5D 28 E6 90 87 9E 1D 0A E6 A0 5A 86 C5 99 
E3 EE 3A AC 29 6C 2F 14 7B A9 36 A3 34 78 21 75 72 B1 36 03 79 18 0A A6 4B E9 69 7B 0E 76 93 B9 
EC C1 A6 A7 9D 14 4D 49 EE B5 F1 C1 94 A1 FB 5F D0 1E F4 23 43 8A 08 29 24 45 64 66 A5 DE 07 83 
3B 94 11 4F 52 46 BE 52 C4 D3 37 5E 68 BE EF 03 82 20 0A F8 9B 65 5F 00 82 AC 2D E8 5D 08 A2 0A 
F4 2E 04 51 05 7A 17 82 A8 02 BD 0B 41 54 81 DE 85 20 AA F8 5B E6 51 4D D3 16 7C 1D 48 39 82 17 
2A D8 5F 2B 42 EA FD 16 DB BB E8 7A 6B 86 A6 69 6B 70 83 A4 53 AD C1 ED 5C 74 E8 BF 71 38 32 BC 
D8 2C E0 A9 25 FD 14 F5 79 D2 E6 79 D7 57 6F 5F BE 75 ED C6 A7 DF 8A 59 FB BC 7F E7 DA E5 5B D7 
6E 7C F4 79 50 FC AF 3B D7 2E DF BA 76 E9 CE D1 E3 64 BD 7B 1F 5D BB 7C 2B FC 2F 30 7E EF A3 5F 
11 35 BF 0D 1A 5E BE 75 8D BC 92 4B B7 DE BE 27 78 5B 08 88 F4 9D 68 7F 05 88 F7 5A FF 2B C6 C5 
D4 A1 FB D2 7F 38 7C 16 F1 F1 AF 3F B9 79 FD F0 E6 B3 1F 7E C6 AC 96 E2 EB 4F 82 9A DF BC 7F 78 
B5 F7 D0 FF FA 93 9B CF 1E 7E F0 75 54 64 70 FE C1 F5 37 6E BE 7F EE FB 0F DF 4A D4 3C FF E0 FA 
1B 6F FD 39 51 F5 9B F7 0F AF 3E 9B 3E 58 1E DE 8D 5F 2C E2 1E 64 DE 4E 6E DF 09 F6 57 DA 78 81 
5E 8B 49 74 DF 7A 7C F9 34 B4 43 65 3F BB BE FD F8 C1 A3 CD 2B 37 AE 7C F9 97 A2 7F 75 4E CF BF 
7D E9 B2 9E 5D E7 DE 27 EF 41 FB 3F 7F 7B 09 1E 9F 7B F0 DC 4F 5E 82 17 7F 7B FB 0B EB 15 80 F3 
C7 7F 85 BB BF BE 75 ED D2 9D B7 83 9A 8F 3F 7D F3 18 5E 2E 78 09 35 A7 58 DF 89 F4 57 80 48 AF 
A5 CE 58 DB EE CB F1 AE B3 EF FF A9 F3 F3 9F 6C 82 E7 FD 90 6F EB A5 9F EF DE FC FE 77 97 6E BD 
79 FC 3D 00 00 BC F2 9B 77 E0 BD BF BF F5 CB 77 9E 7B FD F5 57 A8 DA 3F 1C DD F9 F2 E5 ED BF 7B 
11 00 BC F3 47 F0 FD F0 46 DC 31 AF FC C7 F9 BB 5F 9C BF FB C7 77 E0 2E 00 00 7C FE DE 03 FD F6 
3F 0A F5 3D 12 91 DF 77 C5 FA 2B 40 AC D7 EE 24 86 A3 F5 ED BE 4C EF FA EA 2F 7F 80 BB BF BE F5 
BB 3F C0 A3 E3 87 22 53 AF 7F B0 6E 7F F1 C3 BB 1F DD BE 0A 00 F0 F8 D3 37 DF 79 EE F7 E7 EF FE 
F1 9D EF DF FB 57 6A F4 FF F8 E1 BD 07 CF DD F8 C5 25 00 00 FD F2 CB F0 DC EE BD 74 C7 BC F8 8B 
2B 2F 03 C0 E3 4F 87 A7 57 7E 73 A3 E0 8D D5 1D A1 BE 2B D0 5F 01 82 BD F6 E0 7C DE BC CE DD 97 
E5 5D F7 1E DE F5 AF FE FE FC DD 2F FE A7 9D F8 BE 78 3C FE EA ED 4B 77 8E 1E C3 E7 CE 97 B0 79 
F9 45 EF FC 51 46 65 EF FC D1 95 2B 3F 7D 09 00 00 5E BA AC 03 FC 5F 3C 93 7E FC E9 AF 2E DD 39 
7A 0C DF 7E FC E0 11 00 7C FC E0 D1 5F C7 BF BC FC C1 5D 0D EE FE 0B 67 C2 8D A4 10 E9 BB 42 FD 
15 20 D8 6B 57 2E BF 18 37 A9 75 F7 F1 E6 67 BE EF BF 7F 78 F5 FA 27 DF F8 BE EF 3F 7C 4B 2C A2 
F0 D9 FB 87 57 2F BD 71 F5 7A 38 93 FE AC 77 78 F5 D2 1B 57 9F 3D A4 DB A6 A6 CE DF FC F9 C3 9B 
44 CD B0 E1 F5 0F 3F 98 B7 10 BD 06 21 78 37 7E B1 88 7B 90 BE 1D B1 BE 13 EA 2F C2 B8 68 AF 7D 
4D 9F 8A B8 8C F5 F8 F2 69 68 87 62 BF 54 5D 8F 97 AD 19 AC C7 0D 06 F1 5F DF F7 95 DE 8E 74 E3 
EB F1 E5 D3 C4 DD 11 1F E1 6A 35 90 8B 85 D2 57 B4 F5 79 FF 2B 17 F4 AE 35 61 95 9F 5D 4F 5F 68 
3C F3 9D 4B 1A AC 7C 51 17 03 54 42 21 88 2A D0 BB EA 8C 37 D8 6D 69 9A D6 DA 1D 78 8C 62 89 26 
B4 05 6F B0 DB 02 80 4C 9B EB 0B 2F FA A1 30 B6 92 E4 FE FD FB 0B 3B 57 CC 22 6F 50 1D 71 0F 96 
BC 1D C7 04 C3 72 7D D7 32 C0 74 A8 22 71 16 D1 26 B4 05 C7 04 C3 7A F2 BC 4E DA E4 5F AD 6B 99 
06 00 18 A6 E5 32 8A E9 BA C6 FC 57 6C 98 8E 1B 1F 9B 9F 3A 2E 38 26 00 18 2C 33 12 A1 1D 0A 9F 
5D F5 C5 3B 9B 41 73 43 07 7D A3 09 B3 33 2F 55 2C D1 84 B6 10 1C 01 80 0C 9B 73 C6 87 FD 59 D7 
F5 DD EE AC 7F 38 A6 8A 0C 42 3F 72 9D EE AC B3 13 3F 1C 83 13 8D 8F 6D F2 B2 0D C3 98 9E BA 4C 
2B EA 40 EF AA 2F EE E9 34 A3 58 A2 09 6D 41 C4 66 4C 09 6F 0F D0 DB 5B 5D 63 3A 3A F1 00 00 4C 
B3 39 3A F1 C0 3B 9B 19 A6 19 3C DE BC 93 D1 B4 D9 ED 1A F6 31 DB 49 95 81 DE 55 5F 1A 9B 46 46 
B1 44 13 DA 82 88 CD 98 12 DE 1E A1 6F 34 E3 7F 6F 6E C2 A9 EB 9D 8C A6 CD CD CD D8 92 B9 DD 13 
7A 7C CA 45 AE 77 31 A6 C5 E3 5D 4D DB 65 FC C9 48 1D 2F 54 64 9D 8B 5B CC 9D AC CF 8D 7B 83 96 
36 A7 35 F0 B8 6D BD 41 2B FA 9C 68 D2 DA 1D 7B 91 45 31 3B 4B 25 7A 28 84 0F 89 54 B1 44 13 DA 
42 70 04 00 32 6C C6 94 F0 F6 08 EF 6C 16 FF 7B 63 AB 3B 3B 3E 3C 9D 9A DB 5B C1 81 F1 B1 0D 76 
47 EB D8 10 3D DF 16 06 6F 7E 56 66 5A 47 4D 6A C3 99 27 39 47 F6 D3 C7 EF DF BF 9F AA 96 5D 64 
9F 8B 5F 64 4E D6 E3 1B 4C 18 4F CF E7 B9 13 7D DF B5 8C 60 92 4C 7C E2 3A D1 41 C7 4C 5F 2E CB 
4E 38 67 8F A6 E4 C1 95 04 33 EF F9 BF 83 A9 79 54 2D F5 35 C6 3D 58 B2 BF E6 61 03 C7 65 14 D3 
DF 95 40 13 DA 82 6B 99 C6 93 E7 75 D2 26 F7 6A C5 A2 2C B1 61 23 F1 13 21 AA 3A 66 D0 A5 41 31 
EE 0D 8E 21 69 D0 0E 25 F3 D9 95 1E 28 8F 77 77 4E BB 96 49 D5 A3 8E 27 8A A9 4F 39 46 C4 A7 D7 
59 C3 77 DE 15 F2 EE 28 93 C4 E8 3F D7 CE 78 B7 D1 87 7D D7 F7 7D B7 3B EB C4 B3 F6 A0 BD 7B 0A 
46 FC 87 3B 72 DE EE AC C3 1A 04 54 41 EF 0D 27 BE EF 4F 86 6D 9D 51 2C D1 84 B6 A0 F7 86 13 00 
C8 B4 19 D1 DE B3 9A A3 86 D6 18 35 9D BD 36 55 64 60 77 82 E1 41 63 D4 74 8E 7A F1 09 DA DB 26 
18 9B 8D A0 70 76 36 0B 0B FA 46 13 16 3B F5 92 E9 5D E9 81 72 7B 38 19 F6 36 E8 7A D4 F1 44 31 
F5 29 C7 88 F8 F4 3A 6B F8 CE 30 1E 75 98 D6 1A 78 C5 26 E5 C9 D1 7F 8E 1D EF 6C 66 58 7B 6D 1D 
00 F4 DE C4 1F 06 BF 1E D3 34 A7 A3 13 6F 7C 6C 37 9B CD 54 0B 7D 6B 09 D3 F2 05 53 C4 DB F5 DE 
64 FE 90 08 AB 44 5F 65 7B E8 4F 7A 7A 50 EC F5 26 FE 24 F0 BC F6 D0 8F BE E9 C5 20 53 09 D5 D8 
34 E0 54 A2 BD 9C 73 3D B9 F3 FA D3 17 E0 55 D0 FF E4 B4 01 F4 27 C0 2E 02 00 F9 D1 D3 3B 40 AA 
72 28 4C 87 E8 00 AF D8 1D 79 67 33 80 CD 2A 76 B6 B7 4D FB F8 F0 D8 36 B7 1D B0 67 F9 F5 91 95 
46 E6 B3 4B 64 5A 2C F1 5C 3F FA F1 BF 3D FD 6E FC DF 3F F6 FE DD 76 9F B1 7F C6 2B A6 3F CA 72 
2D C6 59 0A DC 91 77 32 9A 1A DD 2D D6 DF 59 D6 74 7F DA 3F 1C 7B 40 44 48 00 00 1A 9B 86 6D DB 
F1 B0 26 65 DD DC 5E E4 5F 5E A4 2A 52 63 86 F9 03 65 65 E7 E2 17 2B 5D 95 50 5B E6 E8 3F 1E 19 
6A AD 81 C7 B0 D3 1E BA 16 1C 34 D2 CD F4 AD AE 01 49 17 0D 2D 35 46 4D 47 F6 B0 46 BA 12 0A 20 
1D E0 45 25 14 2B FA 21 3F A4 C2 01 95 50 A5 89 7B B0 E4 ED 48 57 42 D1 01 DE A2 4A A8 38 8A 1A 
14 33 94 50 44 34 35 AC 1F 07 72 7D 22 20 3B 8F 5F 66 5A 93 01 ED 50 F8 36 B9 BE 48 57 42 D1 31 
D8 42 4A 28 6F B0 D3 07 CB F5 7D A7 69 1F 0C 3C 61 25 94 6B 35 ED 64 38 75 1E 90 F5 DD 48 24 95 
6F 4D 3E E8 5D F5 45 BA 12 8A 8E C1 16 0A BA EA 61 74 2F 7C 35 5C E0 75 C8 0C C8 99 AA 37 38 B0 
A3 80 6C C2 A8 F0 CB 15 59 A0 77 D5 17 E9 4A A8 DC FA 22 78 83 9D FE D4 E8 6E E9 02 9E 19 4C 49 
1B 7D 30 F7 7B C9 60 12 15 83 2A F8 72 45 0E E8 5D F5 45 BA 12 8A 57 1F C4 94 50 00 C1 98 6E 6A 
58 47 3D 5D C4 33 A3 A9 DC 84 0A F7 50 8F A7 12 7E 5E 1D F9 DE 45 84 8C B2 62 50 AB A1 33 8C 64 
82 AD DD 31 80 B8 CE 90 A1 2D 4C C4 D5 C7 81 A1 B0 15 29 3B 6C ED 8E 53 07 C2 36 FC 26 71 9D F0 
5A E4 C9 15 0B 6B 23 8A 37 69 EF 59 CD 11 00 08 05 6C BD 41 AB 63 83 E9 04 2F 7F 4B BF E0 D1 B7 
BA 46 F4 BA 03 BC 41 F0 DB 59 E4 EB A2 39 BC E8 47 B9 B0 49 22 64 C4 D7 89 AD 88 CE D0 25 84 81 
EC 2B E5 7C C0 D0 16 92 01 AB A0 95 1B 09 DC E6 B2 43 D7 22 15 88 42 4D C8 AF 6C AE E6 33 C0 F0 
25 E8 0C 85 A8 6E FC C9 F3 BA 88 41 72 3D 64 F8 9D B2 74 8F 21 9C AF 88 11 33 B4 52 31 43 96 35 
19 D0 0E 25 F5 D9 95 0C 19 71 E7 91 2B A3 33 4C 4D A3 69 72 A7 C2 5C 6D E1 74 74 C2 14 DE 10 F3 
EF F1 61 1F 2C 42 1D C7 6B 12 5C C9 C9 68 6A EE 0F 89 99 3A FB 8A 2F 32 A4 B6 69 D2 D3 73 94 50 
F4 57 A5 CF 35 4F 51 53 DF F7 27 C3 E4 21 21 C5 A3 24 E4 BE 4D 4E 84 8C B8 F3 C8 D5 D1 19 02 00 
31 8D 06 80 E2 3A 43 52 5B 18 5F F3 9E 63 42 BF 31 1F F6 31 E6 DF DE E0 C0 26 DE 1A 73 9B 50 57 
12 0D 1A F3 2E 0C 59 3E 0A 33 AE 29 95 1D A6 8C BF EA 94 D5 19 86 D3 68 B7 A7 03 78 50 5C 1F 48 
6A 0B 63 F4 F6 70 E2 EF 8D 77 77 3A 3B 83 AD C9 46 DA 2C 44 AA A9 23 5D B4 C9 FC 4A DA 43 DF DF 
1B B4 68 A9 14 B2 72 28 8C 19 2A 9D 47 A6 8C 97 D4 19 26 A7 D1 65 6E 81 A5 2D 0C 63 30 7A 23 ED 
74 04 EE 69 A2 59 6E 13 7D AB 6B D8 9D 20 82 E2 8D 4F 46 72 C2 CB D5 95 50 C4 C5 B3 3F 45 25 14 
6B 7E 56 7A 6E 47 CC 36 B3 E6 91 71 B5 40 09 95 9A A3 66 17 59 C6 B9 45 E6 65 40 1C D5 88 E0 C6 
5F 58 B7 40 67 23 4A 4C A9 7D C7 24 15 3D 8C AB 67 C5 4F D2 4D 20 71 6D 3E 39 53 07 C3 B4 FC 55 
50 42 E5 C6 B1 8A 29 A1 A2 26 F3 20 10 57 BB 64 25 02 EC A6 43 2E A0 A4 D3 42 D1 DF 67 22 A5 94 
1C 79 14 ED 50 A8 33 BC C0 54 F4 AE D8 09 C2 80 65 B2 48 9E 85 D7 C4 77 4C C3 B4 E2 40 28 6D 21 
38 F2 E4 79 9D B4 99 79 B5 84 77 64 AF 4D F6 D3 41 42 D2 BB 82 A3 A1 4F 85 DE 45 07 18 E3 A8 34 
EB AF 77 71 68 EF C2 B7 C9 F5 A5 BA 12 2A 37 8E 55 54 21 E1 0D 76 46 10 3E 54 CA 6B 97 18 69 A1 
96 03 7A 57 7D A9 AE 84 CA FD B4 A0 42 62 7C 38 6A EE EF 87 41 D8 0A DA 25 3A 2D 14 A4 62 B0 89 
23 8D 03 B0 5C 15 8B 96 D1 BB EA 4B 75 25 54 EE A7 85 94 50 DE E0 60 D6 9D 2B 3A 2A 68 97 A8 B4 
50 00 C4 D0 2F 8E 61 05 93 72 03 00 40 8D 7A 03 BD AB C6 54 57 42 E5 7E 5A 44 09 75 32 9A 4E FB 
0D AD 63 03 D8 9D D6 00 2A C4 9C F5 8D A6 6D DB 22 4B B9 F5 DE 91 65 4C 6D E9 09 81 02 78 F3 B3 
D2 73 3B 81 70 5F E2 78 E5 98 21 33 E7 F8 BC C8 0C 3D 45 37 E8 04 7F B9 8C 44 FC 2B 24 29 C6 49 
C6 94 12 19 D7 E6 2D C2 F8 21 23 E2 A7 64 E9 5E DC 83 55 FA 4B E4 2C 15 2D 08 2A A1 42 18 31 43 
8E 76 89 17 D5 A0 03 A0 EC 98 21 B9 C6 53 42 96 79 DA A1 54 EA 0C 57 3E 9F A1 3B 57 F7 A5 93 13 
B2 CF 92 B8 01 B9 F9 0C 93 9E 9D FA 41 04 4E 99 8A E5 AD AB 77 5D 58 68 EF 52 A8 33 5C FD 7C 86 
7A 98 99 8B 27 33 5C 64 3E 43 BD 37 21 7C 35 98 62 C7 12 DF A6 BD E8 24 B2 88 0C 14 EA 0C 2F 40 
3E 43 80 70 BD B9 B9 1F E9 26 96 9A CF 30 C5 B4 DF D0 34 4D D3 0E 66 56 7A 75 20 72 11 B8 A8 51 
8D C6 A6 F1 C4 79 FD E9 0B 8D 57 1D FD 4F 77 DA AF 3A 3A AF 98 FA E8 E9 0B 29 85 9E DE 9B B8 C4 
06 35 89 C8 52 C1 B0 15 A9 B5 AF 62 27 22 4A 75 3D 6D 6E A8 49 B1 25 5D 09 45 17 55 2A A1 A8 85 
73 8C 23 89 75 77 8B E6 A2 7A 97 94 7C 86 DE A0 A5 65 F6 FA E2 F3 19 52 B4 87 8E 09 F6 81 92 DF 
47 E1 FD B2 18 75 02 A9 66 4C AA 18 D4 07 00 B1 5C 31 F3 E5 AB A1 67 E4 7B 3B 9D B8 86 97 CA 66 
09 5C 54 EF 92 92 CF 50 EF ED 9B B3 7E 43 6B 8C 9A E4 42 2B FE 59 D8 C8 CC 67 C8 BE 06 63 1A FE 
36 A3 B1 A2 94 BF C6 D5 73 42 E5 4E 92 8B ED 8E E7 9D 8C A6 E1 33 7F D8 2E 98 C5 29 99 B8 86 7D 
64 F1 F0 A2 1F 92 E3 29 7C 50 67 58 9A B8 07 CB DD 4E 1C DA 0C 82 91 A9 22 79 16 5E 93 D4 41 BA 
18 FC 5B 54 67 18 87 49 8D E8 3D 2F EB 7A 18 F5 81 A5 7E A6 93 1C 2A 86 76 A8 0B FB EC 42 2A 23 
5D 09 55 B5 7E 7B E8 FB BE EF BB 16 D8 07 03 4F 2C A4 44 27 AE E1 A7 B2 59 38 E8 5D F5 45 BA 12 
8A 57 1F C4 73 42 01 84 59 67 4E DD A5 64 71 92 0B 7A 57 8D 91 AE 84 E2 D4 07 31 25 54 14 DE F1 
4E 46 53 63 B3 B1 9C 2C 4E 72 E1 8D 20 4B 8D 3C B3 D5 49 EC 6A 4B 55 42 A5 3F 4A 8C DB 73 94 50 
44 4D 93 5A 8E 17 2D 78 A2 B2 99 CB 24 EE C1 B2 FD 25 7A 96 8A 16 84 B5 1A E1 12 52 AA 13 55 65 
71 92 0B ED 50 2A 77 76 E5 C9 88 92 C7 97 B9 B3 6B AE D0 29 AB 02 6B ED 5D 6A 85 32 00 6B E5 B2 
34 D6 CE BB 2E 36 B4 77 29 DC D9 95 17 E1 A5 8F 2F 4B 09 C5 FB 28 CE 85 56 25 F9 38 33 9B 39 52 
2B 14 EE EC CA 8B F9 D0 C7 97 A5 84 62 7F 44 E4 42 CB 0C 5B 09 AC BD BB A8 D3 05 44 0E 32 BD 4B 
30 C2 2B 25 16 24 45 09 C5 BC 12 52 71 91 79 A9 02 6B EF 16 B6 D9 46 49 4A 28 A1 00 B2 92 40 61 
4E A8 04 0A 77 76 E5 C5 7C A4 C4 82 A4 28 A1 98 57 42 E6 42 CB DF 85 80 BF F6 8E 99 CD 7C B5 28 
A1 84 4A 69 9D 72 2D 14 53 42 8D A3 B4 FE 02 DE 9E 48 FA 1F 66 F3 0F 8E 85 DF 73 A0 39 0C 0A E3 
5D AE C0 25 B5 79 80 D4 3C FE 2A 77 76 E5 45 6F A5 6C 00 2B 65 67 57 C6 47 49 47 CA BF 54 BD 37 
E1 08 01 F5 DE 24 12 3B 69 8D 91 A2 CC 0D 55 28 A1 84 CA 4E 66 4E 5B 28 B8 3B DE 41 1F 2C D7 77 
2D 10 F4 F6 78 65 1D 21 C3 8E 46 0C E3 E3 B9 DE D1 3B 9B 19 86 31 3D E5 6D 99 3D 0F 50 35 ED F8 
5C F3 30 D6 A4 A7 83 37 68 35 46 F3 1D F7 FA 3B 42 FE C5 8B 7E A8 09 AB 30 40 25 54 69 E2 1E 2C 
77 3B 25 94 50 74 C3 6C 0B 85 95 50 44 84 36 47 09 95 5A 89 6A C4 49 E3 4C 33 34 62 98 A6 11 8F 
DF 4D 8B 93 B8 8D 19 FE A5 5E 01 F1 D3 BE CD A1 1D 0A DF 26 D7 97 12 4A A8 A2 16 0A 2A A1 F6 2C 
E8 37 B4 46 BF 69 ED B5 0B 2D AE 23 57 D6 D1 09 A1 DC D3 A9 B9 DD CB 78 7C 32 03 54 39 79 FC 45 
84 D4 E8 5D F5 A5 84 12 AA A8 85 82 39 A1 76 FA 4D C7 F7 5D 6B D6 DF 19 78 45 3C 93 5C 59 47 25 
84 1A 1F DB 60 77 B4 8E 0D EC 95 E3 9C 00 15 67 85 5E 7B 98 16 13 F0 41 EF AA 31 25 94 50 45 2D 
14 51 42 A5 1E 56 05 BC 3D B9 B2 2E 95 10 6A 7C 6C C7 A9 82 F9 53 AF 9C E4 50 25 F3 F8 F3 46 90 
D9 43 4C 0E 0C DD 50 AE 88 49 B5 12 8A A9 C9 02 CE AA 8A 18 BE 0C 8A 92 63 44 33 8C 79 8E F7 B9 
6E 83 CE AE 4C 9C C2 8C 93 41 13 C7 8A 88 7E E2 1E 2C DB 5F A2 67 A9 68 A1 A8 12 6A 9E A3 2B 43 
09 C5 FA DE 58 CA 1D 6B DE 5F CC FC 64 CC E4 50 02 79 FC E9 6B A2 1D 4A A5 12 4A 4C C4 A4 5A 09 
C5 D4 64 C5 37 C8 30 9E 27 83 22 DC 8B 4C F3 15 75 70 62 37 4A 81 DC E5 CC AC 52 62 AC 9D 77 5D 
6C 68 EF 52 A8 84 12 14 31 81 62 25 54 56 AC 99 65 3C 57 06 A5 C7 A9 9F A2 01 09 B9 31 A4 DE EE 
ED 9B BC F1 7D 0E 19 59 A5 10 39 24 DF 6E A9 7E 07 29 73 77 3C 46 0A FF 36 8C 77 FB 39 D5 00 26 
C3 DE BC 5A AA 15 DF C8 13 87 DC 0E 0F 92 BB E3 71 8B 89 9D EC 68 E3 81 0C CA CD 94 41 E9 5B 5D 
A3 3F 3A F1 B6 20 DC E1 8E AE 39 3D 75 61 8B D9 D8 EE 68 C1 7B 18 C3 0C C2 53 C9 1C 70 8C 9D 2C 
11 89 E8 BD 89 DF 5B D8 D9 14 2A A1 2A 56 CB 35 F2 A3 CE 7B CF 7C E7 FE 6F C7 FB E7 DB E3 A7 B7 
2F 09 16 B3 CD 8A C9 A0 82 A7 D7 E1 61 54 97 AE C9 CF E8 90 AD 9F E2 EE E0 AC 06 E9 4A A8 D4 A7 
80 4A 28 69 08 06 79 64 29 A1 32 62 C1 A5 63 CD 82 32 28 7D AB 6B 4C 6D 3B AA 9B 0C 28 ED 76 6C 
76 66 A8 79 73 5E 78 8A 9F 55 4A 09 D2 95 50 55 73 42 A5 24 4B 7C 6F CF CE AC 46 4A 96 C6 5E A2 
D1 62 D5 68 2A 95 50 15 AB 15 3A 57 A1 22 17 61 19 94 BE D5 35 20 DE 61 1C F4 DE 91 63 CE 3A 0D 
4D D3 1A 9D 19 B1 53 6C 32 33 14 D1 3E A9 9F 62 66 95 52 8E 74 25 54 D5 9C 50 29 C9 52 8E B7 F3 
32 AB CD B3 1C FB 44 A2 CA B4 DB 2F 06 5E F4 43 79 84 25 02 95 50 A5 89 7B B0 DC ED 48 57 42 A5 
3E F5 8B 2A A1 92 92 A5 2C 25 14 B9 93 03 11 B6 25 FE 9F BE 62 72 8F 4C 45 D0 0E 85 6F 93 EB 8B 
74 25 54 89 0A 49 12 92 A5 3C 25 54 F0 C0 6F F4 E7 43 88 08 7A F4 CF 4B BA AE 98 E5 7B D7 6B AF 
BD B6 EC 4B A8 29 D2 95 50 BC 0A 20 98 13 2A 29 59 CA F3 4C 7E 66 B5 95 59 56 B7 7C EF 42 96 86 
74 25 14 A7 02 88 29 A1 52 92 A5 72 D1 AF D5 5A 56 C7 1B 41 AA 1B 9E AE 02 EB 71 83 71 0F 2A BD 
9D EA C6 05 B5 1A 94 64 89 AF 84 A2 35 4D C9 9D F2 E6 A9 B8 88 29 18 6F A3 46 59 D0 0E A5 C5 47 
49 34 8D 7D 7C 6D 58 8F 1B D4 34 0D 00 7C DF 57 7A 3B D5 8D 3F 7D A1 41 2E 09 5F 8F 2F 9F 26 EE 
8E F8 08 8E 0C 11 44 15 E8 5D 08 A2 0A F4 AE 3A 53 5D 09 95 2A 62 4E A8 04 E8 5D 35 A6 BA 12 2A 
4F 18 55 58 09 C5 10 31 71 7C DE 1B B4 08 A7 0E 33 D0 33 73 42 F1 B6 1F 54 99 0D 2A 80 EB 5D DA 
5A B3 1E 37 28 DE CD 4C AA 2B A1 72 84 51 85 95 50 2C 11 53 A6 CF 33 97 12 D3 39 A1 F8 28 CB 06 
05 00 3C EF 4A 05 19 91 B5 44 30 77 72 82 CC 4C C9 B4 85 42 DB BA 33 73 83 E7 F9 3C E5 5F A6 D9 
1C 9D 78 E0 9D CD 0C D3 94 B0 1A 83 5C BC 57 34 63 79 CE C8 50 DD CB 01 44 16 A5 7F 37 D5 95 50 
D9 06 CB D8 A4 5E 1B 67 FA A7 E9 04 5A E8 44 B2 0C 3A 27 54 06 AA B2 41 05 E0 BC AB BE 54 57 42 
65 1B 84 A2 4A 28 60 88 98 72 FC B3 3D 74 CC 69 BF D3 27 5C 90 CA 09 95 85 AA 6C 50 01 E8 5D 35 
A6 BA 12 2A DB 20 14 53 42 31 45 4C B9 3E DF DE 4B FF E0 53 39 A1 72 51 92 0D 2A 60 A9 83 1A 44 
0E 4A FB B1 BA F1 02 59 6B 18 22 26 8E 1E 8A CC F6 63 19 90 91 A5 88 B7 79 9A BC 6C 50 F1 7D 81 
88 12 0A B9 58 68 A8 84 5A 01 34 54 42 21 35 65 B1 D9 A0 02 D6 F3 AF 48 DD C0 67 D7 2A 80 CF 2E 
84 44 BA 12 8A 2E A2 12 0A A9 27 D2 95 50 15 73 42 31 54 48 A2 0E BF 9A A0 77 D5 17 E9 4A A8 EA 
39 A1 D2 2A 24 31 87 5F 59 D0 BB EA 8B 74 25 14 9D 1C A6 90 12 8A 46 C8 E1 57 18 F4 AE FA 22 5D 
09 95 7B 0A 01 38 2A A4 8B 09 7A 57 7D 91 AE 84 E2 9D 02 C4 95 50 3C 15 12 45 18 60 17 D7 FC 2D 
03 F4 AE 1A 23 5D 09 C5 39 05 88 29 A1 68 B2 92 8D F7 26 91 0F AE 30 5C 4D 0A 72 71 50 DA 8F D5 
8D 8B 2A A1 18 2A A4 CC 0D F2 56 0C DA A1 D6 F3 BD 5E DD 58 F1 B7 C9 AA 0D AE 08 F4 DB E4 F5 BC 
CF BA 51 7D 9D 32 22 0B 1F B5 1A 6B 06 39 32 41 56 07 7C 76 21 88 2A F0 D9 85 20 AA 40 EF 42 10 
55 A0 77 21 88 2A D0 BB 10 44 15 E8 5D 08 A2 0A F4 AE F5 C4 1B B4 96 B9 2B 1C 02 00 E8 5D 6B C9 
60 B7 B5 33 82 D9 41 2B D6 B8 AE 5E CA F5 71 C1 5D 21 05 EA 8F 77 09 59 EF 4A 2C BB 44 EF 5A 43 
7A C3 FD 26 4C A1 B9 4F 6A 5C 57 3C E5 7A 75 BC B3 99 61 18 D3 53 17 40 74 9D B5 6A D0 BB D6 10 
6F 70 0C FB EE 3E 1C 27 7F DC AB 90 72 3D 7A B4 B5 0E 66 F1 91 16 F9 A4 1B 87 4F C9 70 0B 94 DC 
FA 89 93 37 BB 5D C3 3E 1E AF CC B2 4B F4 AE 35 44 EF 0D 87 6D BD 3D 1C 12 3F EE D5 48 B9 3E 3E 
EC DB 4D C7 F7 FD A3 6E 70 C0 1B EC F4 9B 8E EF BB 5D E8 EF 0C 3C 18 1F F7 A7 86 E5 FA FE 64 D8 
D6 05 EA CF 71 4F A7 E6 76 2F F4 A6 15 59 76 89 DE 55 1B 56 20 E5 BA 77 36 83 20 03 B5 BE D1 04 
80 20 35 80 DD D1 B4 46 DF 86 E9 A9 0B ED 3D CB 84 7E 23 74 CC FC FA 31 E3 63 1B EC 8E D6 B1 61 
3A 3A F1 54 AC B3 2E 01 7A 57 8D 58 7A CA 75 7D A3 09 F6 F1 18 02 3F 03 08 52 03 C4 1E 3A 6C 03 
E8 BD E1 C4 F7 5D CB 98 8E 4E 3C 81 FA 21 E3 63 3B 38 EE 5A C6 F4 D4 55 B1 CE BA 04 E8 5D 75 42 
EF 1D A5 FC AB BD 6D 82 B1 D9 28 62 62 12 0C 31 83 51 59 3C 32 D4 5A 03 0F F4 DE C4 B5 E0 A0 A1 
69 5A A3 D3 07 D3 DA DF 4A FE B2 DB 7B 96 39 EB 68 9A B6 33 8A CC ED 07 07 C2 89 54 38 CF 6A F4 
C1 EC 6E E9 F9 F5 43 BC B3 59 78 1B A1 43 AA 58 67 5D 1C D4 C8 23 88 2A F0 D9 85 50 2C 23 E5 FA 
5A 82 DE 85 50 84 19 61 62 86 F2 C6 56 E3 E0 1D 2F E7 85 B5 9A 0C 4F E2 3B 9D CB 06 47 86 C8 02 
F1 06 AD C6 E9 BE 4C 77 CD 65 BC AB 75 C0 71 87 ED F0 65 77 D7 9D F4 DC 5D ED 60 D3 9D 6C 9D A8 
BE 9A CA 99 70 10 44 94 28 A4 12 EC 60 17 6F 83 E7 F8 BE CF D8 C3 CE 31 01 0C C3 08 EA 58 AE 4F 
04 F9 8D 28 3E 18 56 30 9D D4 47 11 CC 9D F1 E2 6D F1 C2 5D F4 94 81 23 43 64 71 F4 F6 4D 00 D3 
F1 87 ED F1 6E C7 6E 3A 6E A0 AB EA 64 8C 08 BB 47 BE EF 47 DA AB 70 C8 EA 5A 61 9C 1E 00 A6 CD 
7D DF F7 87 6D C6 47 11 C5 76 3A 97 09 7A 17 B2 04 C2 D7 C4 3A 80 DE DE 36 93 EF 85 13 04 9E D1 
DE 36 83 D7 57 E3 C1 6E AB D5 6A 10 6F C4 A3 77 75 8C 8F 22 8A EE 74 2E 0F F4 2E 64 09 84 6F A5 
3C 00 6F 7C 6C 67 BC 71 9B CB 8C 9B 1B FA F8 B0 D3 87 EE D1 91 43 8B 22 79 1F 95 DB E9 5C 16 E8 
5D C8 32 68 0F 1D 73 D6 69 68 5A A3 33 33 9D 8C 7C D5 A3 C3 96 A6 75 66 A6 B5 D7 86 F6 B6 69 D8 
FD C6 CE F1 8C 7A 20 71 3F 22 5E 70 6B 8D 51 A8 90 5C D4 BB 66 8C 19 22 AB 4A 10 ED 5B 68 80 51 
32 F8 EC 42 10 55 E0 B3 0B 41 54 81 CF 2E 04 51 05 7A 17 82 A8 02 BD 0B 41 54 81 DE 85 20 AA 40 
EF 42 10 55 A0 77 21 88 2A D0 BB 10 44 15 E8 5D 08 A2 0A F4 2E 04 51 05 7A 17 82 A8 02 BD 0B 41 
54 81 DE 85 20 AA 40 EF 42 10 55 FC 3F 29 C1 25 51 E8 BE 90 05 00 00 00 00 49 45 4E 44 AE 42 60 
82 
EndData
$EndBitmap
Text Notes 5425 2650 0    50   Italic 10
0x17x
Text Notes 5450 2550 0    50   Italic 10
0xFx\n
Text Notes 5425 2875 0    50   Italic 10
0x27x
Text Notes 5350 3050 0    50   Italic 10
0x37x
Text Label 7275 3025 2    50   Italic 10
~IOW
Wire Wire Line
	5425 2650 5700 2650
Wire Wire Line
	5700 2650 5700 2525
Wire Wire Line
	5700 2525 6750 2525
Text Label 6750 2325 2    50   Italic 10
~IOR
Wire Wire Line
	6750 2525 6750 2825
Wire Wire Line
	6750 2825 7275 2825
Connection ~ 6750 2525
Text Label 9175 11275 2    50   Italic 10
volIRQ
Text Label 7875 2925 0    50   Italic 10
~volWr
Text Label 10025 11075 0    50   Italic 10
volID
Text Notes 7650 9675 0    50   Italic 10
volRQ0 poem volID=0
$Comp
L 74xx:74LS74 U24
U 1 1 61E1AC04
P 6950 9775
F 0 "U24" H 6950 10256 50  0000 C CNN
F 1 "74LS74" H 6950 10165 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6950 9775 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 6950 9775 50  0001 C CNN
	1    6950 9775
	1    0    0    1   
$EndComp
$Comp
L 74xx:74LS86 U500
U 1 1 62365B36
P 6350 8825
F 0 "U500" H 6350 9150 50  0000 C CNN
F 1 "74LS86" H 6350 9059 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6350 8825 50  0001 C CNN
F 3 "74xx/74ls86.pdf" H 6350 8825 50  0001 C CNN
	1    6350 8825
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS86 U500
U 2 1 6236A3E0
P 6350 9325
F 0 "U500" H 6350 9650 50  0000 C CNN
F 1 "74LS86" H 6350 9559 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6350 9325 50  0001 C CNN
F 3 "74xx/74ls86.pdf" H 6350 9325 50  0001 C CNN
	2    6350 9325
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 9225 6050 9225
Wire Wire Line
	5975 9425 6050 9425
$Comp
L 74xx:74LS86 U500
U 4 1 6239EA9B
P 7400 9225
F 0 "U500" H 7400 9550 50  0000 C CNN
F 1 "74LS86" H 7400 9459 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 7400 9225 50  0001 C CNN
F 3 "74xx/74ls86.pdf" H 7400 9225 50  0001 C CNN
	4    7400 9225
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS86 U500
U 3 1 623C3068
P 7425 8725
F 0 "U500" H 7425 9050 50  0000 C CNN
F 1 "74LS86" H 7425 8959 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 7425 8725 50  0001 C CNN
F 3 "74xx/74ls86.pdf" H 7425 8725 50  0001 C CNN
	3    7425 8725
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 8825 7125 8825
Connection ~ 6950 8825
$Comp
L Device:R R29
U 1 1 6241A262
P 7125 8475
F 0 "R29" V 6918 8475 50  0000 C CNN
F 1 "R" V 7009 8475 50  0000 C CNN
F 2 "GERAL:standard_2pins" V 7055 8475 50  0001 C CNN
F 3 "~" H 7125 8475 50  0001 C CNN
	1    7125 8475
	1    0    0    -1  
$EndComp
Text Label 7125 8325 1    50   ~ 0
5V
Wire Wire Line
	7725 8725 8150 8725
Wire Wire Line
	8150 8725 8150 9450
Wire Wire Line
	6950 9450 6950 9475
Wire Wire Line
	6950 9450 8150 9450
Text Label 8150 8725 0    50   Italic 10
~volIRQ0
Text Notes 7650 9800 0    50   Italic 10
volRQ1 poem volID=1
Wire Wire Line
	6650 9325 6650 9775
Connection ~ 6650 9325
Wire Wire Line
	6650 10075 6950 10075
Text Notes 5450 2450 0    50   Italic 10
0x7x
Text Label 1200 7675 0    315  Italic 63
POTENTIOMETERS
$Comp
L Connector_Generic:Conn_02x06_Counter_Clockwise J1
U 1 1 636F4338
P 17675 3650
F 0 "J1" H 17725 4067 50  0000 C CNN
F 1 "Conn_02x06_Counter_Clockwise" H 17725 3976 50  0000 C CNN
F 2 "GERAL:Conector2x6_Oval_anticlock" H 17675 3650 50  0001 C CNN
F 3 "~" H 17675 3650 50  0001 C CNN
	1    17675 3650
	1    0    0    -1  
$EndComp
Text Notes 10300 8000 0    50   Italic 10
Latch Enable (Load)\nquando descodificado o endereço
Text Label 9425 6850 0    315  Italic 63
LEDS
$Comp
L 74xx:74LS00 U5
U 5 1 640D0615
P 4375 6375
F 0 "U5" H 4375 6700 50  0000 C CNN
F 1 "74LS00" H 4375 6609 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 4375 6375 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls00" H 4375 6375 50  0001 C CNN
	5    4375 6375
	1    0    0    -1  
$EndComp
Text Label 4375 5875 0    50   ~ 0
5V
$Comp
L 74xx:74LS32 U18
U 5 1 61EFA2D9
P 4900 6400
F 0 "U18" H 4900 6125 50  0000 C CNN
F 1 "74LS32" H 4900 6200 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 4900 6400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 4900 6400 50  0001 C CNN
	5    4900 6400
	1    0    0    -1  
$EndComp
Text Label 4375 6875 0    50   ~ 0
GND
Text Label 4900 6900 0    50   ~ 0
GND
$Comp
L 74xx:74LS00 U5
U 3 1 6414DCE9
P 8575 12025
F 0 "U5" H 8575 12350 50  0000 C CNN
F 1 "74LS00" H 8575 12259 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 8575 12025 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls00" H 8575 12025 50  0001 C CNN
	3    8575 12025
	1    0    0    -1  
$EndComp
Wire Wire Line
	8275 11925 8275 12125
$Comp
L 74xx:74LS86 U500
U 5 1 64191834
P 6700 7650
F 0 "U500" H 6700 7975 50  0000 C CNN
F 1 "74LS86" H 6700 7884 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6700 7650 50  0001 C CNN
F 3 "74xx/74ls86.pdf" H 6700 7650 50  0001 C CNN
	5    6700 7650
	1    0    0    -1  
$EndComp
Text Label 6700 7150 0    50   ~ 0
5V
Text Label 6700 8150 0    50   ~ 0
GND
$Comp
L 74xx:74LS74 U24
U 3 1 641B39FD
P 9775 9100
F 0 "U24" H 10125 9600 50  0000 C CNN
F 1 "74LS74" H 10150 9525 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 9775 9100 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 9775 9100 50  0001 C CNN
	3    9775 9100
	-1   0    0    -1  
$EndComp
Text Label 9775 8700 0    50   ~ 0
5V
Text Label 9775 9500 0    50   ~ 0
GND
$Comp
L 74xx:74LS32 U20
U 5 1 646FD92D
P 8100 1475
F 0 "U20" H 8100 1200 50  0000 C CNN
F 1 "74LS32" H 8100 1275 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 8100 1475 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 8100 1475 50  0001 C CNN
	5    8100 1475
	1    0    0    -1  
$EndComp
Text Label 8100 975  0    50   ~ 0
5V
Text Label 8100 1975 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x03_Male J2
U 1 1 61F76AC9
P 925 9100
F 0 "J2" H 1033 9381 50  0000 C CNN
F 1 "Conn_01x03_Male" H 1033 9290 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 925 9100 50  0001 C CNN
F 3 "~" H 925 9100 50  0001 C CNN
	1    925  9100
	1    0    0    -1  
$EndComp
Text Label 1125 9100 0    50   Italic 10
toPotResistor0
Text Label 1125 9000 0    50   Italic 10
5V
Text Label 1425 9200 0    50   Italic 10
GND
$Comp
L Connector:Conn_01x03_Male J3
U 1 1 61FAD731
P 900 9700
F 0 "J3" H 1008 9981 50  0000 C CNN
F 1 "Conn_01x03_Male" H 1008 9890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 900 9700 50  0001 C CNN
F 3 "~" H 900 9700 50  0001 C CNN
	1    900  9700
	1    0    0    -1  
$EndComp
Text Label 1100 9700 0    50   Italic 10
toPotResistor1
Text Label 1100 9600 0    50   Italic 10
5V
Text Label 1400 9800 0    50   Italic 10
GND
Text Label 7100 3600 0    50   Italic 10
~dispCS
$Comp
L 74xx:74LS32 U18
U 3 1 61BB8BB4
P 6800 3600
F 0 "U18" H 6800 3925 50  0000 C CNN
F 1 "74LS32" H 6800 3834 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 6800 3600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 6800 3600 50  0001 C CNN
	3    6800 3600
	1    0    0    -1  
$EndComp
Text Notes 6925 3450 0    50   Italic 10
0x2F0:3
Wire Wire Line
	6500 3500 6500 3550
Wire Wire Line
	6500 3550 6350 3550
Wire Wire Line
	6350 3550 6350 4050
Wire Wire Line
	6350 4050 6525 4050
Connection ~ 6500 3500
Text Notes 4800 9750 0    50   Italic 10
ATENÇÃO ESQUECI-ME!\nencima e embaixo\nfalta um divisor de tensão depois \nde cada opamp inversor 10K/7K!!
$Comp
L Device:R R33
U 1 1 62085AAD
P 1275 9200
F 0 "R33" V 1482 9200 50  0000 C CNN
F 1 "270" V 1391 9200 50  0000 C CNN
F 2 "" V 1205 9200 50  0001 C CNN
F 3 "~" H 1275 9200 50  0001 C CNN
	1    1275 9200
	0    1    1    0   
$EndComp
$Comp
L Device:R R32
U 1 1 620A0B50
P 1250 9800
F 0 "R32" V 1457 9800 50  0000 C CNN
F 1 "270" V 1366 9800 50  0000 C CNN
F 2 "" V 1180 9800 50  0001 C CNN
F 3 "~" H 1250 9800 50  0001 C CNN
	1    1250 9800
	0    1    1    0   
$EndComp
$Comp
L Device:C C5
U 1 1 620C3A74
P 3675 9175
F 0 "C5" V 3423 9175 50  0000 C CNN
F 1 "470pF" V 3514 9175 50  0000 C CNN
F 2 "" H 3713 9025 50  0001 C CNN
F 3 "~" H 3675 9175 50  0001 C CNN
	1    3675 9175
	0    1    1    0   
$EndComp
Wire Wire Line
	3925 9175 3825 9175
Connection ~ 3925 9175
Wire Wire Line
	3925 9175 3925 8825
Wire Wire Line
	3525 9175 3325 9175
Wire Wire Line
	3325 9175 3325 9550
Connection ~ 3325 9550
$Comp
L 74xx:74LS74 U24
U 2 1 61FF8301
P 12025 11050
F 0 "U24" H 12025 11531 50  0000 C CNN
F 1 "74LS74" H 12025 11440 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 12025 11050 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 12025 11050 50  0001 C CNN
	2    12025 11050
	1    0    0    1   
$EndComp
Text Notes 11200 10625 0    50   Italic 10
EOC é ativo a LOW, tem de se inverter!
Text Label 11825 11375 2    50   Italic 10
~volEOC
Wire Wire Line
	11825 11375 12025 11375
Wire Wire Line
	12025 11375 12025 11350
Text Label 12325 11150 0    50   Italic 10
volIRQPIN
Text Label 12025 10750 0    50   Italic 10
GND
Wire Wire Line
	10025 11275 10025 11175
$Comp
L 74xx:74LS74 U?
U 1 1 62CC4BA8
P 8450 4950
F 0 "U?" H 8450 5431 50  0000 C CNN
F 1 "74LS74" H 8450 5340 50  0000 C CNN
F 2 "" H 8450 4950 50  0001 C CNN
F 3 "74xx/74hc_hct74.pdf" H 8450 4950 50  0001 C CNN
	1    8450 4950
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS32 U20
U 2 1 6209A572
P 7575 2925
F 0 "U20" H 7575 2650 50  0000 C CNN
F 1 "74LS32" H 7575 2725 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 7575 2925 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 7575 2925 50  0001 C CNN
	2    7575 2925
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS32 U20
U 1 1 61FBDBAA
P 7050 2425
F 0 "U20" H 7050 2150 50  0000 C CNN
F 1 "74LS32" H 7050 2225 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 7050 2425 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS32" H 7050 2425 50  0001 C CNN
	1    7050 2425
	1    0    0    -1  
$EndComp
Text Notes 5425 3150 0    50   Italic 10
0x3Fx
$Comp
L 74xx:74LS00 U5
U 4 1 63DBCCA8
P 7550 4250
F 0 "U5" H 7550 4575 50  0000 C CNN
F 1 "74LS00" H 7550 4484 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 7550 4250 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls00" H 7550 4250 50  0001 C CNN
	4    7550 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7125 4150 7250 4150
Wire Wire Line
	7250 4350 7250 4150
Connection ~ 7250 4150
Wire Wire Line
	5425 3150 5425 3350
Wire Wire Line
	5425 3350 5075 3350
$Comp
L 74xx:74LS00 U5
U 1 1 69DB17CB
P 4125 3050
F 0 "U5" H 4125 3375 50  0000 C CNN
F 1 "74LS00" H 4125 3284 50  0000 C CNN
F 2 "GERAL:standard_DIP_14OvalPins" H 4125 3050 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls00" H 4125 3050 50  0001 C CNN
	1    4125 3050
	1    0    0    -1  
$EndComp
Text Label 4425 3150 2    50   Italic 10
AEN
Text Label 8150 4850 2    50   Italic 10
D0
Wire Wire Line
	8150 4950 7850 4950
Wire Wire Line
	8450 4650 8500 4650
Wire Wire Line
	8500 4650 8500 5250
Wire Wire Line
	8500 5250 8475 5250
Text Label 8750 5050 0    50   Italic 10
digitsON
Text Label 14850 4500 0    50   Italic 10
digitsON
Wire Wire Line
	14825 4750 14825 4500
Wire Wire Line
	14825 4500 14850 4500
Wire Wire Line
	8475 5250 8475 5375
Connection ~ 8475 5250
Wire Wire Line
	8475 5250 8450 5250
Text Label 7525 4700 2    50   Italic 10
D3
$EndSCHEMATC
