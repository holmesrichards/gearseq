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
P 3550 4750
F 0 "#PWR051" H 3550 4500 50  0001 C CNN
F 1 "GND1" H 3555 4577 50  0000 C CNN
F 2 "" H 3550 4750 50  0001 C CNN
F 3 "" H 3550 4750 50  0001 C CNN
	1    3550 4750
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
L ao_symbols:R_POT RV1
U 1 1 6114F64E
P 3750 2500
F 0 "RV1" V 3543 2500 50  0000 C CNN
F 1 "10k" V 3634 2500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 3750 2500 50  0001 C CNN
F 3 "~" H 3750 2500 50  0001 C CNN
F 4 "Tayda" H 3750 2500 50  0001 C CNN "Vendor"
	1    3750 2500
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV2
U 1 1 6114F830
P 3750 3000
F 0 "RV2" V 3543 3000 50  0000 C CNN
F 1 "100k" V 3634 3000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 3750 3000 50  0001 C CNN
F 3 "~" H 3750 3000 50  0001 C CNN
F 4 "Tayda" H 3750 3000 50  0001 C CNN "Vendor"
	1    3750 3000
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV3
U 1 1 61150713
P 3750 3500
F 0 "RV3" V 3543 3500 50  0000 C CNN
F 1 "100k" V 3634 3500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 3750 3500 50  0001 C CNN
F 3 "~" H 3750 3500 50  0001 C CNN
F 4 "Tayda" H 3750 3500 50  0001 C CNN "Vendor"
	1    3750 3500
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV4
U 1 1 61150719
P 3750 4000
F 0 "RV4" V 3543 4000 50  0000 C CNN
F 1 "100k" V 3634 4000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 3750 4000 50  0001 C CNN
F 3 "~" H 3750 4000 50  0001 C CNN
F 4 "Tayda" H 3750 4000 50  0001 C CNN "Vendor"
	1    3750 4000
	0    -1   1    0   
$EndComp
Wire Wire Line
	3600 2500 3550 2500
Wire Wire Line
	3550 2500 3550 3000
Wire Wire Line
	3900 2500 3950 2500
Wire Wire Line
	3550 3000 3600 3000
Connection ~ 3550 3000
Wire Wire Line
	3550 3000 3550 3500
Wire Wire Line
	3900 3000 3950 3000
Wire Wire Line
	3900 3500 3950 3500
Wire Wire Line
	3600 3500 3550 3500
Connection ~ 3550 3500
Wire Wire Line
	3550 3500 3550 4000
Wire Wire Line
	3900 4000 3950 4000
Wire Wire Line
	3600 4000 3550 4000
Connection ~ 3550 4000
Wire Wire Line
	3950 2500 3950 2300
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
P 2100 2800
F 0 "#PWR050" H 2100 2550 50  0001 C CNN
F 1 "GND1" H 2105 2627 50  0000 C CNN
F 2 "" H 2100 2800 50  0001 C CNN
F 3 "" H 2100 2800 50  0001 C CNN
	1    2100 2800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x11 J14
U 1 1 6133F476
P 2600 2550
F 0 "J14" V 2800 2550 50  0000 C CNN
F 1 "Conn_01x11" V 2700 2550 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x11_P2.54mm_Vertical" H 2600 2550 50  0001 C CNN
F 3 "~" H 2600 2550 50  0001 C CNN
	1    2600 2550
	0    -1   -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 6133F499
P 2300 3950
F 0 "#FLG02" H 2300 4025 50  0001 C CNN
F 1 "PWR_FLAG" H 2300 4123 50  0000 C CNN
F 2 "" H 2300 3950 50  0001 C CNN
F 3 "~" H 2300 3950 50  0001 C CNN
	1    2300 3950
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 6133F4A6
P 2750 3850
F 0 "#FLG03" H 2750 3925 50  0001 C CNN
F 1 "PWR_FLAG" H 2750 4023 50  0000 C CNN
F 2 "" H 2750 3850 50  0001 C CNN
F 3 "~" H 2750 3850 50  0001 C CNN
	1    2750 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 4000 3550 4500
Wire Wire Line
	3600 4500 3550 4500
Connection ~ 3550 4500
Wire Wire Line
	3550 4500 3550 4750
Wire Wire Line
	2100 2750 2100 2800
