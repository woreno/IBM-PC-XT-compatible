EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
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
$Comp
L GERAL:Intel82077AA U1
U 1 1 62B2755A
P 6775 6950
F 0 "U1" H 7350 5225 50  0000 L CNN
F 1 "Intel82077AA" H 7150 5150 50  0000 L CNN
F 2 "Package_LCC:PLCC-68_SMD-Socket" H 6825 7125 50  0001 C CNN
F 3 "" H 6825 7125 50  0001 C CNN
	1    6775 6950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10125 6475 10125 6575
Wire Wire Line
	10125 6375 10125 6275
$Comp
L Device:C C1
U 1 1 62B29E47
P 10750 6575
F 0 "C1" H 10525 6650 50  0000 L CNN
F 1 "22pF" H 10525 6575 50  0000 L CNN
F 2 "GERAL:standard_2pins" H 10788 6425 50  0001 C CNN
F 3 "~" H 10750 6575 50  0001 C CNN
	1    10750 6575
	0    -1   -1   0   
$EndComp
$Comp
L Device:Crystal Y1
U 1 1 62B2A768
P 10475 6425
F 0 "Y1" H 10475 6693 50  0000 C CNN
F 1 "X20M" H 10475 6602 50  0000 C CNN
F 2 "GERAL:standard_2pins_half" H 10475 6425 50  0001 C CNN
F 3 "~" H 10475 6425 50  0001 C CNN
	1    10475 6425
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10125 6575 10475 6575
Wire Wire Line
	10125 6275 10475 6275
$Comp
L Device:C C2
U 1 1 62B2C91D
P 10750 6275
F 0 "C2" H 10525 6350 50  0000 L CNN
F 1 "22pF" H 10425 6425 50  0000 L CNN
F 2 "GERAL:standard_2pins_half" H 10788 6125 50  0001 C CNN
F 3 "~" H 10750 6275 50  0001 C CNN
	1    10750 6275
	0    1    1    0   
$EndComp
Wire Wire Line
	10475 6275 10600 6275
Connection ~ 10475 6275
Wire Wire Line
	10475 6575 10600 6575
Connection ~ 10475 6575
Wire Wire Line
	10900 6575 10900 6425
Wire Wire Line
	10900 6425 11000 6425
Connection ~ 10900 6425
Wire Wire Line
	10900 6425 10900 6275
$Comp
L power:Earth #PWR0101
U 1 1 62B2CFDB
P 11000 6425
F 0 "#PWR0101" H 11000 6175 50  0001 C CNN
F 1 "Earth" H 11000 6275 50  0001 C CNN
F 2 "" H 11000 6425 50  0001 C CNN
F 3 "~" H 11000 6425 50  0001 C CNN
	1    11000 6425
	0    -1   -1   0   
