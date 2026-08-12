// Done by Muznabin Ahmed


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
 * Symbol table implementation with nested scopes.
 */

#include "symbol_table.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *strdup_safe(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}

SymbolTable *symtab_create(void) {
    SymbolTable *table = (SymbolTable *)calloc(1, sizeof(SymbolTable));
    if (!table) return NULL;
    symtab_enter_scope(table);
    return table;
}

void symtab_destroy(SymbolTable *table) {
    while (table && table->current) {
        symtab_exit_scope(table);
    }
    free(table);
}

void symtab_enter_scope(SymbolTable *table) {
    Scope *scope;
    if (!table) return;

    scope = (Scope *)calloc(1, sizeof(Scope));
    if (!scope) return;

    scope->parent = table->current;
    scope->level = table->current ? table->current->level + 1 : 0;
    table->current = scope;
}

void symtab_exit_scope(SymbolTable *table) {
    Scope *old;
    Symbol *sym;
    Symbol *next;

    if (!table || !table->current) return;

    old = table->current;
    sym = old->symbols;
    while (sym) {
        next = sym->next;
        free(sym->name);
        free(sym);
        sym = next;
    }

    table->current = old->parent;
    free(old);
}

int symtab_insert(SymbolTable *table, const char *name, DataType type, int line) {
    Symbol *sym;

    if (!table || !name) return 0;

    if (symtab_lookup_current(table, name)) {
        error_report(ERR_SEMANTIC, line,
                     "Duplicate declaration of variable '%s'", name);
        table->error_count++;
        return 0;
    }

    sym = (Symbol *)calloc(1, sizeof(Symbol));
    if (!sym) return 0;

    sym->name = strdup_safe(name);
    sym->type = type;
    sym->line = line;
    sym->next = table->current->symbols;
    table->current->symbols = sym;
    return 1;
}

Symbol *symtab_lookup(SymbolTable *table, const char *name) {
    Scope *scope;
    Symbol *sym;

    if (!table || !name) return NULL;

    for (scope = table->current; scope; scope = scope->parent) {
        for (sym = scope->symbols; sym; sym = sym->next) {
            if (strcmp(sym->name, name) == 0) {
                return sym;
            }
        }
    }
    return NULL;
}

Symbol *symtab_lookup_current(SymbolTable *table, const char *name) {
    Symbol *sym;

    if (!table || !table->current || !name) return NULL;

    for (sym = table->current->symbols; sym; sym = sym->next) {
        if (strcmp(sym->name, name) == 0) {
            return sym;
        }
    }
    return NULL;
}

void symtab_print(SymbolTable *table) {
    Scope *scope;
    Symbol *sym;

    if (!table) return;

    printf("=== Symbol Table ===\n");
    for (scope = table->current; scope; scope = scope->parent) {
        printf("Scope level %d:\n", scope->level);
        for (sym = scope->symbols; sym; sym = sym->next) {
            printf("  %s : %s (declared at line %d)\n",
                   sym->name, data_type_to_string(sym->type), sym->line);
        }
    }
}
