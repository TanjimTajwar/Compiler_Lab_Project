# MiniLang Compiler — Project Report

**Course:** Compiler Design  
**Project:** Complete MiniLang Compiler using Flex and Bison  

---

## Team Members

| Name | ID |
|------|-----|
| Tanjim Tajwar Arnab | 22701066 |
| Hafiz Hasnat Sifat Jami | 22701068 |
| Muznabin Ahmed | 22701069 |
| Monir Hossain | 21701009 |

---

## 1. Introduction

This project implements a complete compiler for **MiniLang**, a small imperative programming language, written in C using **Flex** (lexical analyzer generator) and **Bison** (parser generator). The compiler transforms MiniLang source code through successive phases — lexical analysis, syntax analysis, semantic analysis, intermediate code generation, optimization, and target code generation — producing three-address code (TAC) and pseudo-assembly output.

The primary objectives of this project are:

- To demonstrate the full pipeline of a traditional multi-phase compiler
- To apply theoretical concepts from compiler design in a working implementation
- To provide modular, maintainable code with proper error handling at each phase

The compiler accepts a `.ml` source file and produces:

- `output.tac` — optimized three-address intermediate code
- `output.asm` — machine-independent pseudo assembly

---

## 2. Language Design

### 2.1 Supported Features

| Feature | Description |
|---------|-------------|
| `int` | Integer data type |
| `bool` | Boolean data type (0 = false, non-zero = true) |
| Variable declarations | `int x;` or `bool flag;` |
| Assignments | `x = expression;` |
| Arithmetic expressions | `+`, `-`, `*`, `/` with standard precedence |
| Relational expressions | `<`, `>`, `<=`, `>=`, `==`, `!=` |
| Logical NOT | `!expr` on boolean operands |
| Unary minus | `-expr` on integer operands |
| `if` / `if-else` | Conditional control flow |
| `while` | Iterative control flow |
| `print(expr)` | Output statement |
| Block scope | `{ ... }` with nested scopes |

### 2.2 Unsupported Features

- Arrays
- Functions and procedures
- Pointers
- Floating-point types
- String literals (beyond identifiers)

### 2.3 Sample Program

```c
int a;
int b;
int x;

a = 2;
b = 3;
x = a + b * c;
print(x);
```

### 2.4 Operator Precedence (Highest to Lowest)

1. Unary `-`, `!`
2. `*`, `/`
3. `+`, `-`
4. Relational operators (`<`, `>`, `<=`, `>=`, `==`, `!=`)

All binary operators are left-associative except relational operators which are non-associative in practice through grammar structure.

---

## 3. Lexical Analysis

### 3.1 Implementation

The lexical analyzer is implemented in `lexer.l` using Flex. It tokenizes the input stream and tracks line numbers via `%option yylineno`.

### 3.2 Recognized Tokens

**Keywords:** `int`, `bool`, `if`, `else`, `while`, `print`

**Identifiers:** Letter or underscore followed by letters, digits, or underscores

**Integers:** Sequence of digits

**Operators:** `+`, `-`, `*`, `/`, `!`, `<`, `>`, `<=`, `>=`, `==`, `!=`, `=`

**Delimiters:** `;`, `,`, `(`, `)`, `{`, `}`

### 3.3 Ignored Input

- Whitespace (spaces, tabs, newlines)
- Single-line comments: `// ...`
- Multi-line comments: `/* ... */`

### 3.4 Error Handling

Invalid characters trigger a lexical error reported with the current line number:

```
Lexical error at line 5: Invalid character '@'
```

---

## 4. Syntax Analysis

### 4.1 Implementation

The parser is defined in `parser.y` using Bison. It implements an LALR(1) grammar and constructs the Abstract Syntax Tree (AST) during parsing via semantic actions.

### 4.2 Grammar Overview

