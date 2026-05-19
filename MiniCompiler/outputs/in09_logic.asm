; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 1
    LI r1, 0
    NOT r2, r1
    AND r3, r0, r2
    MOV r4, r3
    OR r5, r4, r1
    BEQZ r5, L0
    PARAM r6
    CALL print
    JMP L1
L0:
    PARAM r7
    CALL print
L1:
    HALT
