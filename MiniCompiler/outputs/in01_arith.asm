; Pseudo Assembly (MiniLang)
.text
main:
    LI r0, 10
    LI r1, 20
    ADD r2, r0, r1
    PARAM r2
    CALL print
    HALT
