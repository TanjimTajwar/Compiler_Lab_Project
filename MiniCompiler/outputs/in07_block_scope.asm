; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 1
    LI r0, 2
    PARAM r0
    CALL print
    PARAM r0
    CALL print
    HALT
