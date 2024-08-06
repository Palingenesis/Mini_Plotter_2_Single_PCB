EESchema Schematic File Version 4
LIBS:Mini Plotter 2.1-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Mini Plotter 2.1"
Date "2019-02-20"
Rev "1"
Comp "Tim"
Comment1 "Motherboard"
Comment2 "Two 78M05 configured as Contant Current 0.4A"
Comment3 "Re Routed Power to NANO, removed Vdrop Diode."
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5C674050
P 6650 5200
F 0 "A1" H 6650 4022 50  0001 C CNN
F 1 "Arduino_Nano_v3.x" H 6650 4113 50  0001 C CNN
F 2 "Module:Arduino_Nano_Footprint_with_Sockets" H 6800 4250 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 6650 4200 50  0001 C CNN
	1    6650 5200
	-1   0    0    1   
$EndComp
$Comp
L Regulator_Linear:L7805 U1
U 1 1 5C6740EF
P 3400 1100
F 0 "U1" H 3400 1342 50  0000 C CNN
F 1 "L7805" H 3400 1251 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3425 950 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 3400 1050 50  0001 C CNN
	1    3400 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NMOS_GDS Q1
U 1 1 5C6742D0
P 9100 1500
F 0 "Q1" H 9388 1546 50  0000 L CNN
F 1 "N-MOSFET" H 9388 1455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9300 1600 50  0001 C CNN
F 3 "~" H 9100 1500 50  0001 C CNN
	1    9100 1500
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5C674390
P 4800 3400
F 0 "R4" V 4593 3400 50  0000 C CNN
F 1 "12" V 4684 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0516_L15.5mm_D5.0mm_P7.62mm_Horizontal" V 4730 3400 50  0001 C CNN
F 3 "~" H 4800 3400 50  0001 C CNN
	1    4800 3400
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5C674410
P 4800 2500
F 0 "R3" V 4593 2500 50  0000 C CNN
F 1 "12" V 4684 2500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0516_L15.5mm_D5.0mm_P7.62mm_Horizontal" V 4730 2500 50  0001 C CNN
F 3 "~" H 4800 2500 50  0001 C CNN
	1    4800 2500
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5C674657
P 1500 2450
F 0 "R1" H 1570 2496 50  0000 L CNN
F 1 "680" H 1570 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1430 2450 50  0001 C CNN
F 3 "~" H 1500 2450 50  0001 C CNN
	1    1500 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5C674745
P 5200 5750
F 0 "R5" H 5270 5796 50  0000 L CNN
F 1 "10k" H 5270 5705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5130 5750 50  0001 C CNN
F 3 "~" H 5200 5750 50  0001 C CNN
	1    5200 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5C6747BA
P 5650 5750
F 0 "R6" H 5720 5796 50  0000 L CNN
F 1 "10k" H 5720 5705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5580 5750 50  0001 C CNN
F 3 "~" H 5650 5750 50  0001 C CNN
	1    5650 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5C6747F8
P 9200 1950
F 0 "R7" V 8993 1950 50  0000 C CNN
F 1 "10k" V 9084 1950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9130 1950 50  0001 C CNN
F 3 "~" H 9200 1950 50  0001 C CNN
	1    9200 1950
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5C674837
P 4300 1550
F 0 "R2" H 4370 1596 50  0000 L CNN
F 1 "22k" H 4370 1505 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4230 1550 50  0001 C CNN
F 3 "~" H 4300 1550 50  0001 C CNN
	1    4300 1550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Barrel_Jack_Switch J1
U 1 1 5C674A43
P 1100 1950
F 0 "J1" H 1155 2267 50  0000 C CNN
F 1 "12v Power" H 1155 2176 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal_DC_Power_Jack_002" H 1150 1910 50  0001 C CNN
F 3 "~" H 1150 1910 50  0001 C CNN
	1    1100 1950
	1    0    0    -1  
$EndComp
$Comp
L pspice:DIODE D2
U 1 1 5C674AFE
P 1850 1700
F 0 "D2" H 1850 1965 50  0000 C CNN
F 1 "DIODE" H 1850 1874 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 1850 1700 50  0001 C CNN
F 3 "~" H 1850 1700 50  0001 C CNN
	1    1850 1700
	1    0    0    -1  
$EndComp
$Comp
L pspice:DIODE D4
U 1 1 5C674C32
P 4750 1550
F 0 "D4" V 4796 1422 50  0000 R CNN
F 1 "DIODE" V 4705 1422 50  0000 R CNN
F 2 "Diode_THT:D_A-405_P10.16mm_Horizontal" H 4750 1550 50  0001 C CNN
F 3 "~" H 4750 1550 50  0001 C CNN
	1    4750 1550
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5C674DFF
P 3150 1700
F 0 "C1" V 2898 1700 50  0000 C CNN
F 1 "0.33µF" V 2989 1700 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 3188 1550 50  0001 C CNN
F 3 "~" H 3150 1700 50  0001 C CNN
	1    3150 1700
	0    1    1    0   