Text GLabel 2400 2750 3    50   Output ~ 0
POT_1_CW_P
Text GLabel 2300 2750 3    50   Input ~ 0
POT_1_W_P
Text GLabel 2600 2750 3    50   Output ~ 0
POT_2_CW_P
Text GLabel 2500 2750 3    50   Input ~ 0
POT_2_W_P
Text GLabel 2800 2750 3    50   Output ~ 0
POT_3_CW_P
Text GLabel 2900 2750 3    50   Input ~ 0
POT_3_W_P
Text GLabel 3000 2750 3    50   Output ~ 0
POT_4_CW_P
Text GLabel 3100 2750 3    50   Input ~ 0
POT_4_W_P
Text GLabel 2700 2750 3    50   Input ~ 0
POT_ALG_P
$Comp
L power:GND1 #PWR049
U 1 1 617444E6
P 2750 3850
F 0 "#PWR049" H 2750 3600 50  0001 C CNN
F 1 "GND1" H 2755 3677 50  0000 C CNN
F 2 "" H 2750 3850 50  0001 C CNN
F 3 "" H 2750 3850 50  0001 C CNN
	1    2750 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3750 2300 3950
Text GLabel 3950 3000 2    50   Input ~ 0
POT_1_CW_P
Text GLabel 4000 3200 2    50   Output ~ 0
POT_1_W_P
Wire Wire Line
	3750 3150 3750 3200
Wire Wire Line
	3750 3200 4000 3200
Text GLabel 3950 3500 2    50   Input ~ 0
POT_2_CW_P
Text GLabel 3950 4000 2    50   Input ~ 0
POT_3_CW_P
Text GLabel 4000 4500 2    50   Input ~ 0
POT_4_CW_P
Text GLabel 4000 3700 2    50   Output ~ 0
POT_2_W_P
Wire Wire Line
	3750 3650 3750 3700
Wire Wire Line
	3750 3700 4000 3700
Text GLabel 4000 4200 2    50   Output ~ 0
POT_3_W_P
Wire Wire Line
	3750 4150 3750 4200
Wire Wire Line
	3750 4200 4000 4200
Text GLabel 4000 4700 2    50   Output ~ 0
POT_4_W_P
Wire Wire Line
	3750 4650 3750 4700
Wire Wire Line
	3750 4700 4000 4700
Text GLabel 4000 2700 2    50   Output ~ 0
POT_ALG_P
Wire Wire Line
	3750 2650 3750 2700
Wire Wire Line
	3750 2700 4000 2700
$Comp
L ao_symbols:R_POT RV5
U 1 1 61152468
P 3750 4500
F 0 "RV5" V 3543 4500 50  0000 C CNN
F 1 "100k" V 3634 4500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_RD901F-40-00D_Single_Vertical_centered" H 3750 4500 50  0001 C CNN
F 3 "~" H 3750 4500 50  0001 C CNN
F 4 "Tayda" H 3750 4500 50  0001 C CNN "Vendor"
	1    3750 4500
	0    -1   1    0   
$EndComp
Wire Wire Line
	3900 4500 4000 4500
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
L ao_symbols:SW_Push_black SW1
U 1 1 615ADE7E
P 6950 5000
F 0 "SW1" H 6950 5285 50  0000 C CNN
F 1 "SW_Push_black" H 6950 5194 50  0000 C CNN
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
L ao_symbols:SW_Push_red SW2
U 1 1 615B2113
P 7900 5000
F 0 "SW2" H 7900 5285 50  0000 C CNN
F 1 "SW_Push_red" H 7900 5194 50  0000 C CNN
F 2 "ao_tht:Push_button_A-5050" H 7900 5000 50  0001 C CNN
F 3 "~" H 7900 5000 50  0001 C CNN
F 4 "Tayda" H 7900 5000 50  0001 C CNN "Vendor"
F 5 "A-3486" H 7900 5000 50  0001 C CNN "SKU"
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
	6500 4300 9500 4300
Wire Notes Line
	9500 4300 9500 5800
Wire Notes Line
	9500 5800 6500 5800
Wire Notes Line
	6500 5800 6500 4300
