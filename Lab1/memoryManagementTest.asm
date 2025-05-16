.orig x3000



LEA R1, store
LEA R1 #2
LDW R5, R2, #10
 
LDB R3 R3, #10

LDB R4 R5 #10
STW R1 R2 #-31
STW R1 R2 #31
STB R1 R5 #-32
STB R1 R5 #10




load .fill x3000 
store .fill x0000
backstore .fill x2000

.end