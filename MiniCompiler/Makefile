# MiniLang Compiler Makefile
#
# Team Members:
# - Tanjim Tajwar Arnab (22701066)
# - Hafiz Hasnat Sifat Jami (22701068)
# - Muznabin Ahmed (22701069)
# - Monir Hossain (21701009)
#
# Primary Contributor:
# Hafiz Hasnat Sifat Jami
#
# Contributors:
# Tanjim Tajwar Arnab
# Muznabin Ahmed
# Monir Hossain

CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c99
FLEX    = flex
BISON   = bison

TARGET  = minicompiler
SRCS    = ast.c symbol_table.c errors.c semantic.c codegen.c optimizer.c targetcode.c main.c
GENS    = lex.yy.c parser.tab.c parser.tab.h

OBJS    = $(GENS:.c=.o) $(SRCS:.c=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(GENS) $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) lex.yy.c parser.tab.c $(SRCS)

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) lexer.l

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d parser.y

%.o: %.c parser.tab.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(GENS) *.o output.tac output.asm
	rm -f lex.yy.c

test: $(TARGET)
	./$(TARGET) testcases/test1.ml
	./$(TARGET) testcases/test2.ml
	./$(TARGET) testcases/test3.ml