$EndComp
$Comp
L Device:CP C2
U 1 1 5C674F12
P 3400 2400
F 0 "C2" H 3282 2354 50  0000 R CNN
F 1 "1µF" H 3282 2445 50  0000 R CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3438 2250 50  0001 C CNN
F 3 "~" H 3400 2400 50  0001 C CNN
	1    3400 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:CP C3
U 1 1 5C674FBD
P 3850 1550
F 0 "C3" H 3968 1596 50  0000 L CNN
F 1 "22µF" H 3968 1505 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 3888 1400 50  0001 C CNN
F 3 "~" H 3850 1550 50  0001 C CNN
	1    3850 1550
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:L7805 U2
U 1 1 5C675097
P 4250 2500
F 0 "U2" H 4250 2742 50  0000 C CNN
F 1 "78M05" H 4250 2651 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 4275 2350 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4250 2450 50  0001 C CNN
	1    4250 2500
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:L7805 U3
U 1 1 5C67514C
P 4250 3400
F 0 "U3" H 4250 3642 50  0000 C CNN
F 1 "78M05" H 4250 3551 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 4275 3250 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4250 3350 50  0001 C CNN
	1    4250 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 5C675236
P 1500 2900
F 0 "D1" V 1445 2978 50  0000 L CNN
F 1 "LED" V 1536 2978 50  0000 L CNN
F 2 "LED_THT:LED_D5.0mm" H 1500 2900 50  0001 C CNN
F 3 "~" H 1500 2900 50  0001 C CNN
	1    1500 2900
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Male J9
U 1 1 5C676DDE
P 9000 800
F 0 "J9" V 9060 840 50  0000 L CNN
F 1 "5v Aux." V 9151 840 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9000 800 50  0001 C CNN
F 3 "~" H 9000 800 50  0001 C CNN
	1    9000 800 
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J6
U 1 1 5C676E63
P 7250 750
F 0 "J6" V 7310 890 50  0000 L CNN
F 1 "Servo" V 7401 890 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 7250 750 50  0001 C CNN
F 3 "~" H 7250 750 50  0001 C CNN
	1    7250 750 
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 5C676F0A
P 3950 5200
F 0 "J2" H 4056 5578 50  0000 C CNN
F 1 "Rotory Encoder" H 4056 5487 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x05_P2.54mm_Vertical" H 3950 5200 50  0001 C CNN
F 3 "~" H 3950 5200 50  0001 C CNN
	1    3950 5200
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Female J3
U 1 1 5C676F7F
P 4600 4600
F 0 "J3" H 4572 4480 50  0000 C CNN
F 1 "IC2" H 4572 4571 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 4600 4600 50  0001 C CNN
F 3 "~" H 4600 4600 50  0001 C CNN
	1    4600 4600
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x08_Female J7
U 1 1 5C6770C7
P 8350 3900
F 0 "J7" V 8547 3900 50  0000 C CNN
F 1 "To Driver 1" V 8429 3900 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 8350 3900 50  0001 C CNN
F 3 "~" H 8350 3900 50  0001 C CNN
	1    8350 3900
	1    0    0    1   
$EndComp
Wire Wire Line
	1400 1950 1500 1950
Wire Wire Line
	1500 1950 1500 2050
Wire Wire Line
	1500 2600 1500 2750
Wire Wire Line
	1500 2050 1400 2050
Connection ~ 1500 2050
Wire Wire Line
	2050 1700 2850 1700
Wire Wire Line
	3000 1700 2850 1700
Wire Wire Line
	3100 1100 2850 1100
Wire Wire Line
	2850 1100 2850 1700
Connection ~ 2850 1700
Wire Wire Line
	3400 1400 3400 1700
Wire Wire Line
	3400 1700 3300 1700
Wire Wire Line
	3400 1950 3400 1700
Connection ~ 3400 1700
Wire Wire Line
	3400 2250 3400 1950
Wire Wire Line
	3400 1950 1500 1950
Connection ~ 3400 1950
Wire Wire Line
	1500 3050 1500 3400
Wire Wire Line
	1500 3400 2850 3400
Wire Wire Line
	3950 2500 3800 2500
Wire Wire Line
	3800 3400 3950 3400
Wire Wire Line
	2850 3400 3400 3400
Connection ~ 2850 3400
Connection ~ 3800 3400
Wire Wire Line
	3850 1700 3850 1950
