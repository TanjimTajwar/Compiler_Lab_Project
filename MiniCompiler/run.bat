@echo off
REM Run the compiler on a test file (default: testcases\test1.ml)
setlocal

if not exist minicompiler.exe (
    echo minicompiler.exe not found. Run build.bat first.
    exit /b 1
)

set "SRC=%~1"
if "%SRC%"=="" set "SRC=testcases\test1.ml"

minicompiler.exe %SRC%
if errorlevel 1 exit /b 1

echo.
echo --- output.tac ---
type output.tac
echo.
echo --- output.asm ---
type output.asm
exit /b 0
