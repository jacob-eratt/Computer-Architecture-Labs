.orig x3000
LEA R2 num ;3000
LDW R2 R2 #-5 ;3002
LEA R1 load ;3004
LEA R2 load ;3005
LDB R1 R2 #-1 ;3008
LDB R1 R2 #1 ;300a
LDW R1 R2 #-1 ;300c
LDW R1 R2 #5 ;300e
HALT ;3012 ;3010
num .fill 0xFFFF ;3012
.fill 0x0000 ;3014
.fill 0x0000 ;3016
.fill 0x0000 ;3018
.fill 0x5678 ;301a
.fill 0x1234 ;301c
load .fill 0xFFFF ;301e ; -2 address 
.fill 0x1289 ;3020
.fill 0x1370 ;3022
.fill 0x1469 ;3024
.fill 0x5634 ;3026
.fill 0x5533 ;2028
.fill 0x0000 ;302a
.end 



0x3000
0xE408
0x64BB
0xE20C
0xE40B
0x22BF
0x2245
0x62BF
0x6285
0xF025
0xFFFF
0x0000
0x0000
0x0000
0x5678
0x1234
0xFFFF
0x1289
0x1370
0x1469
0x5634
0x5533
0x0000