Wire Wire Line
	4300 1700 4300 1950
Wire Wire Line
	4750 1750 4750 1950
Wire Wire Line
	3700 1100 3850 1100
Wire Wire Line
	3850 1100 3850 1400
Wire Wire Line
	4300 1100 4300 1400
Wire Wire Line
	4750 1350 4750 1100
Wire Wire Line
	9300 1500 9450 1500
Wire Wire Line
	9450 1500 9450 1950
Wire Wire Line
	9450 1950 9350 1950
Wire Wire Line
	9000 1700 9000 1950
Wire Wire Line
	9000 1950 9050 1950
Connection ~ 9000 1950
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5C683883
P 5400 2500
F 0 "J4" H 5373 2473 50  0000 R CNN
F 1 "Motor Power 1" H 5373 2382 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5400 2500 50  0001 C CNN
F 3 "~" H 5400 2500 50  0001 C CNN
	1    5400 2500
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J5
U 1 1 5C683906
P 5400 3400
F 0 "J5" H 5373 3373 50  0000 R CNN
F 1 "Motor Power 2" H 5373 3282 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5400 3400 50  0001 C CNN
F 3 "~" H 5400 3400 50  0001 C CNN
	1    5400 3400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5200 3500 5100 3500
Wire Wire Line
	5100 3500 5100 2600
Wire Wire Line
	5100 2600 5200 2600
Wire Wire Line
	5100 2600 5100 1950
Connection ~ 5100 2600
Connection ~ 5100 1950
Wire Wire Line
	7250 1100 7250 950 
Connection ~ 4750 1100
Wire Wire Line
	7250 1100 8900 1100
Wire Wire Line
	8900 1100 8900 1000
Connection ~ 7250 1100
Wire Wire Line
	7350 5950 9450 5950
Wire Wire Line
	9450 5950 9450 1950
Wire Wire Line
	7150 950  7150 1950
Connection ~ 7150 1950
Wire Wire Line
	7150 1950 9000 1950
Wire Wire Line
	4650 2500 4550 2500
Wire Wire Line
	4650 3400 4550 3400
Wire Wire Line
	4950 2500 5000 2500
Wire Wire Line
	4250 2850 4250 2800
Wire Wire Line
	4950 3400 5000 3400
Wire Wire Line
	5000 3400 5000 3750
Wire Wire Line
	5000 3750 4250 3750
Wire Wire Line
	4250 3750 4250 3700
Wire Wire Line
	5200 3400 5000 3400
Connection ~ 5000 3400
Wire Wire Line
	8200 5500 8050 5500
Wire Wire Line
	8050 5500 8050 5400
Wire Wire Line
	8050 5400 8200 5400
Wire Wire Line
	8150 3500 8050 3500
Wire Wire Line
	8050 3500 8050 3600
Wire Wire Line
	8050 3600 8150 3600
Wire Wire Line
	7150 4900 7350 4900
Wire Wire Line
	7850 4100 7850 4900
Wire Wire Line
	7850 4900 8200 4900
Wire Wire Line
	6650 4200 6650 4100
Wire Wire Line
	6650 4100 7850 4100
Connection ~ 7850 4100
Wire Wire Line
	6450 6200 6450 6400
Wire Wire Line
	6450 6400 7950 6400
Wire Wire Line
	7950 4200 8150 4200
Wire Wire Line
	7950 4200 7950 4800
Wire Wire Line
	8200 4800 7950 4800
Connection ~ 7950 4800
Wire Wire Line
	7950 4800 7950 6400
Connection ~ 8050 5400
Connection ~ 8050 3600
Wire Wire Line
	7150 5400 8050 5400
Wire Wire Line
	8050 3600 8050 5400
Wire Wire Line
	9000 1000 9000 1300
Wire Wire Line
	7350 950  7350 4900
Wire Wire Line
	5000 2500 5200 2500
Wire Wire Line
	4250 2850 5000 2850
Connection ~ 5000 2500
Wire Wire Line
	5000 2500 5000 2850
Wire Wire Line
	3800 2500 3800 3400
Wire Wire Line
	3400 2550 3400 3400
Connection ~ 3400 3400
Wire Wire Line
	3400 3400 3800 3400
Connection ~ 1500 1950
Wire Wire Line
	1400 1850 1400 1700
Wire Wire Line
	2850 1700 2850 3400
Connection ~ 3850 1100
Wire Wire Line
	3850 1100 4300 1100
Connection ~ 3850 1950
Wire Wire Line
	3850 1950 3400 1950
Connection ~ 4300 1100
Connection ~ 4300 1950
Wire Wire Line
	4300 1950 3850 1950