```
program        → statement_list
statement_list → statement_list statement | ε
statement      → declaration | assignment | if | while | print | block
declaration    → type IDENTIFIER ;
assignment     → IDENTIFIER = expression ;
if_statement   → if ( expression ) statement [ else statement ]
while_statement→ while ( expression ) statement
print_statement→ print ( expression ) ;
block          → { statement_list }
expression     → rel_expr
rel_expr       → add_expr | rel_expr rel_op add_expr
add_expr       → mul_expr | add_expr add_op mul_expr
mul_expr       → unary_expr | mul_expr mul_op unary_expr
unary_expr     → primary | - unary_expr | ! unary_expr
primary        → IDENTIFIER | INTEGER | ( expression )
```

### 4.3 Precedence and Associativity

Declared in Bison:

```yacc
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%right NOT UMINUS
```

This ensures expressions like `a + b * c` parse as `a + (b * c)`.

### 4.4 Syntax Error Handling

Bison's `%define parse.error verbose` enables detailed syntax error messages. The custom `yyerror()` function reports errors with line numbers.

---

## 5. AST Design

### 5.1 Node Types

| Node Type | Purpose |
|-----------|---------|
| `NODE_PROGRAM` | Root of the program |
| `NODE_DECL` | Variable declaration |
| `NODE_ASSIGN` | Assignment statement |
| `NODE_IF` | If / if-else statement |
| `NODE_WHILE` | While loop |
| `NODE_PRINT` | Print statement |
| `NODE_BLOCK` | Block with local scope |
| `NODE_IDENT` | Identifier reference |
| `NODE_NUMBER` | Integer literal |
| `NODE_BINOP` | Binary arithmetic operation |
| `NODE_RELOP` | Relational operation |
| `NODE_UNARY` | Unary operation |

### 5.2 AST Structure

Each AST node stores:

- Node type
- Source line number
- Data type (populated during semantic analysis)
- Type-specific payload in a union

### 5.3 Traversal Functions

- `ast_print(node, indent)` — pretty-prints the AST for debugging
- `ast_free(node)` — recursively frees the entire tree

---

## 6. Symbol Table

### 6.1 Design

The symbol table (`symbol_table.c`) uses a **stack of scopes** implemented as a linked list of `Scope` structures. Each scope contains a linked list of `Symbol` entries.

### 6.2 Operations

| Function | Description |
|----------|-------------|
| `symtab_create()` | Create table with global scope |
| `symtab_enter_scope()` | Push new scope (on block entry) |
| `symtab_exit_scope()` | Pop current scope (on block exit) |
| `symtab_insert()` | Insert symbol in current scope |
| `symtab_lookup()` | Search from innermost to outermost scope |
| `symtab_lookup_current()` | Search only current scope |

### 6.3 Scope Management

- Global scope created at program start
- New scope entered on `{` block entry
- Scope exited on `}` block exit
- Inner scopes can shadow outer declarations

---

## 7. Semantic Analysis

### 7.1 Checks Performed

1. **Duplicate declaration detection** — variable declared twice in same scope
2. **Undeclared variable detection** — use of identifier not in any active scope
3. **Type mismatch checking** — assignment and operand type compatibility
4. **Invalid conditional expressions** — `if`/`while` conditions must be `bool`

### 7.2 Type Rules

| Context | Rule |
|---------|------|
| Arithmetic operands | Both must be `int` |
| Relational operands | Both must be `int`; result is `bool` |
| Assignment | Right-hand type must match left-hand variable type |
| Condition | Must be `bool` |
| `!` operand | Must be `bool` |
| Unary `-` operand | Must be `int` |

### 7.3 Error Reporting

```
Semantic error at line 8: Undeclared variable 'z'
Semantic error at line 12: Type mismatch in assignment to 'flag': expected bool, got int
```

Compilation aborts if any semantic errors are detected.

---

## 8. TAC Generation

### 8.1 Three-Address Code Format

Each TAC instruction has at most one operator and three operands:

```
result = arg1 op arg2
```

### 8.2 Example

**Input:** `x = a + b * c;`

**Output:**
```
t0 = b * c
t1 = a + t0
x = t1
```

### 8.3 Control Flow TAC

**If-else:**
```
ifFalse cond goto Lelse
... then branch ...
goto Lend
Lelse:
... else branch ...
Lend:
```

