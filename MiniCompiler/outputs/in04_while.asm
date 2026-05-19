; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 0
L0:
    LI r3, 3
    SLT r1, r0, r3
    BEQZ r1, L1
    PARAM r0
    CALL print
    LI r6, 1
    ADD r4, r0, r6
    MOV r0, r4
    JMP L0
L1:
    HALT
