; MiniLang Pseudo Assembly
; Machine-independent target code

LOAD_IMM 0
STORE arnab_counter
LOAD_IMM 0
STORE jami_sum
LOAD_IMM 5
STORE monir_limit
JMP Lcond1
Lbody0:
LOAD jami_sum
ADD arnab_counter
STORE t0
LOAD t0
STORE jami_sum
LOAD arnab_counter
ADD 1
STORE t1
LOAD t1
STORE arnab_counter
Lcond1:
LOAD arnab_counter
LT monir_limit
STORE t2
LOAD t2
JNZ Lbody0
LOAD jami_sum
GT 5
STORE t3
LOAD t3
STORE muznabin_condition_1
LOAD jami_sum
EQ 10
STORE t4
LOAD t4
STORE jami_relation_1
LOAD muznabin_condition_1
JZ Lelse2
LOAD_IMM 100
STORE arnab_inner_scope
LOAD arnab_inner_scope
PRINT
LOAD jami_sum
PRINT
JMP Lend3
Lelse2:
LOAD 0
PRINT
Lend3:
LOAD arnab_counter
PRINT
LOAD jami_sum
PRINT

HALT
