.orig x1200

ADD R6 R6 #-2 ;save R1 on SSP
STW R1 R6 #0
ADD R6 R6 #-2 ; save R2 on SSP
STW R2 R6 #0
ADD R6 R6 #-2 ; save R2 on SSP
STW R3 R6 #0

LEA R1 PTBR
LDW R1 R1 #0 ;PTBR

LEA R2 NUM_PTE
LDW R2 R2 #0 ;contains 128


Loop LDW R3 R1 #0
    AND R3 R3 b11110
    STW R3 R1 #0
    ADD R1 R1 #2
    ADD R2 R2 #-1
BRp Loop

LDW R3 R6 #0
ADD R6 R6 #2

LDW R2 R6 #0 ;POP{R2}
ADD R6 R6 #2

LDW R1 R6 #0 ;POP{R1}
ADD R6 R6 #2

RTI


PTBR .fill x1000 ;this is a virtual address but the frames are directly mapped
NUM_PTE .fill x80
.end


