EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 2 2
Title "G.E.A.R. Sequencer"
Date "2021-07-29"
Rev ""
Comp "Rich Holmes / Analog Output"
Comment1 "or neighboring rights to this work. Published from United States."
Comment2 "To the extent possible under law, Richard Holmes has waived all copyright and related "
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND1 #PWR051
U 1 1 6112F162
P 4950 4750
F 0 "#PWR051" H 4950 4500 50  0001 C CNN
F 1 "GND1" H 4955 4577 50  0000 C CNN
F 2 "" H 4950 4750 50  0001 C CNN
F 3 "" H 4950 4750 50  0001 C CNN
	1    4950 4750
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:OLED_128X64 OLED1
U 1 1 6112F273
P 7000 3800
F 0 "OLED1" H 7278 3846 50  0000 L CNN
F 1 "OLED_128X64" H 7278 3755 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7000 3800 50  0001 C CNN
F 3 "" H 7000 3800 50  0001 C CNN
	1    7000 3800
	1    0    0    -1  
$EndComp
Text Notes 7300 4050 0    50   ~ 0
(or similar with \nVCC, GND reversed)
NoConn ~ 6850 3500
NoConn ~ 6950 3500
NoConn ~ 7050 3500
NoConn ~ 7150 3500
Text Notes 6700 3350 0    50   ~ 0
OLED (panel mounted)
Wire Notes Line
	6600 3200 8150 3200
Wire Notes Line
	8150 3200 8150 4150
Wire Notes Line
	8150 4150 6600 4150
Wire Notes Line
	6600 4150 6600 3200
$Comp
L Device:R_POT RV1
U 1 1 6114F64E
P 5150 2500
F 0 "RV1" V 4943 2500 50  0000 C CNN
F 1 "10K" V 5034 2500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 5150 2500 50  0001 C CNN
F 3 "~" H 5150 2500 50  0001 C CNN
	1    5150 2500
	0    -1   1    0   
$EndComp
$Comp
L Device:R_POT RV2
U 1 1 6114F830
P 5150 3000
F 0 "RV2" V 4943 3000 50  0000 C CNN
F 1 "10K" V 5034 3000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 5150 3000 50  0001 C CNN
F 3 "~" H 5150 3000 50  0001 C CNN
	1    5150 3000
	0    -1   1    0   
$EndComp
$Comp
L Device:R_POT RV3
U 1 1 61150713
P 5150 3500
F 0 "RV3" V 4943 3500 50  0000 C CNN
F 1 "10K" V 5034 3500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 5150 3500 50  0001 C CNN
F 3 "~" H 5150 3500 50  0001 C CNN
	1    5150 3500
	0    -1   1    0   
$EndComp
$Comp
L Device:R_POT RV4
U 1 1 61150719
P 5150 4000
F 0 "RV4" V 4943 4000 50  0000 C CNN
F 1 "10K" V 5034 4000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 5150 4000 50  0001 C CNN
F 3 "~" H 5150 4000 50  0001 C CNN
	1    5150 4000
	0    -1   1    0   
$EndComp
Wire Wire Line
	5000 2500 4950 2500
Wire Wire Line
	4950 2500 4950 3000
Wire Wire Line
	5300 2500 5350 2500
Wire Wire Line
	4950 3000 5000 3000
Connection ~ 4950 3000
Wire Wire Line
	4950 3000 4950 3500
Wire Wire Line
	5300 3000 5350 3000
Wire Wire Line
	5300 3500 5350 3500
Wire Wire Line
	5000 3500 4950 3500
Connection ~ 4950 3500
Wire Wire Line
	4950 3500 4950 4000
Wire Wire Line
	5300 4000 5350 4000
Wire Wire Line
	5000 4000 4950 4000
Connection ~ 4950 4000
Wire Wire Line
	5350 2500 5350 2300
$Comp
L ao_symbols:MountingHole H?
U 1 1 6146FF72
P 2300 5900
AR Path="/6146FF72" Ref="H?"  Part="1" 
AR Path="/61124C5F/6146FF72" Ref="H4"  Part="1" 
F 0 "H4" H 2400 5946 50  0000 L CNN
F 1 "MountingHole" H 2400 5855 50  0000 L CNN
F 2 "ao_tht:MountingHole_3.2mm_M3" H 2300 5900 50  0001 C CNN
F 3 "" H 2300 5900 50  0001 C CNN
F 4 "DNF" H 2300 5900 50  0001 C CNN "Config"
	1    2300 5900
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:MountingHole H?
U 1 1 6146FF79
P 3100 5900
AR Path="/6146FF79" Ref="H?"  Part="1" 
AR Path="/61124C5F/6146FF79" Ref="H6"  Part="1" 
F 0 "H6" H 3200 5946 50  0000 L CNN
F 1 "MountingHole" H 3200 5855 50  0000 L CNN
F 2 "ao_tht:MountingHole_3.2mm_M3" H 3100 5900 50  0001 C CNN
F 3 "" H 3100 5900 50  0001 C CNN
F 4 "DNF" H 3100 5900 50  0001 C CNN "Config"
	1    3100 5900
	1    0    0    -1  
