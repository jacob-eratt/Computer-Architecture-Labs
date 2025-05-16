; Test Case for LSHF, RSHFL, and RSHFA instructions

.ORIG x3000          ; Start of the program
LSHF R1, R2, #3      ; Logical shift left R2 by 3, store in R1
LSHF R1, R2, xF      ; Logical shift left R2 by 3, store in R1
RSHFL R3, R4, #2     ; Logical shift right R4 by 2, store in R3
RSHFA R5, R6, #1     ; Arithmetic shift right R6 by 1, store in R5dos2unix

HALT 


.END                 
