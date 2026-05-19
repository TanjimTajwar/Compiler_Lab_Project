# Run tests one by one (Windows / PowerShell)

Build once:

```powershell
cd "C:\Users\MrfAr\OneDrive - University of Chittagong\Semester 7\MiniCompiler"
build.bat
```

Then run **any single test** using the commands below (copy one block at a time).

---

## Test 1 — arithmetic

```powershell
.\minicompiler.exe testcases\in01_arith.ml
type output.tac
type output.asm
```

## Test 2 — assignment / subtraction

```powershell
.\minicompiler.exe testcases\in02_assign.ml
type output.tac
type output.asm
```

## Test 3 — if-else

```powershell
.\minicompiler.exe testcases\in03_if_else.ml
type output.tac
type output.asm
```

## Test 4 — while loop

```powershell
.\minicompiler.exe testcases\in04_while.ml
type output.tac
type output.asm
```

## Test 5 — bool literals

```powershell
.\minicompiler.exe testcases\in05_bool.ml
type output.tac
type output.asm
```

## Test 6 — relational / equality

```powershell
.\minicompiler.exe testcases\in06_rel.ml
type output.tac
type output.asm
```

## Test 7 — block scope

```powershell
.\minicompiler.exe testcases\in07_block_scope.ml
type output.tac
type output.asm
```

## Test 8 — nested if

```powershell
.\minicompiler.exe testcases\in08_nested_if.ml
type output.tac
type output.asm
```

## Test 9 — logical operators

```powershell
.\minicompiler.exe testcases\in09_logic.ml
type output.tac
type output.asm
```

## Test 10 — full program (while + if)

```powershell
.\minicompiler.exe testcases\in10_full.ml
type output.tac
type output.asm
```

---

## Optional shortcuts

```powershell
run_test.bat 1
run_test.bat 3
run_test.bat testcases\in10_full.ml
```

Debug mode (shows AST):

```powershell
.\minicompiler.exe -d testcases\in01_arith.ml
```

Save a copy under `outputs\` (after a successful run):

```powershell
copy output.tac outputs\my_run.tac
copy output.asm outputs\my_run.asm
```
