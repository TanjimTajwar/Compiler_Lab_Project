Build once
cd "C:\Users\MrfAr\OneDrive - University of Chittagong\Semester 7\MiniCompiler"
build.bat
Run each test separately (copy one block)
Test 1

.\minicompiler.exe testcases\in01_arith.ml
type output.tac
type output.asm
Test 2

.\minicompiler.exe testcases\in02_assign.ml
type output.tac
type output.asm
Test 3

.\minicompiler.exe testcases\in03_if_else.ml
type output.tac
type output.asm
Test 4

.\minicompiler.exe testcases\in04_while.ml
type output.tac
type output.asm
Test 5

.\minicompiler.exe testcases\in05_bool.ml
type output.tac
type output.asm
Test 6

.\minicompiler.exe testcases\in06_rel.ml
type output.tac
type output.asm
Test 7

.\minicompiler.exe testcases\in07_block_scope.ml
type output.tac
type output.asm
Test 8

.\minicompiler.exe testcases\in08_nested_if.ml
type output.tac
type output.asm
Test 9

.\minicompiler.exe testcases\in09_logic.ml
type output.tac
type output.asm
Test 10

.\minicompiler.exe testcases\in10_full.ml
type output.tac
type output.asm
Shortcut (still one test only)
run_test.bat 1
run_test.bat 7
run_all_tests.bat no longer runs all 10 together. Full list is in RUN_TESTS.md.