; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 1
    LI r1, 0
    BEQZ r0, L0
    PARAM r2
    CALL print
    JMP L1
L0:
    PARAM r3
    CALL print
L1:
    HALT
