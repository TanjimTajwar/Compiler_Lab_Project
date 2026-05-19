/*
 * File: codegen.c
 * Developed by: Tanjim Tajwar Arnab (ID: 22701066)
 * Contribution: Three-Address Code (TAC) Generation
 *
 * Complexity: O(n) AST traversal; O(1) TAC emit per node.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char place[64];
} ExprResult;

static void codegen_stmt(ASTNode *n, TACList *tac);
static ExprResult codegen_expr(ASTNode *n, TACList *tac);

static void set_place(ExprResult *r, const char *s) {
    strncpy(r->place, s, sizeof(r->place) - 1);
    r->place[sizeof(r->place) - 1] = '\0';
}

static const char *binop_tac(BinOpKind op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_LT:  return "<";
        case OP_GT:  return ">";
        case OP_EQ:  return "==";
        case OP_NE:  return "!=";
        case OP_AND: return "&&";
        case OP_OR:  return "||";
        default:     return "?";
    }
}

static ExprResult codegen_expr(ASTNode *n, TACList *tac) {
    ExprResult res;
    res.place[0] = '\0';

    if (!n) return res;

    switch (n->type) {
        case NODE_INT: {
            char buf[32];
            snprintf(buf, sizeof(buf), "%d", n->u.intval.value);
            set_place(&res, buf);
            break;
        }
        case NODE_BOOL: {
            set_place(&res, n->u.boolval.value ? "1" : "0");
            break;
        }
        case NODE_ID:
            set_place(&res, n->u.id.name);
            break;
        case NODE_BINOP: {
            ExprResult l = codegen_expr(n->u.binop.left, tac);
            ExprResult r = codegen_expr(n->u.binop.right, tac);
            char *tmp = tac_new_temp(tac);
            tac_emit(tac, n->line, binop_tac(n->u.binop.op), l.place, r.place, tmp);
            set_place(&res, tmp);
            free(tmp);
            break;
        }
        case NODE_UNOP: {
            ExprResult o = codegen_expr(n->u.unop.operand, tac);
            char *tmp = tac_new_temp(tac);
            if (n->u.unop.op == OP_NOT)
                tac_emit(tac, n->line, "!", o.place, NULL, tmp);
            else
                tac_emit(tac, n->line, "neg", o.place, NULL, tmp);
            set_place(&res, tmp);
            free(tmp);
            break;
        }
        default:
            break;
    }
    return res;
}

static void codegen_stmt(ASTNode *n, TACList *tac) {
    if (!n) return;

    switch (n->type) {
        case NODE_DECL:
            /* no TAC for decl — symbol table only */
            break;
        case NODE_ASSIGN: {
            ExprResult e = codegen_expr(n->u.assign.value, tac);
            tac_emit(tac, n->line, "=", e.place, NULL, n->u.assign.name);
            break;
        }
        case NODE_PRINT: {
            ExprResult e = codegen_expr(n->u.print_stmt.expr, tac);
            tac_emit(tac, n->line, "param", e.place, NULL, NULL);
            tac_emit(tac, n->line, "call", "print", "1", NULL);
            break;
        }
        case NODE_IF: {
            ExprResult c = codegen_expr(n->u.if_stmt.cond, tac);
            char *Lelse = tac_new_label(tac);
            char *Lend = tac_new_label(tac);
            tac_emit(tac, n->line, "if_false", c.place, NULL, Lelse);
            codegen_stmt(n->u.if_stmt.then_br, tac);
            if (n->u.if_stmt.else_br) {
                tac_emit(tac, n->line, "goto", NULL, NULL, Lend);
                tac_emit_label(tac, n->line, Lelse);
                codegen_stmt(n->u.if_stmt.else_br, tac);
                tac_emit_label(tac, n->line, Lend);
            } else {
                tac_emit_label(tac, n->line, Lelse);
            }
            free(Lelse);
            free(Lend);
            break;
        }
        case NODE_WHILE: {
            char *Lstart = tac_new_label(tac);
            char *Lend = tac_new_label(tac);
            tac_emit_label(tac, n->line, Lstart);
            ExprResult c = codegen_expr(n->u.while_stmt.cond, tac);
            tac_emit(tac, n->line, "if_false", c.place, NULL, Lend);
            codegen_stmt(n->u.while_stmt.body, tac);
            tac_emit(tac, n->line, "goto", NULL, NULL, Lstart);
            tac_emit_label(tac, n->line, Lend);
            free(Lstart);
            free(Lend);
            break;
        }
        case NODE_BLOCK:
            for (int i = 0; i < n->u.block.count; i++)
                codegen_stmt(n->u.block.stmts[i], tac);
            break;
        default:
            break;
    }
}

TACList *codegen_generate(ASTNode *root) {
    TACList *tac = tac_create();
    if (!root || root->type != NODE_PROGRAM) return tac;

    for (int i = 0; i < root->u.program.count; i++)
        codegen_stmt(root->u.program.stmts[i], tac);

    tac_emit(tac, 0, "halt", NULL, NULL, NULL);
    return tac;
}