**While loop:**
```
goto Lcond
Lbody:
... body ...
Lcond:
t = condition
ifTrue t goto Lbody
```

### 8.4 Temporaries and Labels

- Temporaries: `t0`, `t1`, `t2`, ...
- Labels: `Lbody0`, `Lcond0`, `Lelse0`, `Lend0`, ...

---

## 9. Optimization

Three optimization passes are implemented in `optimizer.c`:

### 9.1 Constant Folding

**Before:** `t1 = 2 + 3`  
**After:** `t1 = 5`

Evaluates binary operations when both operands are numeric constants.

### 9.2 Algebraic Simplification

| Pattern | Simplified To |
|---------|---------------|
| `x + 0` | `x` |
| `0 + x` | `x` |
| `x * 1` | `x` |
| `1 * x` | `x` |
| `x * 0` | `0` |
| `x - 0` | `x` |
| `x / 1` | `x` |

### 9.3 Redundant Temporary Removal

If:
```
t1 = a
x = t1
```

Then all uses of `t1` are replaced with `a`, and the temporary assignment is removed.

The optimizer runs multiple passes until no further changes occur.

---

## 10. Target Code Generation

### 10.1 Pseudo Assembly Instructions

| TAC | Assembly |
|-----|----------|
| `x = y` | `LOAD y` / `STORE x` |
| `x = imm` | `LOAD_IMM imm` / `STORE x` |
| `x = y + z` | `LOAD y` / `ADD z` / `STORE x` |
| `x = y * z` | `LOAD y` / `MUL z` / `STORE x` |
| `ifFalse x goto L` | `LOAD x` / `JZ L` |
| `ifTrue x goto L` | `LOAD x` / `JNZ L` |
| `goto L` | `JMP L` |
| `print x` | `LOAD x` / `PRINT` |
| `label L` | `L:` |

### 10.2 Example

**TAC:**
```
t0 = b * c
t1 = a + t0
x = t1
```

**Assembly:**
```
LOAD b
MUL c
STORE t0
LOAD a
ADD t0
STORE t1
LOAD t1
STORE x
HALT
```

This is machine-independent pseudo assembly suitable for a hypothetical stack or accumulator machine.

---

## 11. Complexity Analysis

Let **n** = source file length in characters, **t** = number of tokens, **s** = number of statements, **e** = number of AST nodes, **i** = number of TAC instructions.

### 11.1 Scanner Complexity

| Aspect | Complexity |
|--------|------------|
| Time | **O(n)** — each character examined once |
| Space | **O(1)** — constant auxiliary space (excluding token strings) |

Flex generates a DFA; each character transition is O(1).

### 11.2 Parser Complexity

| Aspect | Complexity |
|--------|------------|
| Time | **O(t)** — each token processed once by LALR parser |
| Space | **O(t)** — parser stack depth proportional to input |

### 11.3 AST Complexity

| Aspect | Complexity |
|--------|------------|
| Construction time | **O(e)** — one node per grammar construct |
| Space | **O(e)** — one node per AST element |
| Traversal time | **O(e)** — visit each node once |

### 11.4 Symbol Table Complexity

| Operation | Complexity |
|-----------|------------|
| Insert | **O(1)** — hash/list insert at scope head |
| Lookup | **O(d × m)** — d = scope depth, m = symbols per scope |
| Enter/exit scope | **O(1)** |

For typical programs with shallow nesting, lookup is effectively O(m).

### 11.5 Semantic Analysis Complexity

| Aspect | Complexity |
|--------|------------|
| Time | **O(e)** — one pass over AST |
| Space | **O(s)** — symbol table size |

### 11.6 TAC Generation Complexity

| Aspect | Complexity |
|--------|------------|
| Time | **O(e)** — each expression node emits constant number of instructions |
| Space | **O(i)** — TAC list grows linearly with expressions |

### 11.7 Optimization Complexity

| Pass | Complexity |
|------|------------|
| Constant folding | **O(i)** per pass |
| Algebraic simplification | **O(i)** per pass |
| Redundant temp removal | **O(i²)** worst case (propagation scans all instructions) |

