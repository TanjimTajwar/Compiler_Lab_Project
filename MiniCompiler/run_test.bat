@echo off
REM Run ONE test only. Usage:
REM   run_test.bat 1
REM   run_test.bat in03_if_else.ml
setlocal
cd /d "%~dp0"

if "%~1"=="" (
    echo Usage: run_test.bat ^<1-10 or filename^>
    echo.
    echo Examples:
    echo   run_test.bat 1
    echo   run_test.bat testcases\in04_while.ml
    goto :list
)

if not exist minicompiler.exe (
    echo Run build.bat first.
    exit /b 1
)

set "ARG=%~1"
set "FILE="

if "%ARG%"=="1" set "FILE=testcases\in01_arith.ml"
if "%ARG%"=="2" set "FILE=testcases\in02_assign.ml"
if "%ARG%"=="3" set "FILE=testcases\in03_if_else.ml"
if "%ARG%"=="4" set "FILE=testcases\in04_while.ml"
if "%ARG%"=="5" set "FILE=testcases\in05_bool.ml"
if "%ARG%"=="6" set "FILE=testcases\in06_rel.ml"
if "%ARG%"=="7" set "FILE=testcases\in07_block_scope.ml"
if "%ARG%"=="8" set "FILE=testcases\in08_nested_if.ml"
if "%ARG%"=="9" set "FILE=testcases\in09_logic.ml"
if "%ARG%"=="10" set "FILE=testcases\in10_full.ml"

if not defined FILE set "FILE=%ARG%"
if not exist "%FILE%" (
    echo File not found: %FILE%
    exit /b 1
)

if not exist outputs mkdir outputs

echo ===== %FILE% =====
minicompiler.exe "%FILE%"
if errorlevel 1 exit /b 1

for %%N in ("%FILE%") do set "BASE=%%~nN"
copy /y output.tac outputs\%BASE%.tac >nul
copy /y output.asm outputs\%BASE%.asm >nul

echo.
echo Saved: outputs\%BASE%.tac  outputs\%BASE%.asm
echo.
echo --- output.tac ---
type output.tac
echo.
echo --- output.asm ---
type output.asm
exit /b 0

:list
echo.
echo  1  in01_arith.ml
echo  2  in02_assign.ml
echo  3  in03_if_else.ml
echo  4  in04_while.ml
echo  5  in05_bool.ml
echo  6  in06_rel.ml
echo  7  in07_block_scope.ml
echo  8  in08_nested_if.ml
echo  9  in09_logic.ml
echo 10  in10_full.ml
exit /b 0
