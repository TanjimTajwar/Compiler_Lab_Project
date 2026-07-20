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
 * Pseudo-assembly code generation from optimized TAC.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void targetcode_generate(TACList *list, FILE *out) {
    TAC *cur;

    if (!list || !out) return;

    fprintf(out, "; MiniLang Pseudo Assembly\n");
    fprintf(out, "; Machine-independent target code\n\n");

    for (cur = list->head; cur; cur = cur->next) {
        if (strcmp(cur->op, "label") == 0) {
            fprintf(out, "%s:\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "goto") == 0) {
            fprintf(out, "JMP %s\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "ifFalse") == 0) {
            fprintf(out, "LOAD %s\n", cur->arg1);
            fprintf(out, "JZ %s\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "ifTrue") == 0) {
            fprintf(out, "LOAD %s\n", cur->arg1);
            fprintf(out, "JNZ %s\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "print") == 0) {
            fprintf(out, "LOAD %s\n", cur->arg1);
            fprintf(out, "PRINT\n");
            continue;
        }

        if (strcmp(cur->op, "=") == 0) {
            if (isdigit((unsigned char)cur->arg1[0]) ||
                (cur->arg1[0] == '-' && isdigit((unsigned char)cur->arg1[1]))) {
                fprintf(out, "LOAD_IMM %s\n", cur->arg1);
            } else {
                fprintf(out, "LOAD %s\n", cur->arg1);
            }
            fprintf(out, "STORE %s\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "uminus") == 0) {
            fprintf(out, "LOAD %s\n", cur->arg1);
            fprintf(out, "NEG\n");
            fprintf(out, "STORE %s\n", cur->result);
            continue;
        }

        if (strcmp(cur->op, "!") == 0) {
            fprintf(out, "LOAD %s\n", cur->arg1);
            fprintf(out, "NOT\n");
            fprintf(out, "STORE %s\n", cur->result);
            continue;
        }

        if (cur->arg2[0] != '\0') {
            fprintf(out, "LOAD %s\n", cur->arg1);
            if (strcmp(cur->op, "+") == 0) {
                fprintf(out, "ADD %s\n", cur->arg2);
            } else if (strcmp(cur->op, "-") == 0) {
                fprintf(out, "SUB %s\n", cur->arg2);
            } else if (strcmp(cur->op, "*") == 0) {
                fprintf(out, "MUL %s\n", cur->arg2);
            } else if (strcmp(cur->op, "/") == 0) {
                fprintf(out, "DIV %s\n", cur->arg2);
            } else if (strcmp(cur->op, "<") == 0) {
                fprintf(out, "LT %s\n", cur->arg2);
            } else if (strcmp(cur->op, ">") == 0) {
                fprintf(out, "GT %s\n", cur->arg2);
            } else if (strcmp(cur->op, "<=") == 0) {
                fprintf(out, "LE %s\n", cur->arg2);
            } else if (strcmp(cur->op, ">=") == 0) {
                fprintf(out, "GE %s\n", cur->arg2);
            } else if (strcmp(cur->op, "==") == 0) {
                fprintf(out, "EQ %s\n", cur->arg2);
            } else if (strcmp(cur->op, "!=") == 0) {
                fprintf(out, "NE %s\n", cur->arg2);
            } else {
                fprintf(out, "; unknown op %s\n", cur->op);
            }
            fprintf(out, "STORE %s\n", cur->result);
        }
    }

    fprintf(out, "\nHALT\n");
}
