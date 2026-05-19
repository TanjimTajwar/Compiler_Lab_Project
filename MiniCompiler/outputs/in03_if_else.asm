; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 7
    LI r1, 3
    SGT r2, r0, r1
    MOV r3, r2
    BEQZ r3, L0
    PARAM r0
    CALL print
    JMP L1
L0:
    PARAM r1
    CALL print
L1:
    HALT