$EndComp
Text Label 7925 7475 3    50   Italic 10
A2
Text Notes 9175 3125 0    50   Italic 10
0 0 0 R Status Register A SRA\n0 0 1 R Status Register B SRB\n0 1 0 R/W Digital Output Register DOR\n0 1 1 R/W Tape Drive Register TDR\n1 0 0 R Main Status Register MSR\n1 0 0 W Data Rate Select Register DSR\n1 0 1 R/W Data (FIFO) FIFO\n1 1 0 Reserved\n1 1 1 R Digital Input Register DIR\n1 1 1 W Configuration Control Register CCR
Text Notes 9200 2275 0    20   Italic 4
A2   A1   A0
Text Notes 8875 2350 0    50   Italic 10
0x3Fx
Text Label 7625 7000 2    50   Italic 10
A1
Text Label 7625 6900 2    50   Italic 10
A0
Text Label 7625 7100 2    50   Italic 10
GND
Text Label 8125 7475 3    50   Italic 10
GND
Text Label 8025 5150 1    50   Italic 10
GND
Text Label 7925 5150 1    50   Italic 10
5V
Text Label 7625 6800 2    50   Italic 10
~FDC_CS
Text Label 7625 6700 2    50   Italic 10
~IOW
Text Label 7625 6600 2    50   Italic 10
~IOR
Text Label 7625 6500 2    50   Italic 10
~FDC_DACK
Text Label 1850 3975 2    50   Italic 10
~FDC_DACK
Text Label 1825 1975 2    50   Italic 10
FDC_DRQ
Text Label 9325 7475 3    50   Italic 10
FDC_DRQ
Text Label 8025 7475 3    50   Italic 10
FDC_D0
Text Label 8225 7475 3    50   Italic 10
FDC_D1
Text Label 8325 7475 3    50   Italic 10
FDC_D2
Text Label 8425 7475 3    50   Italic 10
FDC_D3
Text Label 8525 7475 3    50   Italic 10
GND
Text Label 8625 7475 3    50   Italic 10
FDC_D4
Text Label 8725 7475 3    50   Italic 10
5V
Text Label 8825 7475 3    50   Italic 10
FDC_D5
Text Label 8925 7475 3    50   Italic 10
FDC_D6
Text Label 9025 7475 3    50   Italic 10
GND
Text Label 9125 7475 3    50   Italic 10
FDC_D7
Text Label 9225 7475 3    50   Italic 10
FDC_IRQ
Text Label 9425 7475 3    50   Italic 10
TC
Text Label 9775 5775 0    50   Italic 10
5V
Text Label 9775 6175 0    50   Italic 10
GND
Wire Wire Line
	9775 6475 10125 6475
Wire Wire Line
	9775 6375 10125 6375
$Comp
L GERAL:floppy_interface U2
U 1 1 62B39F2C
P 14400 2850
F 0 "U2" H 14362 2925 50  0000 C CNN
F 1 "floppy_interface" H 14362 2834 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x17_P2.54mm_Vertical" H 14400 2825 50  0001 C CNN
F 3 "" H 14400 2825 50  0001 C CNN
	1    14400 2850
	1    0    0    -1  
$EndComp
Text Label 9775 7075 0    50   Italic 10
PULLUP
Text Notes 10350 7130 2    30   Italic 6
(modo AT)
Text Label 9775 6275 0    50   Italic 10
GND
Text Notes 8925 5625 3    50   Italic 10
estes sinais são\nativos a LOW,\nse INVERT a LOW
Text Label 13925 3375 2    50   Italic 10
~A_MOTOR_ON
Text Label 8225 5150 1    50   Italic 10
~A_MOTOR_ON
Text Label 13925 3675 2    50   Italic 10
~B_MOTOR_ON
Text Label 7625 5500 2    50   Italic 10
~B_MOTOR_ON
Text Label 13925 3575 2    50   Italic 10
~A_SELECT
Text Label 13925 3475 2    50   Italic 10
~B_SELECT
Text Label 8125 5150 1    50   Italic 10
~A_SELECT
Text Label 7625 5600 2    50   Italic 10
~B_SELECT
Text Label 13925 4475 2    50   Italic 10
~SIDE1
Text Label 8825 5150 1    50   Italic 10
~SIDE1
Text Label 13925 3975 2    50   Italic 10
~WRITE_DATA
Text Label 13925 3775 2    50   Italic 10
~DIRECTION
Text Label 8325 5150 1    50   Italic 10
~DIRECTION
Text Label 8625 5150 1    50   Italic 10
~WRITE_DATA
Text Label 8525 5150 1    50   Italic 10
GND
Text Label 13925 3875 2    50   Italic 10
~STEP
Text Label 8425 5150 1    50   Italic 10
~STEP
Text Label 13925 3000 2    50   Italic 10
~DENSEL
Text Label 9025 5150 1    50   Italic 10
~DENSEL
Text Label 13925 3275 2    50   Italic 10
~INDEX
Text Label 13925 4575 2    50   Italic 10
~DISK_CHG
Text Label 9775 6675 0    50   Italic 10
~DISK_CHG
Text Label 13925 4175 2    50   Italic 10
~TRACK0
Text Label 7625 6400 2    50   Italic 10
~TRACK0
Text Label 13925 4275 2    50   Italic 10
~WR_PROTECT
Text Label 7625 6300 2    50   Italic 10
~WR_PROTECT
Text Label 9525 7475 3    50   Italic 10
~INDEX
Text Label 7625 6200 2    50   Italic 10
5V
NoConn ~ 7625 6100
NoConn ~ 7625 6000
NoConn ~ 7625 5800
NoConn ~ 7625 5700
Text Label 7625 5900 2    50   Italic 10
GND
Text Label 8925 5150 1    50   Italic 10
GND
Text Label 13925 4075 2    50   Italic 10
~WRITE_ENABLE
Text Label 8725 5150 1    50   Italic 10
~WRITE_ENABLE
Text Notes 9130 4400 3    30   Italic 6
(modo AT, tem pullup interno)
Text Label 10425 6075 0    50   Italic 10
GND
Wire Wire Line
	9775 6075 10425 6075
