# Expected Output for test1.ml

## Source Program
```
int a;
int b;
int c;
int x;

a = 2;
b = 3;
c = 4;
x = a + b * c;
print(x);
```

## Expected TAC (after optimization)
```
a = 2
b = 3
c = 4
t0 = b * c
t1 = a + t0
x = t1
print x
```

## Expected Pseudo Assembly (excerpt)
```
LOAD_IMM 2
STORE a
LOAD_IMM 3
STORE b
LOAD_IMM 4
STORE c
LOAD b
MUL c
STORE t0
LOAD a
ADD t0
STORE t1
LOAD t1
STORE x
LOAD x
PRINT
HALT
```

## Compiler Console Output
- Semantic Analysis: PASSED
- Compilation successful
- Produces output.tac and output.asm
