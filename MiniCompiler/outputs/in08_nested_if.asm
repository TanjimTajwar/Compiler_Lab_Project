; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 10
    LI r3, 0
    SGT r1, r0, r3
    MOV r4, r1
    BEQZ r4, L0
    LI r7, 10
    SEQ r5, r0, r7
    BEQZ r5, L2
    PARAM r0
    CALL print
    JMP L3
L2:
    PARAM r2
    CALL print
L3:
    JMP L1
L0:
    LI r8, -1
    PARAM r8
    CALL print
L1:
    HALT