$Comp
L Device:C C3
U 1 1 62B44BFD
P 10425 5925
F 0 "C3" V 10200 6000 50  0000 L CNN
F 1 "0.0047 uF" V 10600 5950 50  0000 L CNN
F 2 "GERAL:standard_2pins" H 10463 5775 50  0001 C CNN
F 3 "~" H 10425 5925 50  0001 C CNN
	1    10425 5925
	1    0    0    -1  
$EndComp
Wire Wire Line
	10425 5775 10425 5750
NoConn ~ 9125 5150
NoConn ~ 9775 6975
NoConn ~ 9775 6875
Text Label 9775 5875 0    50   Italic 10
PULLUP
Wire Wire Line
	10275 5975 10275 5750
Wire Wire Line
	9775 5975 10275 5975
Text Notes 10425 5505 2    30   Italic 6
(optimizado para disquete\n em vez de cassete)
Wire Wire Line
	10275 5750 10425 5750
Text Label 9325 5150 1    50   Italic 10
5V
Text Label 9425 5150 1    50   Italic 10
GND
Text Label 13925 4375 2    50   Italic 10
~RD_DATA
Text Label 9775 5675 0    50   Italic 10
~RD_DATA
$Comp
L Connector:Bus_ISA_8bit J1
U 1 1 650A7D45
P 3175 2975
F 0 "J1" H 3175 4742 50  0000 C CNN
F 1 "Bus_ISA_8bit" H 3175 4651 50  0000 C CNN
F 2 "GERAL:ISA_8bit_edge" H 3175 2975 50  0001 C CNN
F 3 "https://en.wikipedia.org/wiki/Industry_Standard_Architecture" H 3175 2975 50  0001 C CNN
	1    3175 2975
	1    0    0    -1  
$EndComp
Text Notes 6400 2600 0    50   ~ 0
3F4 estado\n3F5 dados\n3F2 digital output\n\n[11|1111|0XXX]\n\n
Text Label 7425 2900 2    50   Italic 10
A9
$Comp
L Logic_Programmable:PAL16L8 U4
U 1 1 650B95BF
P 7925 2800
F 0 "U4" H 8025 3525 50  0000 C CNN
F 1 "PAL16L8" H 8125 3450 50  0000 C CNN
F 2 "GERAL:standard_DIP_20pins_thin_body" H 7925 2800 50  0001 C CNN
F 3 "" H 7925 2800 50  0001 C CNN
	1    7925 2800
	1    0    0    -1  
