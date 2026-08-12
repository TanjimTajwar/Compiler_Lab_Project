//Done by Tanjim Tajwar Arnab

/*

 * Team Members:
 * - Tanjim Tajwar Arnab (22701066)
 * - Hafiz Hasnat Sifat Jami (22701068)
 * - Muznabin Ahmed (22701069)
 * - Monir Hossain (21701009)
 *
 * Primary Contributor:
 * Tanjim Tajwar Arnab
 *
 * Contributors:
 * Tanjim Tajwar Arnab
 * Hafiz Hasnat Sifat Jami
 * Monir Hossain
 *
 * TAC optimization: constant folding, algebraic simplification,
 * and redundant temporary elimination.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int is_number(const char *s) {
    int i;
    if (!s || !*s) return 0;
    if (*s == '-' && s[1]) i = 1;
    else i = 0;
    for (; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }
    return 1;
}

static int to_int(const char *s) {
    return atoi(s);
}

static char *int_to_str(int v) {
    char *buf = (char *)malloc(16);
    if (buf) snprintf(buf, 16, "%d", v);
    return buf;
}

static int eval_binop(const char *op, int a, int b, int *ok) {
    *ok = 1;
    if (strcmp(op, "+") == 0) return a + b;
    if (strcmp(op, "-") == 0) return a - b;
    if (strcmp(op, "*") == 0) return a * b;
    if (strcmp(op, "/") == 0) {
        if (b == 0) { *ok = 0; return 0; }
        return a / b;
    }
    if (strcmp(op, "<") == 0) return a < b;
    if (strcmp(op, ">") == 0) return a > b;
    if (strcmp(op, "<=") == 0) return a <= b;
    if (strcmp(op, ">=") == 0) return a >= b;
    if (strcmp(op, "==") == 0) return a == b;
    if (strcmp(op, "!=") == 0) return a != b;
    *ok = 0;
    return 0;
}

static int is_temp(const char *s) {
    return s && s[0] == 't' && isdigit((unsigned char)s[1]);
}

static void replace_operand(char *field, const char *from, const char *to) {
    if (field && from && to && strcmp(field, from) == 0) {
        strncpy(field, to, sizeof(((TAC *)0)->arg1) - 1);
        field[sizeof(((TAC *)0)->arg1) - 1] = '\0';
    }
}

static void propagate_temp(TACList *list, const char *temp, const char *value) {
    TAC *cur;
    for (cur = list->head; cur; cur = cur->next) {
        replace_operand(cur->arg1, temp, value);
        replace_operand(cur->arg2, temp, value);
    }
}

static int fold_constants(TAC *instr) {
    int a, b, result, ok;
    char *res_str;

    if (!instr || instr->arg2[0] == '\0') return 0;

    if (!is_number(instr->arg1) || !is_number(instr->arg2)) return 0;

    a = to_int(instr->arg1);
    b = to_int(instr->arg2);
    result = eval_binop(instr->op, a, b, &ok);
    if (!ok) return 0;

    res_str = int_to_str(result);
    strncpy(instr->arg1, res_str, sizeof(instr->arg1) - 1);
    instr->arg2[0] = '\0';
    strncpy(instr->op, "=", sizeof(instr->op) - 1);
    free(res_str);
    return 1;
}

static int simplify_algebraic(TAC *instr) {
    if (!instr || instr->arg2[0] == '\0') return 0;

    if (strcmp(instr->op, "+") == 0) {
        if (strcmp(instr->arg2, "0") == 0) {
            strncpy(instr->op, "=", sizeof(instr->op) - 1);
            instr->arg2[0] = '\0';
            return 1;
        }
        if (strcmp(instr->arg1, "0") == 0) {
            strncpy(instr->arg1, instr->arg2, sizeof(instr->arg1) - 1);
            strncpy(instr->op, "=", sizeof(instr->op) - 1);
            instr->arg2[0] = '\0';
            return 1;
        }
    }

    if (strcmp(instr->op, "*") == 0) {
        if (strcmp(instr->arg2, "1") == 0) {
            strncpy(instr->op, "=", sizeof(instr->op) - 1);
            instr->arg2[0] = '\0';
            return 1;
        }
        if (strcmp(instr->arg1, "1") == 0) {
            strncpy(instr->arg1, instr->arg2, sizeof(instr->arg1) - 1);
            strncpy(instr->op, "=", sizeof(instr->op) - 1);
            instr->arg2[0] = '\0';
            return 1;
        }
        if (strcmp(instr->arg2, "0") == 0 || strcmp(instr->arg1, "0") == 0) {
            strncpy(instr->arg1, "0", sizeof(instr->arg1) - 1);
            strncpy(instr->op, "=", sizeof(instr->op) - 1);
            instr->arg2[0] = '\0';
            return 1;
        }
    }

    if (strcmp(instr->op, "-") == 0 && strcmp(instr->arg2, "0") == 0) {
        strncpy(instr->op, "=", sizeof(instr->op) - 1);
        instr->arg2[0] = '\0';
        return 1;
    }

    if (strcmp(instr->op, "/") == 0 && strcmp(instr->arg2, "1") == 0) {
        strncpy(instr->op, "=", sizeof(instr->op) - 1);
        instr->arg2[0] = '\0';
        return 1;
    }

    return 0;
}

static int remove_redundant_temp(TACList *list, TAC *instr, TAC *next) {
    if (!instr || !next) return 0;

    if (strcmp(instr->op, "=") != 0) return 0;
    if (!is_temp(instr->result)) return 0;
    if (strcmp(next->op, "=") != 0) return 0;
    if (strcmp(next->arg1, instr->result) != 0) return 0;

    propagate_temp(list, instr->result, instr->arg1);
    instr->result[0] = '\0';
    instr->op[0] = '\0';
    return 1;
}

static void compact_tac_list(TACList *list) {
    TAC *cur;
    TAC *prev;
    TAC *next;

    if (!list) return;

    prev = NULL;
    cur = list->head;
    while (cur) {
        next = cur->next;
        if (cur->op[0] == '\0') {
            if (prev) prev->next = next;
            else list->head = next;
            if (cur == list->tail) list->tail = prev;
            free(cur);
            list->count--;
        } else {
            prev = cur;
        }
        cur = next;
    }
}

void optimizer_run(TACList *list) {
    TAC *cur;
    TAC *next;
    int changed;
    int pass;

    if (!list) return;

    for (pass = 0; pass < 3; pass++) {
        changed = 0;
        for (cur = list->head; cur; cur = cur->next) {
            if (fold_constants(cur)) changed = 1;
            if (simplify_algebraic(cur)) changed = 1;
        }
        if (!changed) break;
    }

    for (cur = list->head; cur; cur = cur->next) {
        next = cur->next;
        if (remove_redundant_temp(list, cur, next)) {
            compact_tac_list(list);
        }
    }

    for (cur = list->head; cur; cur = cur->next) {
        if (strcmp(cur->op, "=") == 0 &&
            is_temp(cur->result) &&
            !is_temp(cur->arg1) &&
            cur->next &&
            strcmp(cur->next->op, "=") == 0 &&
            strcmp(cur->next->arg1, cur->result) == 0) {
            propagate_temp(list, cur->result, cur->arg1);
            cur->result[0] = '\0';
            cur->op[0] = '\0';
        }
    }
    compact_tac_list(list);
}