$EndComp
Text Label 5350 2300 0    50   ~ 0
VCC
Wire Notes Line
	2000 5550 3800 5550
Wire Notes Line
	3800 5550 3800 6450
Wire Notes Line
	3800 6450 2000 6450
Wire Notes Line
	2000 6450 2000 5550
Text Notes 2150 5700 0    50   ~ 0
Hardware
$Comp
L power:GND1 #PWR050
U 1 1 6133F470
P 3350 3550
F 0 "#PWR050" H 3350 3300 50  0001 C CNN
F 1 "GND1" H 3355 3377 50  0000 C CNN
F 2 "" H 3350 3550 50  0001 C CNN
F 3 "" H 3350 3550 50  0001 C CNN
	1    3350 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x11 J14
U 1 1 6133F476
P 3850 3300
F 0 "J14" V 4050 3300 50  0000 C CNN
F 1 "Conn_01x11" V 3950 3300 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x11_P2.54mm_Vertical" H 3850 3300 50  0001 C CNN
F 3 "~" H 3850 3300 50  0001 C CNN
	1    3850 3300
	0    -1   -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 6133F499
P 2250 3800
F 0 "#FLG02" H 2250 3875 50  0001 C CNN
F 1 "PWR_FLAG" H 2250 3973 50  0000 C CNN
F 2 "" H 2250 3800 50  0001 C CNN
F 3 "~" H 2250 3800 50  0001 C CNN
	1    2250 3800
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 6133F4A6
P 2550 3600
F 0 "#FLG03" H 2550 3675 50  0001 C CNN
F 1 "PWR_FLAG" H 2550 3773 50  0000 C CNN
F 2 "" H 2550 3600 50  0001 C CNN
F 3 "~" H 2550 3600 50  0001 C CNN
	1    2550 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4000 4950 4500
Wire Wire Line
	5000 4500 4950 4500
Connection ~ 4950 4500
Wire Wire Line
	4950 4500 4950 4750
Wire Wire Line
	3350 3500 3350 3550
Text GLabel 3650 3500 3    50   Output ~ 0
POT_1_CW_P
Text GLabel 3550 3500 3    50   Input ~ 0
POT_1_W_P
Text GLabel 3850 3500 3    50   Output ~ 0
POT_2_CW_P
Text GLabel 3750 3500 3    50   Input ~ 0
POT_2_W_P
Text GLabel 4050 3500 3    50   Output ~ 0
POT_3_CW_P
Text GLabel 4150 3500 3    50   Input ~ 0
POT_3_W_P
Text GLabel 4250 3500 3    50   Output ~ 0
POT_4_CW_P
Text GLabel 4350 3500 3    50   Input ~ 0
POT_4_W_P
Text GLabel 3950 3500 3    50   Input ~ 0
POT_ALG_P
Text Label 3450 3800 3    50   ~ 0
VCC
Wire Wire Line
	3450 3500 3450 3800
$Comp
L power:GND1 #PWR049
U 1 1 617444E6
P 2550 3600
F 0 "#PWR049" H 2550 3350 50  0001 C CNN
F 1 "GND1" H 2555 3427 50  0000 C CNN
F 2 "" H 2550 3600 50  0001 C CNN
F 3 "" H 2550 3600 50  0001 C CNN
	1    2550 3600
	1    0    0    -1  
$EndComp
Text Label 2250 3600 1    50   ~ 0
VCC
Wire Wire Line
	2250 3600 2250 3800
Text GLabel 5350 3000 2    50   Input ~ 0
POT_1_CW_P
Text GLabel 5400 3200 2    50   Output ~ 0
POT_1_W_P
Wire Wire Line
	5150 3150 5150 3200
Wire Wire Line
	5150 3200 5400 3200
Text GLabel 5350 3500 2    50   Input ~ 0
POT_2_CW_P
Text GLabel 5350 4000 2    50   Input ~ 0
POT_3_CW_P
Text GLabel 5400 4500 2    50   Input ~ 0
POT_4_CW_P
Text GLabel 5400 3700 2    50   Output ~ 0
POT_2_W_P
Wire Wire Line
	5150 3650 5150 3700
Wire Wire Line
	5150 3700 5400 3700
Text GLabel 5400 4200 2    50   Output ~ 0
POT_3_W_P
Wire Wire Line
	5150 4150 5150 4200
Wire Wire Line
	5150 4200 5400 4200
Text GLabel 5400 4700 2    50   Output ~ 0
POT_4_W_P
Wire Wire Line
	5150 4650 5150 4700
Wire Wire Line
	5150 4700 5400 4700
Text GLabel 5400 2700 2    50   Output ~ 0
POT_ALG_P
Wire Wire Line
	5150 2650 5150 2700
Wire Wire Line
	5150 2700 5400 2700
