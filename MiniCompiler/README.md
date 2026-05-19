# MiniLang Compiler

A complete multi-phase compiler for **MiniLang**, a small C-like language, implemented in **C** with **Flex** (lexical analysis) and **Bison** (syntax analysis).

## Project Overview

This project implements all major compiler phases:

1. **Lexical Analysis** — tokenization with line numbers  
2. **Syntax Analysis** — LALR parsing into an AST  
3. **Semantic Analysis** — scope-based symbol table and type checking  
4. **Intermediate Code** — Three-Address Code (TAC)  
5. **Optimization** — constant folding and dead code elimination  
6. **Code Generation** — register-style pseudo assembly  

Outputs:

- `output.tac` — optimized three-address code  
- `output.asm` — pseudo assembly  

## Language Design Decisions

| Feature | Decision |
|--------|----------|
| Types | `int`, `bool` only (no float, arrays, pointers, functions) |
| Statements | declaration, assignment, `if-else`, `while`, `print()`, blocks |
| Expressions | `+ - * /`, `< > == !=`, `&& \|\|`, `!`, unary `-` |
| Literals | integer literals, `true` / `false` |
| Scoping | block `{ }` opens a new scope (stack-based symbol table) |
| Comments | `//` line and `/* */` block comments |

## Compiler Phases

### 1. Lexical Analysis (`lexer.l`)
Scans source into tokens. Tracks `yylineno` for error reporting. Reports invalid characters with suggestions.

### 2. Syntax Analysis (`parser.y`)
Bison grammar builds the AST. Uses `%locations` for line-attached nodes. Verbose parse errors.

### 3. AST (`ast.h`, `ast.c`)
Unified node types for statements and expressions. Pretty-print in debug mode. TAC list structures shared across phases.

### 4. Semantic Analysis (`semantic.c` + `symbol_table.c`)
- Inserts declarations into the current scope  
- Resolves identifiers up the scope stack  
- Checks arithmetic (int-only), relational (same type), logical (bool-only)  
- Detailed errors with line numbers and suggestions  

### 5. Code Generation (`codegen.c`)
Walks AST and emits TAC: temporaries `t0…`, labels `L0…`, control flow for `if`/`while`, `print` as `param`/`call`.

### 6. Optimization (`optimizer.c`)
- **Constant folding** — evaluates ops on numeric literals at compile time  
- **Dead code elimination** — removes unused temporary assignments  

### 7. Target Code (`target_code.c`)
Maps TAC to pseudo assembly (`LI`, `LOAD`, `ADD`, `BEQZ`, `JMP`, `CALL print`, etc.) with virtual registers `r0`, `r1`, …

## Team Contributions

| Member | ID | Role | Files |
|--------|-----|------|-------|
| **Tanjim Tajwar Arnab** | 22701066 | **40%** — AST, semantic analysis, TAC, integration | `ast.h`, `ast.c`, `semantic.c`, `codegen.c`, `main.c`, `Makefile` |
| Sabrina Sultana | 22701067 | Lexical analysis | `lexer.l` |
| Hafiz Hasnat Sifat Jami | 22701068 | Syntax analysis | `parser.y` |
| Muznabin Ahmed | 22701069 | Symbol table | `symbol_table.c` |
| Monir Hossain | 21701009 | Optimization & target code | `optimizer.c`, `target_code.c` |

## How to Compile and Run

> Flex and Bison are not built into Windows. Install them once via **MSYS2** below. Linux steps are at the end (for lab PCs or WSL).

### Windows (your PC)

#### 1. One-time setup — MSYS2

1. Install **MSYS2** from https://www.msys2.org/ (default folder: `C:\msys64`).
2. Open **MSYS2 UCRT64** from the Start menu.
3. Run:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-flex mingw-w64-ucrt-x86_64-bison make
```

#### 2. Build

In **PowerShell**, inside the project folder (use `build.bat` — do **not** run only `gcc lex.yy.c parser.tab.c`):

```bat
build.bat
```

`build.bat` uses `win_bison`, `win_flex`, and `gcc`, and links **all** `.c` files.

Or in **MSYS2 UCRT64**:

```bash
cd "/c/Users/MrfAr/OneDrive - University of Chittagong/Semester 7/MiniCompiler"
make
```

#### 3. Run

```bat
run.bat
run.bat testcases\complex.ml
minicompiler.exe -d testcases\test1.ml
```

Creates `output.tac` and `output.asm` in the project folder.

**Run tests one at a time** (do not batch-run all): open **[RUN_TESTS.md](RUN_TESTS.md)** and copy each command block, or use:

```bat
run_test.bat 1
run_test.bat 4
```

| Problem | Fix |
|--------|-----|
| `'flex' is not recognized` | Use `build.bat`, or build inside **MSYS2 UCRT64** |
| `gcc not found` in `build.bat` | Edit `MSYS2_BIN` at the top of `build.bat` |

### Linux (lab / WSL)

```bash
sudo apt install flex bison gcc make
make
./minicompiler testcases/test1.ml
make clean
```

## Example Input / Output

**Input** (`testcases/test1.ml`):
```c
int a;
int b;
a = 10;
b = 20;
print(a + b);
```

**TAC** (`output.tac` — excerpt):
```
a = 10
b = 20
t0 = + a, b
param t0
call print, 1
halt
```

**Assembly** (`output.asm` — excerpt):
```asm
    LI r0, 10
    MOV a, r0
    ...
    CALL print
    HALT
```

## Complexity Analysis (Big-O)

| Phase | Time | Space |
|-------|------|-------|
| Lexical analysis | O(n) | O(1) |
| Parsing + AST build | O(n) | O(n) |
| Semantic analysis | O(n · d) worst case* | O(s) symbols |
| TAC generation | O(n) | O(n) instructions |
| Constant folding | O(n) | O(1) |
| Dead code elimination | O(n · m)† | O(1) |
| Target code generation | O(n) | O(t) registers |

\* *n* = AST nodes, *d* = scope depth, *s* = total symbols  
† *m* = average remaining instructions per def  

## Error Handling

- **Lexical** — unknown characters (line + suggestion)  
- **Syntax** — Bison verbose errors (line + suggestion)  
- **Semantic** — undeclared vars, redeclaration, type mismatch (line + suggestion)  

## Project Structure

```
MiniCompiler/
├── lexer.l
├── parser.y
├── ast.h / ast.c
├── symbol_table.c
├── semantic.c
├── codegen.c
├── optimizer.c
├── target_code.c
├── main.c
├── Makefile
├── README.md
└── testcases/
    ├── test1.ml
    ├── test2.ml
    └── complex.ml
```

## License

Academic project — University of Chittagong, Semester 7.
