.orig x4000
behind ADD r1, r2, #10

Loop AND R2 R2 #0
Brz next
nop
next ADD R2 R2 #1
Brp next2
nop
next2 AND R2 R2 #0
ADD R2 R2 #-1
BRn Loop


Label1 ADD R1, R2, #4
.end

0x4000
0x12AA ;4000
0x54A0 ;4002
0x0401 ;4004
0x0000 ;4006
0x14A1 ;4008
0x0201 ;400a
0x0000 ;400c
0x54A0 ;400e
0x14BF ;4010
0x09F7 ;4012 ;real end to test case
0x12A4 ;4014
