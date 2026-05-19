# MiniLang Compiler — Makefile
# Developed by: Tanjim Tajwar Arnab (ID: 22701066) — Integration & Build

CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c11
FLEX    = flex
BISON   = bison

# On Windows (MSYS2), produce .exe; on Linux, plain name
ifeq ($(OS),Windows_NT)
TARGET  = minicompiler.exe
EXE     = .exe
else
TARGET  = minicompiler
EXE     =
endif
SRCS    = ast.c symbol_table.c semantic.c codegen.c optimizer.c target_code.c main.c
GEN_C   = lex.yy.c parser.tab.c
OBJS    = $(SRCS:.c=.o) lex.yy.o parser.tab.o

TEST    = testcases/test1.ml

.PHONY: all clean run

all: $(TARGET)

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.yy.o

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) -c parser.tab.c -o parser.tab.o

%.o: %.c ast.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET) -d $(TEST) || $(TARGET) -d $(TEST)
	@echo "--- output.tac ---"
	@cat output.tac
	@echo "--- output.asm ---"
	@cat output.asm

clean:
	rm -f $(TARGET) $(OBJS) lex.yy.c parser.tab.c parser.tab.h output.tac output.asm
