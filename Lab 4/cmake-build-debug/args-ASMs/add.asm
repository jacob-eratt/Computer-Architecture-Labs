.orig x3000
AND R0, R0, #0
AND R1, R0, #0
AND R2, R0, #0
AND R3, R0, #0

LEA R0, inc ;50
LDW R0, R0, #0 ;67
ADD R1, R1, #1 ;n
STW R1, R0, #0 ; store 1 into x4000 
AND R1, R1, #0 ;3010

LEA R0, vector ;
LDW R0, R0, #0

ADD R1, R1, #15
ADD R1 R1 #5

LOOP BRnz DONE   
LDB R2, R0, #0
ADD R3, R3, R2
ADD R0, R0, #1 ;3020
ADD R1, R1, #-1
BRnzp LOOP

DONE LEA, R0, store
LDW R0, R0, #0
STW R3, R0, #0

AND R0 R0 #0
STW R3, R0, #0

inc .fill x4000 ;3030
vector .fill xC000
store .fill xC014
testProtection .fill x0000
testUA .fill xC017
.end