$EndComp
Text Label 7425 2800 2    50   Italic 10
A8
Text Label 7425 2700 2    50   Italic 10
A7
Text Label 7425 2600 2    50   Italic 10
A6
Text Label 7425 2400 2    50   Italic 10
A4
Text Label 7425 2500 2    50   Italic 10
A5
Text Label 7425 2300 2    50   Italic 10
A3
Text Label 8425 3000 0    50   Italic 10
~FDC_CS
Text Label 7425 3200 2    50   Italic 10
AEN
Text Label 7425 3100 2    50   Italic 10
~IOW
Text Label 7425 3000 2    50   Italic 10
~IOR
Text Label 3875 1475 0    50   Italic 10
BB1
Text Label 3875 2375 0    50   Italic 10
BB10
Text Label 3875 2575 0    50   Italic 10
BB12
Text Label 3875 2675 0    50   Italic 10
BB13
Text Label 3875 2775 0    50   Italic 10
BB14
Text Label 3875 2875 0    50   Italic 10
BB15
Text Label 3875 2975 0    50   Italic 10
BB16
Text Label 3875 3075 0    50   Italic 10
BB17
Text Label 3875 3175 0    50   Italic 10
BB18
Text Label 3875 3275 0    50   Italic 10
BB19
Text Label 3875 3375 0    50   Italic 10
BB20
Text Label 3875 3475 0    50   Italic 10
BB21
Text Label 2475 1775 2    50   Italic 10
BA4
Text Label 2475 1875 2    50   Italic 10
BA5
Text Label 2475 2075 2    50   Italic 10
BA7
Text Label 2475 2175 2    50   Italic 10
BA8
Text Label 2475 2275 2    50   Italic 10
BA9
Text Label 2475 2375 2    50   Italic 10
BA10
Text Label 2475 2875 2    50   Italic 10
BA15
Text Label 2475 2975 2    50   Italic 10
BA16
Text Label 2475 3075 2    50   Italic 10
BA17
Text Label 2475 3175 2    50   Italic 10
BA18
Text Label 2475 3275 2    50   Italic 10
BA19
Text Label 2475 3375 2    50   Italic 10
BA20
Text Label 2475 3475 2    50   Italic 10
BA21
Text Label 2475 3675 2    50   Italic 10
BA23
Text Label 2475 3775 2    50   Italic 10
BA24
Text Label 2475 3875 2    50   Italic 10
BA25
Text Label 2475 4175 2    50   Italic 10
BA28
Text Label 2475 4375 2    50   Italic 10
BA30
Text Notes 13450 3250 0    50   Italic 10
<--
Text Notes 13375 2975 0    50   Italic 10
-->
Text Notes 13175 3375 0    50   Italic 10
-->
Text Notes 13200 3475 0    50   Italic 10
-->
Text Notes 13225 3575 0    50   Italic 10
-->
Text Notes 13175 3650 0    50   Italic 10
-->
Text Notes 13250 3750 0    50   Italic 10
-->
Text Notes 13450 3850 0    50   Italic 10
-->
Text Notes 13225 3950 0    50   Italic 10
-->
Text Notes 13125 4050 0    50   Italic 10
-->
Text Notes 13350 4150 0    50   Italic 10
<--
Text Notes 13200 4250 0    50   Italic 10
<--
Text Notes 13350 4375 0    50   Italic 10
<--
Text Notes 13275 4550 0    50   Italic 10
<--
Text Notes 13450 4475 0    50   Italic 10
-->
$Comp
L power:Earth #PWR0103
U 1 1 62E4D6DE
P 1075 1100
F 0 "#PWR0103" H 1075 850 50  0001 C CNN
F 1 "Earth" H 1075 950 50  0001 C CNN
F 2 "" H 1075 1100 50  0001 C CNN
F 3 "~" H 1075 1100 50  0001 C CNN
	1    1075 1100
	1    0    0    -1  
$EndComp
Text Label 1075 1100 0    50   Italic 10
GND
Text Notes 7075 8325 1    300  Italic 60
Controlador
Text Notes 13150 1825 0    300  Italic 60
Interface
Text Notes 6400 1825 0    300  Italic 60
Validar I/O
Text Label 2200 3975 2    50   Italic 10
~DACK2
Text Label 2250 1975 2    50   Italic 10
DRQ2
Wire Wire Line
	1850 3975 2475 3975
Wire Wire Line
	1850 3575 2475 3575
Wire Wire Line
	1825 1975 2475 1975
Text Label 2050 2675 2    50   Italic 10
~IOW
Text Label 2050 2775 2    50   Italic 10
~IOR
Wire Wire Line
	2475 2775 2050 2775
