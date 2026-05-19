; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 100
    LI r1, 35
    SUB r2, r0, r1
    PARAM r2
    CALL print
    HALT
