.orig x3000
LEA R2 num ;3002
LDW R2 R2 #-5 ;3004
LEA R1 load ;3006
LEA R2 load ;3008
LDB R1 R2 #-1 ;300a
LDB R1 R2 #5 ;300c
LDW R1 R2 #-1 ;301e
LDW R1 R2 #5 ;3010
HALT ;3012 ;3012
num .fill 0xFFFF ;3014
.fill 0x0000 ;3016
.fill 0x0000 ;3018
.fill 0x0000 ;301a
.fill 0x5678 ;3012
.fill 0x1234 ;301c
load .fill 0xFFFF ;301e
.fill 0x1289 ;3020
.fill 0x1370 ;3022
.fill 0x1469 ;3024
.fill 0x5634 ;3026
.fill 0x5533 ;2028
.fill 0x0000 ;302a
.end 

