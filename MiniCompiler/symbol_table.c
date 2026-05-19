/*
 * File: symbol_table.c
 * Developed by: Muznabin Ahmed (ID: 22701069)
 * Contribution: Scope-Based Symbol Table with Stack
 *
 * Complexity: insert/lookup O(1) average per scope (hash could be used;
 *             linear chain per scope — O(s) per scope with s symbols);
 *             enter/exit scope O(1).
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Scope *current_scope = NULL;
static int scope_depth = 0;

void symtab_init(void) {
    symtab_free();
    symtab_enter_scope(); /* global scope */
}

void symtab_enter_scope(void) {
    Scope *sc = (Scope *)calloc(1, sizeof(Scope));
    sc->level = scope_depth++;
    sc->parent = current_scope;
    current_scope = sc;
}

void symtab_exit_scope(void) {
    if (!current_scope) return;
    Scope *old = current_scope;
    current_scope = old->parent;
    scope_depth--;

    Symbol *sym = old->symbols;
    while (sym) {
        Symbol *n = sym->next;
        free(sym->name);
        free(sym);
        sym = n;
    }
    free(old);
}

static Symbol *find_in_scope(Scope *sc, const char *name) {
    for (Symbol *s = sc->symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static char *sym_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

int symtab_insert(char *name, DataType type, int line) {
    if (!current_scope) symtab_init();
    if (find_in_scope(current_scope, name)) {
        return 0; /* duplicate in same scope */
    }
    Symbol *sym = (Symbol *)calloc(1, sizeof(Symbol));
    sym->name = sym_strdup(name);
    sym->type = type;
    sym->scope_level = current_scope->level;
    sym->line_declared = line;
    sym->next = current_scope->symbols;
    current_scope->symbols = sym;
    return 1;
}

Symbol *symtab_lookup(char *name) {
    for (Scope *sc = current_scope; sc; sc = sc->parent) {
        Symbol *found = find_in_scope(sc, name);
        if (found) return found;
    }
    return NULL;
}

Symbol *symtab_lookup_current(char *name) {
    if (!current_scope) return NULL;
    return find_in_scope(current_scope, name);
}

void symtab_print(void) {
    printf("=== Symbol Table ===\n");
    for (Scope *sc = current_scope; sc; sc = sc->parent) {
        printf("Scope level %d:\n", sc->level);
        for (Symbol *s = sc->symbols; s; s = s->next) {
            const char *t = (s->type == DTYPE_INT) ? "int" : "bool";
            printf("  %s : %s (declared line %d)\n", s->name, t, s->line_declared);
        }
    }
    printf("====================\n");
}

void symtab_free(void) {
    while (current_scope && current_scope->parent)
        symtab_exit_scope();
    if (current_scope) {
        Symbol *sym = current_scope->symbols;
        while (sym) {
            Symbol *n = sym->next;
            free(sym->name);
            free(sym);
            sym = n;
        }
        free(current_scope);
        current_scope = NULL;
    }
    scope_depth = 0;
}