Text Notes 6600 4600 0    50   ~ 0
Push buttons and toggle (panel mounted)\nConnect to Clock, Reset, Display Blank inputs\nvia Molex
$Comp
L power:GND1 #PWR052
U 1 1 617F83DF
P 4800 4750
F 0 "#PWR052" H 4800 4500 50  0001 C CNN
F 1 "GND1" H 4805 4577 50  0000 C CNN
F 2 "" H 4800 4750 50  0001 C CNN
F 3 "" H 4800 4750 50  0001 C CNN
	1    4800 4750
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:R_POT RV6
U 1 1 617F83E5
P 5000 2500
F 0 "RV6" V 4793 2500 50  0000 C CNN
F 1 "10k" V 4884 2500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_16mm_Single_Vertical" H 5000 2500 50  0001 C CNN
F 3 "~" H 5000 2500 50  0001 C CNN
F 4 "Tayda" H 5000 2500 50  0001 C CNN "Vendor"
	1    5000 2500
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV7
U 1 1 617F83EB
P 5000 3000
F 0 "RV7" V 4793 3000 50  0000 C CNN
F 1 "100k" V 4884 3000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_16mm_Single_Vertical" H 5000 3000 50  0001 C CNN
F 3 "~" H 5000 3000 50  0001 C CNN
F 4 "Tayda" H 5000 3000 50  0001 C CNN "Vendor"
	1    5000 3000
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV8
U 1 1 617F83F1
P 5000 3500
F 0 "RV8" V 4793 3500 50  0000 C CNN
F 1 "100k" V 4884 3500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_16mm_Single_Vertical" H 5000 3500 50  0001 C CNN
F 3 "~" H 5000 3500 50  0001 C CNN
F 4 "Tayda" H 5000 3500 50  0001 C CNN "Vendor"
	1    5000 3500
	0    -1   1    0   
$EndComp
$Comp
L ao_symbols:R_POT RV9
U 1 1 617F83F7
P 5000 4000
F 0 "RV9" V 4793 4000 50  0000 C CNN
F 1 "100k" V 4884 4000 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_16mm_Single_Vertical" H 5000 4000 50  0001 C CNN
F 3 "~" H 5000 4000 50  0001 C CNN
	1    5000 4000
	0    -1   1    0   
$EndComp
Wire Wire Line
	4850 2500 4800 2500
Wire Wire Line
	4800 2500 4800 3000
Wire Wire Line
	5150 2500 5200 2500
Wire Wire Line
	4800 3000 4850 3000
Connection ~ 4800 3000
Wire Wire Line
	4800 3000 4800 3500
Wire Wire Line
	5150 3000 5200 3000
Wire Wire Line
	5150 3500 5200 3500
Wire Wire Line
	4850 3500 4800 3500
Connection ~ 4800 3500
Wire Wire Line
	4800 3500 4800 4000
Wire Wire Line
	5150 4000 5200 4000
Wire Wire Line
	4850 4000 4800 4000
Connection ~ 4800 4000
Wire Wire Line
	5200 2500 5200 2300
Wire Wire Line
	4800 4000 4800 4500
Wire Wire Line
	4850 4500 4800 4500
Connection ~ 4800 4500
Wire Wire Line
	4800 4500 4800 4750
Text GLabel 5200 3000 2    50   Input ~ 0
POT_1_CW_P
Text GLabel 5250 3200 2    50   Output ~ 0
POT_1_W_P
Wire Wire Line
	5000 3150 5000 3200
Wire Wire Line
	5000 3200 5250 3200
Text GLabel 5200 3500 2    50   Input ~ 0
POT_2_CW_P
Text GLabel 5200 4000 2    50   Input ~ 0
POT_3_CW_P
Text GLabel 5250 4500 2    50   Input ~ 0
POT_4_CW_P
Text GLabel 5250 3700 2    50   Output ~ 0
POT_2_W_P
Wire Wire Line
	5000 3650 5000 3700
Wire Wire Line
	5000 3700 5250 3700
Text GLabel 5250 4200 2    50   Output ~ 0
POT_3_W_P
Wire Wire Line
	5000 4150 5000 4200
Wire Wire Line
	5000 4200 5250 4200
Text GLabel 5250 4700 2    50   Output ~ 0
POT_4_W_P
Wire Wire Line
	5000 4650 5000 4700
Wire Wire Line
	5000 4700 5250 4700
Text GLabel 5250 2700 2    50   Output ~ 0
POT_ALG_P
Wire Wire Line
	5000 2650 5000 2700
Wire Wire Line
	5000 2700 5250 2700
$Comp
L ao_symbols:R_POT RV10
U 1 1 617F8424
P 5000 4500
F 0 "RV10" V 4793 4500 50  0000 C CNN
F 1 "100k" V 4884 4500 50  0000 C CNN
F 2 "ao_tht:Potentiometer_Alpha_16mm_Single_Vertical" H 5000 4500 50  0001 C CNN
F 3 "~" H 5000 4500 50  0001 C CNN
F 4 "Tayda" H 5000 4500 50  0001 C CNN "Vendor"
	1    5000 4500
	0    -1   1    0   
$EndComp
Wire Wire Line
	5150 4500 5250 4500
