; MiniLang Pseudo Assembly
; Machine-independent target code

LOAD_IMM 10
STORE x
LOAD_IMM 5
STORE y
LOAD x
GT y
STORE t0
LOAD t0
STORE flag
LOAD flag
JZ Lelse0
LOAD x
PRINT
JMP Lend1
Lelse0:
LOAD y
PRINT
Lend1:

HALT
