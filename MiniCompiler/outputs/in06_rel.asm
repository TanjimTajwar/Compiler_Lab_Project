; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 5
    LI r1, 5
    SEQ r2, r0, r1
    MOV r3, r2
    SNE r4, r0, r1
    MOV r5, r4
    BEQZ r3, L0
    PARAM r0
    CALL print
L0:
    HALT
