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

0x3000
0xE608 ;3000
0x66C0 ;3002
0xC0C0 ;3004
0x4801 ;3006
0x4FFE ;3008
0xE802 ;300a
0x4100 ;300c
0x1261 ;300e
0xF025 ;3010
0x3008 ;3012