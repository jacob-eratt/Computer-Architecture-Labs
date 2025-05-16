; Test Case for LSHF, RSHFL, and RSHFA instructions

.ORIG x3000          ; Start of the program
LEA R2 test1
And R0 R0 #0
LDW R2 R2 #0
LSHF R0 R2 #1
RSHFL R0 R2 #1
RSHFA R0 R2 #1

AND R2 R2 #0
ADD R2 R2 #1
LSHF R2 R2 #15
RSHFA R2 R2 #5
RSHFL R2 R2 #5

LEA R2 max
LDw R2 R2 #0
AND R0 R0 #0
LSHF R1, R2, #1      ; Logical shift left R2 by 3, store in R1
RSHFL R3, R4, #15     ; Logical shift right R4 by 2, store in R3
LSHF R1 R2 #10
RSHFA R1, R2,#11     ; Arithmetic shift right R6 by 1, store in R5

HALT 

max .fill 0xFFFF
test1 .fill 0x0030

.END                 


0x3000
0xE413
0x5020
0x6480
0xD081
0xD091
0xD0B1
0x54A0
0x14A1
0xD48F
0xD4B5
0xD495
0xE407
0x6480
0x5020
0xD281
0xD71F
0xD28A
0xD2BB
0xF025
0xFFFF
0x0030