Wire Wire Line
	2475 2675 2050 2675
Text Label 4150 1575 0    50   Italic 10
D7
Text Label 4150 1675 0    50   Italic 10
D6
Text Label 4150 1775 0    50   Italic 10
D5
Text Label 4150 1875 0    50   Italic 10
D4
Text Label 4150 1975 0    50   Italic 10
D3
Text Label 4150 2075 0    50   Italic 10
D2
Text Label 4150 2175 0    50   Italic 10
D1
Text Label 4150 2275 0    50   Italic 10
D0
Wire Wire Line
	4150 1575 3875 1575
Wire Wire Line
	3875 1675 4150 1675
Wire Wire Line
	4150 1775 3875 1775
Wire Wire Line
	3875 1875 4150 1875
Wire Wire Line
	3875 2075 4150 2075
Wire Wire Line
	3875 1975 4150 1975
Wire Wire Line
	3875 2175 4150 2175
Wire Wire Line
	4150 2275 3875 2275
Text Label 4150 2475 0    50   Italic 10
AEN
Wire Wire Line
	4150 2475 3875 2475
Text Label 4150 4475 0    50   Italic 10
A0
Text Label 4150 4375 0    50   Italic 10
A1
Text Label 4150 4275 0    50   Italic 10
A2
Wire Wire Line
	4150 4275 3875 4275
Wire Wire Line
	3875 4375 4150 4375
Wire Wire Line
	4150 4475 3875 4475
Text Label 4150 4175 0    50   Italic 10
A3
Text Label 4150 4075 0    50   Italic 10
A4
Text Label 4150 3975 0    50   Italic 10
A5
Wire Wire Line
	4150 3975 3875 3975
Wire Wire Line
	3875 4075 4150 4075
Wire Wire Line
	4150 4175 3875 4175
Text Label 4150 3875 0    50   Italic 10
A6
Text Label 4150 3775 0    50   Italic 10
A7
Text Label 4150 3675 0    50   Italic 10
A8
Wire Wire Line
	4150 3675 3875 3675
Wire Wire Line
	3875 3775 4150 3775
Wire Wire Line
	4150 3875 3875 3875
Text Label 4150 3575 0    50   Italic 10
A9
Wire Wire Line
	4150 3575 3875 3575
Text Notes 4275 200  0    100  Italic 20
O CPU faz comando de leitura\no FDC faz ~DREQ~ e CPU ~DACK~\no controlador DMA faz ~IOR~ ao FDC;  logo ~MEMW~ à memoria\nquando o controlador DMA faz TC, e o FDC gera INTA \n\nA gate de dados tem de permanecer sempre ativa porcausa de DMA\nO chip select do FDC no entanto pode evitar o DMA com endereço\nnao ententido para ele
Text Label 2225 4075 2    50   Italic 10
TC
Wire Wire Line
	2225 4075 2475 4075
Text Label 2225 4275 2    50   Italic 10
5V
Wire Wire Line
	2475 4275 2225 4275
Text Label 2225 4475 2    50   Italic 10
GND
Wire Wire Line
	2475 4475 2225 4475
Text Label 2250 1475 2    50   Italic 10
GND
Text Label 2250 1675 2    50   Italic 10
5V
Wire Wire Line
	2250 1675 2475 1675
Wire Wire Line
	2475 1475 2250 1475
