        .ORIG x3000          
        LEA R3 haha  ;3000
        LDW R3 R3 #0 ;3002
        JMP R3       ;3004   
yo      JSR TARGET   ;3006
        JSR yo      ;3008
TARGET  LEA R4 goHalt ;300a
        JSRR R4 ;300c
        ADD R1 R1 #1; ;300e
goHalt HALT     ;30010
haha .fill 0x3008 ;3012

        .END                 