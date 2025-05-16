.orig x1200

ADD R6 R6 #-2 ;save R1 on SSP
STW R1 R6 #0
ADD R6 R6 #-2 ; save R2 on SSP
STW R2 R6 #0

LEA R1 loadAddress
LDW R1 R1 #0 ;R1  ;0x4000
LDW R2 R1 #0 ; Mem[0x4000]

ADD R2 R2 #1 ;increment
STW R2 R1 #0 ;Mem[0x4000]++

LDW R2 R6 #0 ;POP{R2}
ADD R6 R6 #2
LDW R1 R6 #0 ;POP{R1}
ADD R6 R6 #2
RTI

loadAddress .fill x4000