Text Notes 6825 2450 0    16   Italic 3
A3
Text Notes 6500 2450 0    16   Italic 3
A8
Text Label 5925 2475 0    50   Italic 10
FDC_D0
Text Label 5925 2575 0    50   Italic 10
FDC_D1
Text Label 5925 2675 0    50   Italic 10
FDC_D2
Text Label 5925 2775 0    50   Italic 10
FDC_D3
Text Label 5925 2875 0    50   Italic 10
FDC_D4
Text Label 5925 2975 0    50   Italic 10
FDC_D5
Text Label 5925 3075 0    50   Italic 10
FDC_D6
Text Label 5925 3175 0    50   Italic 10
FDC_D7
Text Label 4925 2475 2    50   Italic 10
D0
Text Label 4925 2575 2    50   Italic 10
D1
Text Label 4925 2675 2    50   Italic 10
D2
Text Label 4925 2775 2    50   Italic 10
D3
Text Label 4925 2875 2    50   Italic 10
D4
Text Label 4925 2975 2    50   Italic 10
D5
Text Label 4925 3075 2    50   Italic 10
D6
Text Label 4925 3175 2    50   Italic 10
D7
Text Label 8425 2900 0    50   Italic 10
DATA_DIR
$Comp
L power:Earth #PWR0105
U 1 1 62EEDC25
P 7925 3400
F 0 "#PWR0105" H 7925 3150 50  0001 C CNN
F 1 "Earth" H 7925 3250 50  0001 C CNN
F 2 "" H 7925 3400 50  0001 C CNN
F 3 "~" H 7925 3400 50  0001 C CNN
	1    7925 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 630362DF
P 1650 925
F 0 "C4" H 1765 971 50  0000 L CNN
F 1 "C" H 1765 880 50  0000 L CNN
F 2 "GERAL:standard_2pins" H 1688 775 50  0001 C CNN
F 3 "~" H 1650 925 50  0001 C CNN
	1    1650 925 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 63036D47
P 2075 925
F 0 "C5" H 2190 971 50  0000 L CNN
F 1 "C" H 2190 880 50  0000 L CNN
F 2 "GERAL:standard_2pins" H 2113 775 50  0001 C CNN
F 3 "~" H 2075 925 50  0001 C CNN
	1    2075 925 
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0107
U 1 1 63039E5E
P 1650 1075
F 0 "#PWR0107" H 1650 825 50  0001 C CNN
F 1 "Earth" H 1650 925 50  0001 C CNN
F 2 "" H 1650 1075 50  0001 C CNN
F 3 "~" H 1650 1075 50  0001 C CNN
	1    1650 1075
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0108
U 1 1 6303AF75
P 2075 1075
F 0 "#PWR0108" H 2075 825 50  0001 C CNN
F 1 "Earth" H 2075 925 50  0001 C CNN
F 2 "" H 2075 1075 50  0001 C CNN
F 3 "~" H 2075 1075 50  0001 C CNN
	1    2075 1075
	1    0    0    -1  
$EndComp
Text Label 1650 775  0    50   Italic 10
5V
Text Label 2075 775  0    50   Italic 10
5V
$Comp
L Device:C C7
U 1 1 6303F061
P 2850 925
F 0 "C7" H 2965 971 50  0000 L CNN
F 1 "C" H 2965 880 50  0000 L CNN
F 2 "GERAL:standard_2pins_half" H 2888 775 50  0001 C CNN
F 3 "~" H 2850 925 50  0001 C CNN
	1    2850 925 
	1    0    0    -1  
$EndComp
$Comp
L power:Earth #PWR0110
U 1 1 6303F067
P 2850 1075
F 0 "#PWR0110" H 2850 825 50  0001 C CNN
F 1 "Earth" H 2850 925 50  0001 C CNN
F 2 "" H 2850 1075 50  0001 C CNN
F 3 "~" H 2850 1075 50  0001 C CNN
	1    2850 1075
	1    0    0    -1  
$EndComp
Text Label 2850 775  0    50   Italic 10
5V
Text Label 7925 2100 0    50   Italic 10
5V
Text Label 9975 7900 0    50   Italic 10
PULLUP
$Comp
L Device:R R1
U 1 1 6235D439
P 9825 7900
F 0 "R1" H 9895 7946 50  0000 L CNN
F 1 "R" H 9895 7855 50  0000 L CNN
F 2 "GERAL:standard_2pins" V 9755 7900 50  0001 C CNN
F 3 "~" H 9825 7900 50  0001 C CNN
	1    9825 7900
	0    -1   -1   0   
$EndComp
Text Label 9675 7900 3    50   Italic 10
5V
Text Label 2250 1575 2    50   Italic 10
RESET
Wire Wire Line
	2475 1575 2250 1575
