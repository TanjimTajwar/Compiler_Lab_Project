# Pseudo Assembly
MOV CSECU, 10
MOV Tanjim, 0
CMP t1, CSECU, 3 ; >
MOV Sara, t1
IF Sara JMP L1
ADD t2, CSECU, 5
MOV Tanjim, t2
JMP L2
LABEL L1
SUB t3, CSECU, 1
MOV Tanjim, t3
LABEL L2
LABEL L3
CMP t4, Tanjim, 0 ; >
IF t4 JMP L4
PRINT Tanjim
SUB t5, Tanjim, 1
MOV Tanjim, t5
JMP L3
LABEL L4
