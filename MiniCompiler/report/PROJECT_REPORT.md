# MiniLang Compiler

A complete compiler for **MiniLang**, a small statically typed imperative language, implemented using **Flex**, **Bison**, and **C**.

The compiler performs:

* Lexical Analysis
* Syntax Analysis (LALR(1))
* AST Construction
* Semantic Analysis
* Three-Address Code (TAC) Generation
* TAC Optimization
* Pseudo-Assembly Generation

### Outputs

* `output.tac` — Optimized intermediate code
* `output.asm` — Pseudo assembly code

---

## Team Members

| Name                    | ID       |
| ----------------------- | -------- |
| Tanjim Tajwar Arnab     | 22701066 |
| Hafiz Hasnat Sifat Jami | 22701068 |
| Muznabin Ahmed          | 22701069 |
| Monir Hossain           | 21701009 |

---

## Project Features

### Supported Language Constructs

* `int`, `bool`
* Variable declarations
* Assignment statements
* Arithmetic expressions (`+`, `-`, `*`, `/`)
* Relational expressions (`<`, `>`, `<=`, `>=`, `==`, `!=`)
* Unary operators (`-`, `!`)
* `if`
* `if-else`
* `while`
* `print()`
* Nested block scopes `{ }`

### Not Supported

* Arrays
* Functions
* Pointers
* Floating-point values
* Strings

---

## Compiler Pipeline

```text
MiniLang Source
       |
       v
Lexical Analysis (Flex)
       |
       v
Syntax Analysis (Bison)
       |
       v
Abstract Syntax Tree
       |
       v
Semantic Analysis
       |
       v
Three-Address Code
       |
       v
Optimization
       |
       v
Pseudo Assembly
```

---

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
├── optimizer.h
├── optimizer.c
├── targetcode.h
├── targetcode.c
├── errors.h
├── errors.c
├── main.c
├── Makefile
├── testcases/
└── report/
```

---

## Build Requirements

The following tools are required:

```text
gcc
flex
bison
make
```

### Verify Installation

```bash
gcc --version
flex --version
bison --version
make --version
```

---

## Compilation

Build the compiler using:

```bash
make
```

Expected output:

```text
Generating parser...
Generating scanner...
Compiling MiniLang compiler...
Build successful.
```

---

## Running the Compiler

To compile a MiniLang program:

```bash
./minicompiler testcases/test1.ml
```

The compiler generates:

```text
output.tac
output.asm
```

To view the generated files:

```bash
cat output.tac
cat output.asm
```

---

## Example

### Input

```c
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

### Generated TAC

```text
a = 2
b = 3
c = 4
t0 = b * c
t1 = a + t0
x = t1
print x
```

### Generated Assembly

```text
LOAD b
MUL c
STORE t0
LOAD a
ADD t0
STORE t1
LOAD t1
STORE x
PRINT
HALT
```

---

## Error Handling

The compiler detects and reports different types of errors.

### Lexical Errors

```text
Lexical error at line 5:
Invalid character '@'
```

### Syntax Errors

```text
Syntax error at line 8:
Expected ';'
```

### Semantic Errors

```text
Semantic error:
Undeclared variable 'x'
```

```text
Semantic error:
Type mismatch in assignment
```

---

## Testing

Run all automated tests using:

```bash
make test
```

To run an individual test:

```bash
./minicompiler testcases/test1.ml
```

### Available Test Files

```text
test1.ml
test2.ml
test3.ml
test_arithmetic.ml
test_scope.ml
test_if.ml
test_while.ml
error_lexical.ml
error_syntax.ml
error_undeclared.ml
error_duplicate.ml
```

---

## Optimization Passes

The compiler currently implements:

* Constant Folding
* Algebraic Simplification
* Redundant Temporary Elimination

### Planned Optimizations

* Dead Code Elimination
* Copy Propagation
* Common Subexpression Elimination

---

## Complexity Overview

| Phase                  | Time Complexity    |
| ---------------------- | ------------------ |
| Lexical Analysis       | `O(n)`             |
| Parsing                | `O(t)`             |
| AST Construction       | `O(e)`             |
| Semantic Analysis      | `O(e)`             |
| TAC Generation         | `O(e)`             |
| Optimization           | `O(i²)` worst case |
| Target Code Generation | `O(i)`             |

Where:

* `n` = source characters
* `t` = tokens
* `e` = AST nodes
* `i` = TAC instructions

---

## Documentation

Detailed implementation information is available in:

```text
report/PROJECT_REPORT.md
```

The report covers:

* Language Design
* Grammar Specification
* AST Design
* Symbol Table Implementation
* Semantic Analysis
* TAC Generation
* Optimization
* Target Code Generation
* Complexity Analysis
* Test Results

---

## Future Work

Possible future improvements include:

* Hash-based Symbol Table
* Dead Code Elimination
* Copy Propagation
* Function Support
* Array Support
* LLVM IR Backend
* MIPS/x86 Code Generation

---

## Authors

**Group 13**

Department of Computer Science & Engineering
University of Chittagong
Compiler Laboratory Project (CSE 712)
