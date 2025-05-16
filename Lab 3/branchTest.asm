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
0x12AA
0x54A0
0x0401
0x0000
0x14A1
0x0201
0x0000
0x54A0
0x14BF
0x09F7
0x12A4