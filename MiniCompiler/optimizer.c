/*
 * File: optimizer.c
 * Developed by: Monir Hossain (ID: 21701009)
 * Contribution: Constant Folding and Dead Code Elimination on TAC
 *
 * Complexity: constant folding O(n); DCE O(n * u) with u uses (linear scan).
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *opt_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

static int is_number(const char *s) {
    if (!s || !*s) return 0;
    if (*s == '-') s++;
    if (!*s) return 0;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

static int eval_binop(const char *op, int a, int b, int *out) {
    if (!strcmp(op, "+")) { *out = a + b; return 1; }
    if (!strcmp(op, "-")) { *out = a - b; return 1; }
    if (!strcmp(op, "*")) { *out = a * b; return 1; }
    if (!strcmp(op, "/")) { if (b == 0) return 0; *out = a / b; return 1; }
    if (!strcmp(op, "<"))  { *out = a < b; return 1; }
    if (!strcmp(op, ">"))  { *out = a > b; return 1; }
    if (!strcmp(op, "==")) { *out = a == b; return 1; }
    if (!strcmp(op, "!=")) { *out = a != b; return 1; }
    return 0;
}

/* Constant folding: replace ops on numeric literals with a single assignment */
static void fold_constants(TACList *tac) {
    for (TACInstr *p = tac->head; p; p = p->next) {
        if (p->dead || p->is_label) continue;
        if (!p->arg1 || !p->arg2 || !p->result) continue;
        if (!is_number(p->arg1) || !is_number(p->arg2)) continue;

        int a = atoi(p->arg1);
        int b = atoi(p->arg2);
        int r = 0;
        if (!eval_binop(p->op, a, b, &r)) continue;

        char buf[32];
        snprintf(buf, sizeof(buf), "%d", r);
        free(p->op);
        p->op = opt_strdup("=");
        free(p->arg1);
        p->arg1 = opt_strdup(buf);
        free(p->arg2);
        p->arg2 = NULL;
    }

    /* unary neg / ! on constants */
    for (TACInstr *p = tac->head; p; p = p->next) {
        if (p->dead || p->is_label) continue;
        if (!p->result || !p->arg1) continue;
        if (strcmp(p->op, "neg") == 0 && is_number(p->arg1)) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%d", -atoi(p->arg1));
            free(p->op);
            p->op = opt_strdup("=");
            free(p->arg1);
            p->arg1 = opt_strdup(buf);
        } else if (strcmp(p->op, "!") == 0 && is_number(p->arg1)) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%d", atoi(p->arg1) ? 0 : 1);
            free(p->op);
            p->op = opt_strdup("=");
            free(p->arg1);
            p->arg1 = opt_strdup(buf);
        }
    }
}

static int instr_uses(TACInstr *use, const char *name) {
    if (!name || !*name) return 0;
    if (use->arg1 && strcmp(use->arg1, name) == 0) return 1;
    if (use->arg2 && strcmp(use->arg2, name) == 0) return 1;
    if (use->op && strcmp(use->op, "if_false") == 0 && use->arg1 &&
        strcmp(use->arg1, name) == 0)
        return 1;
    return 0;
}

static int is_temp(const char *s) {
    return s && s[0] == 't' && isdigit((unsigned char)s[1]);
}

/* Dead code elimination: remove unused temp assignments */
static void eliminate_dead(TACList *tac) {
    for (TACInstr *def = tac->head; def; def = def->next) {
        if (def->dead || def->is_label) continue;
        if (!def->result || !is_temp(def->result)) continue;
        if (strcmp(def->op, "=") != 0 &&
            strcmp(def->op, "+") != 0 && strcmp(def->op, "-") != 0 &&
            strcmp(def->op, "*") != 0 && strcmp(def->op, "/") != 0 &&
            strcmp(def->op, "<") != 0 && strcmp(def->op, ">") != 0 &&
            strcmp(def->op, "==") != 0 && strcmp(def->op, "!=") != 0 &&
            strcmp(def->op, "&&") != 0 && strcmp(def->op, "||") != 0 &&
            strcmp(def->op, "neg") != 0 && strcmp(def->op, "!") != 0)
            continue;

        int used = 0;
        for (TACInstr *u = def->next; u; u = u->next) {
            if (u->dead) continue;
            if (instr_uses(u, def->result)) { used = 1; break; }
        }
        if (!used) def->dead = 1;
    }
}

TACList *optimize_tac(TACList *in) {
    if (!in) return in;
    fold_constants(in);
    eliminate_dead(in);
    return in;
}