With a fixed number of passes (3), overall optimization is **O(i²)** worst case, **O(i)** typical.

### 11.8 Target Code Generation Complexity

| Aspect | Complexity |
|--------|------------|
| Time | **O(i)** — one assembly instruction group per TAC instruction |
| Space | **O(1)** — streaming output to file |

### 11.9 Overall Compiler Complexity

| Phase | Time | Space |
|-------|------|-------|
| Lexical Analysis | O(n) | O(1) |
| Syntax Analysis | O(t) | O(t) |
| AST Construction | O(e) | O(e) |
| Semantic Analysis | O(e) | O(s) |
| TAC Generation | O(e) | O(i) |
| Optimization | O(i²) | O(i) |
| Target Code Gen | O(i) | O(1) |

**Overall Time: O(n + i²)** — dominated by scanning and optimization  
**Overall Space: O(e + i + s)** — AST, TAC, and symbol table storage

---

## 12. Testing

### 12.1 Test Cases

| File | Purpose |
|------|---------|
| `test1.ml` | Basic arithmetic, precedence, print |
| `test2.ml` | While loop, if-else control flow |
| `test3.ml` | Boolean type, relational expressions |

### 12.2 Test 1 — Arithmetic Expression

**Input (`test1.ml`):**
```c
int a; int b; int c; int x;
a = 2; b = 3; c = 4;
x = a + b * c;
print(x);
```

**Expected TAC:**
```
a = 2
b = 3
c = 4
t0 = b * c
t1 = a + t0
x = t1
print x
```

**Result:** x = 2 + 3*4 = 14

### 12.3 Test 2 — Control Flow

**Input (`test2.ml`):** While loop summing 0..4, then if-else on sum > 10.

**Expected:** sum = 10, condition false, prints 0.

### 12.4 Test 3 — Boolean and Relational

**Input (`test3.ml`):** flag = x > y where x=10, y=5.

**Expected:** flag is true, prints 10.

### 12.5 Running Tests

```bash
make
make test
```

Or individually:
```bash
./minicompiler testcases/test1.ml
```

### 12.6 Error Test Cases (Manual)

- Undeclared variable → semantic error
- Duplicate declaration in same block → semantic error
- `if (x + 1)` where condition is int → semantic error
- Invalid character `@` → lexical error
- Missing semicolon → syntax error

---

## 13. Conclusion

This project successfully implements a complete multi-phase compiler for the MiniLang programming language. All major compiler phases — lexical analysis, parsing, AST construction, semantic analysis, intermediate code generation, optimization, and target code generation — are fully functional and integrated.

### Key Achievements

- Modular architecture with separate files for each compiler phase
- Comprehensive error handling at lexical, syntax, and semantic levels
- Working symbol table with nested scope support
- Three-address code generation with control flow support
- Basic but effective optimization passes
- Machine-independent pseudo assembly output

### Future Enhancements

- Symbol table hashing for O(1) average lookup
- Additional optimizations (dead code elimination, copy propagation)
- Support for functions, arrays, and richer type system
- Code generation for a real target (x86, MIPS, or LLVM IR)
- Interactive debugger and verbose compilation flags

The project demonstrates practical application of compiler design theory and provides a solid foundation for extending toward a production-quality compiler.

---

## Appendix: Project File Structure

```
MiniCompiler/
├── lexer.l
├── parser.y
├── ast.h / ast.c
├── symbol_table.h / symbol_table.c
├── errors.h / errors.c
├── semantic.h / semantic.c
├── codegen.h / codegen.c
├── optimizer.h / optimizer.c
├── targetcode.h / targetcode.c
├── main.c
├── Makefile
├── testcases/
│   ├── test1.ml
│   ├── test2.ml
│   ├── test3.ml
│   ├── expected_test1.md
│   ├── expected_test2.md
│   └── expected_test3.md
├── output.tac
├── output.asm
└── report/
    └── PROJECT_REPORT.md
```

## Appendix: Build Instructions

**Requirements:** gcc, flex, bison, make

```bash
cd MiniCompiler
make
./minicompiler testcases/test1.ml
cat output.tac
cat output.asm
make clean
```