Connection ~ 4750 1950
Wire Wire Line
	4750 1950 4300 1950
Wire Wire Line
	4300 1100 4750 1100
Wire Wire Line
	4750 1950 5100 1950
Wire Wire Line
	1500 2050 1500 2300
Wire Wire Line
	1400 1700 1650 1700
Wire Wire Line
	7150 5300 8200 5300
Wire Wire Line
	8200 5200 7150 5200
Wire Wire Line
	8200 5100 7150 5100
Wire Wire Line
	8200 5000 7150 5000
Wire Wire Line
	7850 4100 8150 4100
Wire Wire Line
	8150 4000 7750 4000
Wire Wire Line
	7750 4000 7750 4800
Wire Wire Line
	7750 4800 7150 4800
Wire Wire Line
	8150 3900 7650 3900
Wire Wire Line
	7650 3900 7650 4700
Wire Wire Line
	7650 4700 7150 4700
Wire Wire Line
	8150 3800 7550 3800
Wire Wire Line
	7550 3800 7550 4600
Wire Wire Line
	7550 4600 7150 4600
Wire Wire Line
	8150 3700 7450 3700
Wire Wire Line
	7450 3700 7450 4500
Wire Wire Line
	7450 4500 7150 4500
Wire Wire Line
	7350 5950 7350 5500
Wire Wire Line
	7350 5500 7150 5500
Wire Wire Line
	4150 5000 6150 5000
Wire Wire Line
	6150 5100 4150 5100
Wire Wire Line
	6150 5200 4150 5200
Wire Wire Line
	4800 4700 5650 4700
Wire Wire Line
	4800 4600 5000 4600
Wire Wire Line
	5000 4600 5000 5300
Wire Wire Line
	5000 5300 4150 5300
Wire Wire Line
	4800 4500 5100 4500
Wire Wire Line
	5100 4500 5100 5400
Wire Wire Line
	5100 5400 4150 5400
Connection ~ 5100 3500
Connection ~ 5100 4500
Wire Wire Line
	5000 6400 5000 5300
Connection ~ 6450 6400
Connection ~ 5000 5300
Wire Wire Line
	5200 5900 5200 6400
Wire Wire Line
	5000 6400 5200 6400
Connection ~ 5200 6400
Wire Wire Line
	5200 6400 5650 6400
Wire Wire Line
	5650 5900 5650 6400
Wire Wire Line
	5200 5600 5200 4800
Wire Wire Line
	4800 4800 5200 4800
Wire Wire Line
	5100 3500 5100 4500
Connection ~ 5200 4800
Connection ~ 5650 6400
Connection ~ 9450 1950
Wire Wire Line
	5200 4800 6150 4800
Wire Wire Line
	5650 5600 5650 4700
Wire Wire Line
	5650 4700 6150 4700
Connection ~ 5650 4700
Wire Wire Line
	5650 6400 6450 6400
Wire Wire Line
	2850 6600 6750 6600
Wire Wire Line
	6750 6200 6750 6600
Wire Wire Line
	6550 4200 6550 4100
Wire Wire Line
	6550 4100 6650 4100
Connection ~ 6650 4100
Wire Wire Line
	6650 4100 6650 1950
Wire Wire Line
	5100 1950 6650 1950
Connection ~ 6650 1950
Wire Wire Line
	6650 1950 7150 1950
$Comp
L power:PWR_FLAG #GND0101
U 1 1 5C6C2DDB
P 1500 1950
F 0 "#GND0101" H 1500 2025 50  0001 C CNN
F 1 "PWR_FLAG" H 1568 1994 50  0000 L CNN
F 2 "" H 1500 1950 50  0001 C CNN
F 3 "~" H 1500 1950 50  0001 C CNN
	1    1500 1950
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5C6C2E70
P 1400 1700
F 0 "#FLG0101" H 1400 1775 50  0001 C CNN
F 1 "PWR_FLAG" H 1400 1874 50  0000 C CNN
F 2 "" H 1400 1700 50  0001 C CNN
F 3 "~" H 1400 1700 50  0001 C CNN
	1    1400 1700
	1    0    0    -1  
$EndComp
Connection ~ 1400 1700
$Comp
L Connector:Conn_01x08_Female J8
U 1 1 5C7FF6AE
P 8400 5100
F 0 "J8" V 8597 5100 50  0000 C CNN
F 1 "To Driver 2" V 8479 5100 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x08_P2.54mm_Vertical" H 8400 5100 50  0001 C CNN
F 3 "~" H 8400 5100 50  0001 C CNN
	1    8400 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1100 7250 1100
Wire Wire Line
	2850 6600 2850 3400
$EndSCHEMATC
