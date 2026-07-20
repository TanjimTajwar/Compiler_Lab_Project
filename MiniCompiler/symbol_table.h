/*
 * Team Members:
 * - Tanjim Tajwar Arnab (22701066)
 * - Hafiz Hasnat Sifat Jami (22701068)
 * - Muznabin Ahmed (22701069)
 * - Monir Hossain (21701009)
 *
 * Primary Contributor:
 * Monir Hossain
 *
 * Contributors:
 * Tanjim Tajwar Arnab
 * Hafiz Hasnat Sifat Jami
 * Muznabin Ahmed
 *
 * Symbol table with lexical scope management.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct Symbol {
    char *name;
    DataType type;
    int line;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *symbols;
    struct Scope *parent;
    int level;
} Scope;

typedef struct SymbolTable {
    Scope *current;
    int error_count;
} SymbolTable;

SymbolTable *symtab_create(void);
void symtab_destroy(SymbolTable *table);

void symtab_enter_scope(SymbolTable *table);
void symtab_exit_scope(SymbolTable *table);

int symtab_insert(SymbolTable *table, const char *name, DataType type, int line);
Symbol *symtab_lookup(SymbolTable *table, const char *name);
Symbol *symtab_lookup_current(SymbolTable *table, const char *name);

void symtab_print(SymbolTable *table);

#endif /* SYMBOL_TABLE_H */
