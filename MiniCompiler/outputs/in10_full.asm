; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 0
    LI r1, 0
    LI r2, 0
L0:
    NOT r3, r2
    BEQZ r3, L1
    ADD r4, r1, r0
    MOV r1, r4
    LI r7, 1
    ADD r5, r0, r7
    MOV r0, r5
    LI r10, 5
    SEQ r8, r0, r10
    BEQZ r8, L2
    LI r2, 1
L2:
    JMP L0
L1:
    PARAM r1
    CALL print
    HALT
