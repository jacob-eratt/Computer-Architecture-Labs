.orig x3000 ;does not include add since already tested in example 0
ADD R0 R0 #5
AND R1 R1 #0
AND R2 R2 R2
XOR R3 R3 R3
XOR R4 R4 #1
ADD R5 R5 #1
nop
nop
nop
nop
nop
nop
LSHF R6 R5 #15
nop
nop
nop
nop
nop
RSHFA R0 R6 #10
RSHLF R1 R6 #10
HALT
.end