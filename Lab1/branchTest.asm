.orig x4000
behind ADD r1, r2, #10

BR behind
BR Label1

brn behind
brn Label1

brnz Label1
brnz behind 

brnzp Label1
brnzp behind

brnp Label1
brnp behind

brzp Label1
brzp behind
brz Label1 
brz behind

brp Label1
brp behind


Label1 ADD R1, R2, #4
.end