Text Notes 4150 5100 0    50   ~ 0
Fit either 9 mm or 16 mm pots
$Comp
L ao_symbols:+5V_1 #PWR?
U 1 1 6186CB18
P 2200 3150
AR Path="/6186CB18" Ref="#PWR?"  Part="1" 
AR Path="/61124C5F/6186CB18" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 2200 3000 50  0001 C CNN
F 1 "+5V_1" H 2215 3323 50  0000 C CNN
F 2 "" H 2200 3150 50  0001 C CNN
F 3 "" H 2200 3150 50  0001 C CNN
	1    2200 3150
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 2750 2200 3150
$Comp
L ao_symbols:+5V_1 #PWR?
U 1 1 6186EA4F
P 2300 3750
AR Path="/6186EA4F" Ref="#PWR?"  Part="1" 
AR Path="/61124C5F/6186EA4F" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 2300 3600 50  0001 C CNN
F 1 "+5V_1" H 2315 3923 50  0000 C CNN
F 2 "" H 2300 3750 50  0001 C CNN
F 3 "" H 2300 3750 50  0001 C CNN
	1    2300 3750
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:+5V_1 #PWR?
U 1 1 6187176A
P 3950 2300
AR Path="/6187176A" Ref="#PWR?"  Part="1" 
AR Path="/61124C5F/6187176A" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 3950 2150 50  0001 C CNN
F 1 "+5V_1" H 3965 2473 50  0000 C CNN
F 2 "" H 3950 2300 50  0001 C CNN
F 3 "" H 3950 2300 50  0001 C CNN
	1    3950 2300
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:+5V_1 #PWR?
U 1 1 61871E11
P 5200 2300
AR Path="/61871E11" Ref="#PWR?"  Part="1" 
AR Path="/61124C5F/61871E11" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 5200 2150 50  0001 C CNN
F 1 "+5V_1" H 5215 2473 50  0000 C CNN
F 2 "" H 5200 2300 50  0001 C CNN
F 3 "" H 5200 2300 50  0001 C CNN
	1    5200 2300
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:Graphic GRAF?
U 1 1 61A7D4BD
P 2800 6250
AR Path="/61A7D4BD" Ref="GRAF?"  Part="1" 
AR Path="/61124C5F/61A7D4BD" Ref="GRAF2"  Part="1" 
F 0 "GRAF2" H 3078 6296 50  0000 L CNN
F 1 "Holes" H 3078 6205 50  0000 L CNN
F 2 "gearseq_panel:gearseq_panel_holes" H 2800 6250 50  0001 C CNN
F 3 "" H 2800 6250 50  0001 C CNN
F 4 "DNF" H 2800 6250 50  0001 C CNN "Config"
	1    2800 6250
	1    0    0    -1  
$EndComp
Text Notes 2100 4900 0    50   ~ 0
100k pots for high input\nimpedance — normally for\nArduino one would limit to\n10k but the resolution \nrequired here is very low.
$Comp
L ao_symbols:SW_SPST SW3
U 1 1 618C1D86
P 8850 5000
F 0 "SW3" H 8850 5235 50  0000 C CNN
F 1 "SW_SPST" H 8850 5144 50  0000 C CNN
F 2 "ao_tht:SPDT-toggle-switch-1M-series" H 8850 5000 50  0001 C CNN
F 3 "~" H 8850 5000 50  0001 C CNN
F 4 "Tayda" H 8850 5000 50  0001 C CNN "Vendor"
F 5 "A-3186 (SPDT, use as SPST)" H 8850 5000 50  0001 C CNN "SKU"
	1    8850 5000
	1    0    0    -1  
$EndComp
$Comp
L ao_symbols:2_pin_Molex_connector J18
U 1 1 618C2CDC
P 8900 5450
F 0 "J18" V 9050 5400 50  0000 L CNN
F 1 "2_pin_Molex_connector" V 9150 5000 50  0000 L CNN
F 2 "ao_tht:Molex_KK-254_AE-6410-02A_1x02_P2.54mm_Vertical" H 8900 5450 50  0001 C CNN
F 3 "" H 8900 5450 50  0001 C CNN
F 4 "Tayda" H 8900 5450 50  0001 C CNN "Vendor"
F 5 "A-826" H 8900 5450 50  0001 C CNN "SKU"
	1    8900 5450
	0    1    1    0   
$EndComp
Wire Wire Line
	8650 5000 8600 5000
Wire Wire Line
	8600 5000 8600 5250
Wire Wire Line
	8600 5250 8800 5250
Wire Wire Line
	9050 5000 9100 5000
Wire Wire Line
	9100 5000 9100 5250
Wire Wire Line
	9100 5250 8900 5250
$EndSCHEMATC
