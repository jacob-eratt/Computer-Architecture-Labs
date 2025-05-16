.orig x3000
AND R0 R0 #0 ;operand 1
AND R1 R1 #0 ;operand 2
AND R2 R2 #0 ;result
AND R3 R3 #0
AND R4 R4 #0 ;used for bitmask
AND R5 R5 #0 ;counter
AND R6 R6 #0 ;addresses
AND R7 R7 #0 ;addresses

LEA R6 operands
LDW R6 R6 #0
LDB R0 R6 #0 ;R0 is operand1
LDB R1 R6 #1 ;R1 is operand2

ADD R5 R5 #8 ;counter

Loop1   BRz finish 
        AND R4 R1 #1 
        Brz Shift
        ADD R2 R2 R0 
Shift   RSHFL R1 R1 #1 
        LSHF R0 R0 #1 
        ADD R5 R5 #-1 
        BRnzp Loop1

finish RSHFL R3 R2 #8 
        BRz Store 
        AND R3 R3 #0 
        ADD R3 R3 #1 

Store LEA R6 result 
        LDW R6 R6 #0
        STB R2 R6 #0
        STB R3 R6 #1

HALT

operands .fill x3100
result .fill x3102

.end

0x3000
0x5020
0x5260
0x54A0
0x56E0
0x5920
0x5B60
0x5DA0
0x5FE0
0xEC15
0x6D80
0x2180
0x2381
0x1B68
0x0407
0x5861
0x0401
0x1480
0xD251
0xD001
0x1B7F
0x0FF8
0xD698
0x0402
0x56E0
0x16E1
0xEC05
0x6D80
0x3580
0x3781
0xF025
0x3100
0x3102
