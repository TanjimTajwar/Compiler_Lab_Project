@echo off
REM Build MiniLang compiler on Windows (win_bison / win_flex / MSYS2 gcc)
setlocal EnableDelayedExpansion

cd /d "%~dp0"

REM --- find gcc (MSYS2 / MinGW) ---
set "GCC="
for %%P in (
    "C:\msys64\ucrt64\bin"
    "C:\msys64\mingw64\bin"
    "C:\msys64\clang64\bin"
    "C:\MinGW\bin"
    "C:\mingw-w64\bin"
    "C:\winflexbison"
) do (
    if exist %%P\gcc.exe (
        set "PATH=%%~P;%PATH%"
        set "GCC=%%~P\gcc.exe"
        goto :have_gcc
    )
)
where gcc >nul 2>&1 && set "GCC=gcc" && goto :have_gcc

echo.
echo ERROR: gcc not found in PATH.
echo Install ONE of these, then run build.bat again:
echo   1. MSYS2: https://www.msys2.org/
echo      pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-flex mingw-w64-ucrt-x86_64-bison
echo   2. Or add your MinGW bin folder to PATH
echo.
exit /b 1

:have_gcc
echo Using gcc: %GCC%

REM --- bison ---
set "BISON=win_bison"
if exist "C:\winflexbison\win_bison.exe" set "PATH=C:\winflexbison;%PATH%"
where win_bison >nul 2>&1 || set "BISON=bison"
where %BISON% >nul 2>&1 || (
    echo ERROR: win_bison / bison not found. Install winflexbison or MSYS2 bison.
    exit /b 1
)
echo Using bison: %BISON%

REM --- flex ---
set "FLEX=win_flex"
if exist "C:\winflexbison\win_flex.exe" set "PATH=C:\winflexbison;%PATH%"
where win_flex >nul 2>&1 || set "FLEX=flex"
where %FLEX% >nul 2>&1 || (
    echo ERROR: win_flex / flex not found.
    exit /b 1
)
echo Using flex: %FLEX%

echo.
echo [1/3] Parser...
%BISON% -d -o parser.tab.c parser.y
if errorlevel 1 exit /b 1
if exist y.tab.h if not exist parser.tab.h copy /y y.tab.h parser.tab.h >nul
if exist y.tab.c if not exist parser.tab.c copy /y y.tab.c parser.tab.c >nul

echo [2/3] Lexer...
%FLEX% -o lex.yy.c lexer.l
if errorlevel 1 exit /b 1

echo [3/3] Compile...
%GCC% -Wall -Wextra -std=c11 -O0 -Wno-unused-function -Wno-unused-variable -o minicompiler.exe ^
    lex.yy.c parser.tab.c ^
    ast.c symbol_table.c semantic.c codegen.c optimizer.c target_code.c main.c
if errorlevel 1 exit /b 1

echo.
echo SUCCESS: minicompiler.exe
exit /b 0