$Comp
L Device:R_POT RV5
U 1 1 61152468
P 5150 4500
F 0 "RV5" V 4943 4500 50  0000 C CNN
F 1 "10K" V 5034 4500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 5150 4500 50  0001 C CNN
F 3 "~" H 5150 4500 50  0001 C CNN
	1    5150 4500
	0    -1   1    0   
$EndComp
Wire Wire Line
	5300 4500 5400 4500
$Comp
L ao_symbols:MountingHole H?
U 1 1 618E5B83
P 2300 6250
AR Path="/618E5B83" Ref="H?"  Part="1" 
AR Path="/61124C5F/618E5B83" Ref="H5"  Part="1" 
F 0 "H5" H 2400 6296 50  0000 L CNN
F 1 "MountingHole" H 2400 6205 50  0000 L CNN
F 2 "ao_tht:MountingHole_3.2mm_M3" H 2300 6250 50  0001 C CNN
F 3 "" H 2300 6250 50  0001 C CNN
F 4 "DNF" H 2300 6250 50  0001 C CNN "Config"
	1    2300 6250
	1    0    0    -1  
$EndComp
Wire Notes Line
	6400 2000 6400 5250
Wire Notes Line
	6400 5250 1900 5250
Wire Notes Line
	1900 5250 1900 2000
Wire Notes Line
	1900 2000 6400 2000
Text Notes 2300 2300 2    50   ~ 0
Pots
$Comp
L ao_symbols:SW_Push SW1
U 1 1 615ADE7E
P 6950 5000
F 0 "SW1" H 6950 5285 50  0000 C CNN
F 1 "SW_Push" H 6950 5194 50  0000 C CNN
F 2 "ao_tht:Push_button_A-5050" H 6950 5000 50  0001 C CNN
F 3 "~" H 6950 5000 50  0001 C CNN
F 4 "Tayda" H 6950 5000 50  0001 C CNN "Vendor"
F 5 "A-3487" H 6950 5000 50  0001 C CNN "SKU"
	1    6950 5000
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:2_pin_Molex_connector J15
U 1 1 615AE230
P 7000 5450
F 0 "J15" V 7150 5400 50  0000 L CNN
F 1 "2_pin_Molex_connector" V 7250 5000 50  0000 L CNN
F 2 "ao_tht:Molex_KK-254_AE-6410-02A_1x02_P2.54mm_Vertical" H 7000 5450 50  0001 C CNN
F 3 "" H 7000 5450 50  0001 C CNN
F 4 "Tayda" H 7000 5450 50  0001 C CNN "Vendor"
F 5 "A-826" H 7000 5450 50  0001 C CNN "SKU"
	1    7000 5450
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 5000 6700 5000
Wire Wire Line
	6700 5000 6700 5250
Wire Wire Line
	6700 5250 6900 5250
Wire Wire Line
	7150 5000 7200 5000
Wire Wire Line
	7200 5000 7200 5250
Wire Wire Line
	7200 5250 7000 5250
$Comp
L ao_symbols:SW_Push SW2
U 1 1 615B2113
P 7900 5000
F 0 "SW2" H 7900 5285 50  0000 C CNN
F 1 "SW_Push" H 7900 5194 50  0000 C CNN
F 2 "ao_tht:Push_button_A-5050" H 7900 5000 50  0001 C CNN
F 3 "~" H 7900 5000 50  0001 C CNN
F 4 "Tayda" H 7900 5000 50  0001 C CNN "Vendor"
F 5 "A-3487" H 7900 5000 50  0001 C CNN "SKU"
	1    7900 5000
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:2_pin_Molex_connector J16
U 1 1 615B211B
P 7950 5450
F 0 "J16" V 8100 5400 50  0000 L CNN
F 1 "2_pin_Molex_connector" V 8200 5000 50  0000 L CNN
F 2 "ao_tht:Molex_KK-254_AE-6410-02A_1x02_P2.54mm_Vertical" H 7950 5450 50  0001 C CNN
F 3 "" H 7950 5450 50  0001 C CNN
F 4 "Tayda" H 7950 5450 50  0001 C CNN "Vendor"
F 5 "A-826" H 7950 5450 50  0001 C CNN "SKU"
	1    7950 5450
	0    1    1    0   
$EndComp
Wire Wire Line
	7700 5000 7650 5000
Wire Wire Line
	7650 5000 7650 5250
Wire Wire Line
	7650 5250 7850 5250
Wire Wire Line
	8100 5000 8150 5000
Wire Wire Line
	8150 5000 8150 5250
Wire Wire Line
	8150 5250 7950 5250
Wire Notes Line
	6500 4300 8500 4300
Wire Notes Line
	8500 4300 8500 5800
Wire Notes Line
	8500 5800 6500 5800
Wire Notes Line
	6500 5800 6500 4300
Text Notes 6600 4600 0    50   ~ 0
Push buttons (panel mounted)\nConnect to Clock and Reset inputs\nvia Molex
$EndSCHEMATC
