# MiniLang Compiler - CSE 712 Final Project

University of Chittagong, Department of CSE

## Group Members

- Tanjim Tajwar Arnab (ID: 22701066) 
- Sabrina Sultana (ID: 22701067) 
- Sifat Jami (ID: 22701068) 
- Muznabin Ahmed (ID: 22701069) 
- Monir Hossain (ID: 21701009) 

## Project Overview

This project implements a full MiniLang compiler in C using Flex and Bison.  
The compiler reads a `.ml` source file, checks syntax + semantics, then generates:

- Three Address Code (`output.tac`)
- Pseudo Assembly (`output.asm`)

Implemented phases:

- Lexical Analysis (Flex)
- Syntax Analysis (Bison)
- AST Construction
- Symbol Table + Scope Handling
- Semantic Analysis (type checking + validations)
- Intermediate Code Generation (TAC)
- Basic Optimization (constant folding, simple dead code skip)
- Target Code Generation (pseudo assembly)

This part was mainly implemented by Jami (AST + semantic flow).  
Parser and code generation flow was mainly handled by Tanjim.

## Requirements

Install required tools:

```bash
sudo apt update
sudo apt install flex bison gcc make
```

## Project Structure

```text
MiniCompiler/
├── lexer.l
├── parser.y
├── ast.h
├── ast.c
├── symbol_table.h
├── symbol_table.c
├── semantic.h
├── semantic.c
├── codegen.h
├── codegen.c
├── main.c
├── Makefile
├── README.md
└── testcases/
    ├── test1.ml
    └── test2.ml
```

## Compilation Instructions

### A) Manual Commands

```bash
bison -d parser.y
flex lexer.l
gcc -Wall -Wextra -std=c11 lex.yy.c parser.tab.c ast.c symbol_table.c semantic.c codegen.c main.c -o minicompiler
```

### B) Using Makefile

```bash
make
```

## Run Instructions

```bash
./minicompiler testcases/test1.ml
```

## Input Format

Input is a plain `.ml` source file containing MiniLang statements:

- `int`, `bool` declarations
- assignments
- arithmetic / relational expressions
- `if-else`, `while`
- `print(...)`
- block scopes `{ ... }`

## Output Format

- `output.tac`: line-by-line Three Address Code
- `output.asm`: pseudo assembly instructions (`MOV`, `ADD`, `SUB`, `JMP`, `IF`, `LABEL`, etc.)

## Example

MiniLang input:

```ml
int a;
int b;
bool c;
a = 5;
b = a + 3;
c = b > 4;
if (c) {
    print(b);
}
```

Possible TAC output:

```text
# Three Address Code
a = 5
t1 = a + 3
b = t1
t2 = b > 4
c = t2
ifFalse c goto L1
print b
goto L2
L1:
L2:
```

## Error Handling

Compiler reports three main kinds of errors:

- **Lexical Errors**: invalid characters/tokens from lexer
- **Syntax Errors**: grammar violations from parser
- **Semantic Errors**: undeclared use, redeclaration, type mismatch, invalid condition types

## Features

- Full tokenization with line tracking
- AST generation during parsing
- Scope-based symbol table
- Semantic type checking
- TAC generation with temporary variables and labels
- Basic constant folding optimization
- Simple dead code skip for obvious `if (0)` style cases
- Pseudo assembly generation from AST/TAC flow

## Complexity Summary

```text
Phase                        Approx Time
Lexical scan                 O(n)
Parsing                      O(n)
Semantic analysis            O(n)
Code generation              O(n)
Symbol lookup (worst-case)   O(s) per lookup, s = symbols in visible scopes
```

## Notes / Limitations

- Arrays are not supported
- Functions are not supported
- Pointers are not supported
- Floating point is not supported
- Optimizer is intentionally basic (lab-friendly level)

Monir mostly focused on target-code style and output format cleanup.  
Sabrina mostly focused on lexer behavior and test case coverage.
