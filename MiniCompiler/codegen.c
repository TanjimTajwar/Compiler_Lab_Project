// Done by Hafiz Hasnat Sifat Jami

/*
 * Team Members:
 * - Tanjim Tajwar Arnab (22701066)
 * - Hafiz Hasnat Sifat Jami (22701068)
 * - Muznabin Ahmed (22701069)
 * - Monir Hossain (21701009)
 *
 * Primary Contributor:
 * Hafiz Hasnat Sifat Jami
 *
 * Contributors:
 * Tanjim Tajwar Arnab
 * Muznabin Ahmed
 * Monir Hossain
 *
 * Three-address code generation from AST.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dup_str(const char *s) {
    size_t n;
    char *p;
    if (!s) return NULL;
    n = strlen(s) + 1;
    p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static int temp_counter = 0;
static int label_counter = 0;

static char *new_temp(void) {
    char *name = (char *)malloc(16);
    if (name) snprintf(name, 16, "t%d", temp_counter++);
    return name;
}

static char *new_label(const char *prefix) {
    char *name = (char *)malloc(16);
    if (name) snprintf(name, 16, "%s%d", prefix, label_counter++);
    return name;
}

static void reset_counters(void) {
    temp_counter = 0;
    label_counter = 0;
}

static char *gen_expression(ASTNode *node, TACList *list);
static void gen_statement(ASTNode *node, TACList *list);
static void gen_stmt_list(StmtList *stmts, TACList *list);

TACList *codegen_generate(ASTNode *root) {
    TACList *list;

    if (!root) return NULL;

    reset_counters();
    list = tac_list_create();
    if (!list) return NULL;

    if (root->type == NODE_PROGRAM) {
        gen_stmt_list(root->u.program.statements, list);
    } else {
        gen_statement(root, list);
    }

    return list;
}

static void gen_stmt_list(StmtList *stmts, TACList *list) {
    int i;
    if (!stmts) return;
    for (i = 0; i < stmts->count; i++) {
        gen_statement(stmts->items[i], list);
    }
}

static void gen_statement(ASTNode *node, TACList *list) {
    char *cond;
    char *expr_result;
    char *label_end;
    char *label_else;
    char *label_body;
    char *label_cond;

    if (!node || !list) return;

    switch (node->type) {
        case NODE_DECL:
            break;

        case NODE_ASSIGN:
            expr_result = gen_expression(node->u.assign.expr, list);
            tac_emit(list, "=", expr_result, "", node->u.assign.name);
            free(expr_result);
            break;

        case NODE_IF:
            cond = gen_expression(node->u.if_stmt.condition, list);
            if (node->u.if_stmt.else_branch) {
                label_else = new_label("Lelse");
                label_end = new_label("Lend");
                tac_emit(list, "ifFalse", cond, "", label_else);
                gen_statement(node->u.if_stmt.then_branch, list);
                tac_emit(list, "goto", "", "", label_end);
                tac_emit(list, "label", "", "", label_else);
                gen_statement(node->u.if_stmt.else_branch, list);
                tac_emit(list, "label", "", "", label_end);
                free(label_else);
                free(label_end);
            } else {
                label_end = new_label("Lend");
                tac_emit(list, "ifFalse", cond, "", label_end);
                gen_statement(node->u.if_stmt.then_branch, list);
                tac_emit(list, "label", "", "", label_end);
                free(label_end);
            }
            free(cond);
            break;

        case NODE_WHILE:
            label_body = new_label("Lbody");
            label_cond = new_label("Lcond");
            tac_emit(list, "goto", "", "", label_cond);
            tac_emit(list, "label", "", "", label_body);
            gen_statement(node->u.while_stmt.body, list);
            tac_emit(list, "label", "", "", label_cond);
            cond = gen_expression(node->u.while_stmt.condition, list);
            tac_emit(list, "ifTrue", cond, "", label_body);
            free(cond);
            free(label_body);
            free(label_cond);
            break;

        case NODE_PRINT:
            expr_result = gen_expression(node->u.print_stmt.expr, list);
            tac_emit(list, "print", expr_result, "", "");
            free(expr_result);
            break;

        case NODE_BLOCK:
            gen_stmt_list(node->u.block.statements, list);
            break;

        default:
            break;
    }
}

static char *expr_to_string(ASTNode *node) {
    char *buf;
    if (!node) return dup_str("0");

    if (node->type == NODE_IDENT) {
        return dup_str(node->u.ident.name);
    }
    if (node->type == NODE_NUMBER) {
        buf = (char *)malloc(16);
        if (buf) snprintf(buf, 16, "%d", node->u.number.value);
        return buf;
    }
    return dup_str("0");
}

static char *gen_expression(ASTNode *node, TACList *list) {
    char *left;
    char *right;
    char *result;
    char *operand;

    if (!node) return dup_str("0");

    switch (node->type) {
        case NODE_IDENT:
        case NODE_NUMBER:
            return expr_to_string(node);

        case NODE_BINOP:
            left = gen_expression(node->u.binop.left, list);
            right = gen_expression(node->u.binop.right, list);
            result = new_temp();
            tac_emit(list, binop_to_string(node->u.binop.op),
                     left, right, result);
            free(left);
            free(right);
            return result;

        case NODE_RELOP:
            left = gen_expression(node->u.relop.left, list);
            right = gen_expression(node->u.relop.right, list);
            result = new_temp();
            tac_emit(list, relop_to_string(node->u.relop.op),
                     left, right, result);
            free(left);
            free(right);
            return result;

        case NODE_UNARY:
            operand = gen_expression(node->u.unary.operand, list);
            result = new_temp();
            if (node->u.unary.op == UNOP_NEG) {
                tac_emit(list, "uminus", operand, "", result);
            } else {
                tac_emit(list, "!", operand, "", result);
            }
            free(operand);
            return result;

        default:
            return dup_str("0");
    }
}