Text Label 9775 6575 0    50   Italic 10
RESET
$Comp
L Device:C C6
U 1 1 62C1B5B0
P 2500 925
F 0 "C6" H 2615 971 50  0000 L CNN
F 1 "C" H 2615 880 50  0000 L CNN
F 2 "GERAL:standard_2pins" H 2538 775 50  0001 C CNN
F 3 "~" H 2500 925 50  0001 C CNN
	1    2500 925 
	1    0    0    -1  
$EndComp
Text Label 2500 775  0    50   Italic 10
5V
$Comp
L power:Earth #PWR0102
U 1 1 62C1C61D
P 2500 1075
F 0 "#PWR0102" H 2500 825 50  0001 C CNN
F 1 "Earth" H 2500 925 50  0001 C CNN
F 2 "" H 2500 1075 50  0001 C CNN
F 3 "~" H 2500 1075 50  0001 C CNN
	1    2500 1075
	1    0    0    -1  
$EndComp
NoConn ~ 8425 2300
NoConn ~ 8425 2400
NoConn ~ 8425 2500
NoConn ~ 8425 2600
NoConn ~ 8425 2700
NoConn ~ 8425 2800
Text Notes 4600 4150 0    50   Italic 10
DATA_DIR:\nA->B se  /IOR=1  (escrita, D0 -> FDC_D0)\nA<-B se  /IOR=0  (leitura, D0 <- FDC_D0)\n
Text Label 9775 6775 0    50   Italic 10
GND
Wire Wire Line
	4925 2575 5925 2575
Wire Wire Line
	5925 2475 4925 2475
Wire Wire Line
	4925 2675 5925 2675
Wire Wire Line
	5925 2775 4925 2775
Wire Wire Line
	4925 2875 5925 2875
Wire Wire Line
	5925 2975 4925 2975
Wire Wire Line
	4925 3075 5925 3075
Wire Wire Line
	5925 3175 4925 3175
Text Label 2475 2575 2    50   Italic 10
BA12
Text Label 2475 2475 2    50   Italic 10
BA11
Text Label 14800 3050 0    50   Italic 10
GND
Wire Wire Line
	14800 3050 14800 3125
Wire Wire Line
	14800 3200 14800 3125
Connection ~ 14800 3125
Wire Wire Line
	14800 3200 14800 3275
Connection ~ 14800 3200
Wire Wire Line
	14800 3275 14800 3350
Connection ~ 14800 3275
Wire Wire Line
	14800 3350 14800 3450
Connection ~ 14800 3350
Wire Wire Line
	14800 3450 14800 3525
Connection ~ 14800 3450
Wire Wire Line
	14800 3600 14800 3525
Connection ~ 14800 3525
Wire Wire Line
	14800 3600 14800 3675
Connection ~ 14800 3600
Wire Wire Line
	14800 3675 14800 3775
Connection ~ 14800 3675
Wire Wire Line
	14800 3775 14800 3875
Connection ~ 14800 3775
Wire Wire Line
	14800 3875 14800 3950
Wire Wire Line
	14800 3950 14800 4025
Connection ~ 14800 3875
Connection ~ 14800 3950
Wire Wire Line
	14800 4025 14800 4100
Connection ~ 14800 4025
Wire Wire Line
	14800 4175 14800 4100
Connection ~ 14800 4100
Wire Wire Line
	14800 4175 14800 4250
Connection ~ 14800 4175
Wire Wire Line
	14800 4250 14800 4325
Connection ~ 14800 4250
Text Label 9775 6475 0    50   Italic 10
X1
Text Label 9775 6375 0    50   Italic 10
x2
Text Label 9775 6075 0    50   Italic 10
LOFIL
Text Label 9775 5975 0    50   Italic 10
hiFIL
Text Label 2200 3575 2    50   Italic 10
IRQ6
Text Label 1850 3575 2    50   Italic 10
FDC_IRQ
$EndSCHEMATC
