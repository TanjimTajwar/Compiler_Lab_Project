# MiniLang Compiler Makefile // Add short file purpose note.
# CSE 712 project automation // Add project context note.

CC=gcc # Select GNU C compiler command.
LEX=flex # Select Flex scanner generator command.
YACC=bison # Select Bison parser generator command.
CFLAGS=-Wall -Wextra -std=c11 # Set common compile warning flags.

TARGET=minicompiler # Define final executable name.
SRC=ast.c symbol_table.c semantic.c codegen.c main.c parser.tab.c lex.yy.c # List all compilation units.

all: $(TARGET) # Set default build target.

parser.tab.c parser.tab.h: parser.y # Generate parser source and header from grammar.
	$(YACC) -d parser.y # Run Bison with header generation.

lex.yy.c: lexer.l parser.tab.h # Generate lexer source after parser header exists.
	$(LEX) lexer.l # Run Flex scanner generator.

$(TARGET): parser.tab.c lex.yy.c ast.c symbol_table.c semantic.c codegen.c main.c # Build executable from all sources.
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) # Compile and link project into binary.

clean: # Remove generated and output files.
	rm -f $(TARGET) parser.tab.c parser.tab.h lex.yy.c output.tac output.asm # Delete build artifacts.

.PHONY: all clean # Mark phony make targets.
