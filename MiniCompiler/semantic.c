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
 * Hafiz Hasnat Sifat Jami
 * Muznabin Ahmed
 * Monir Hossain
 *
 * Semantic analysis: type checking and symbol management.
 */

#include "ast.h"
#include "symbol_table.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

static SymbolTable *g_symtab = NULL;

static DataType check_expression(ASTNode *node);
static void analyze_statement(ASTNode *node);
static void analyze_stmt_list(StmtList *list);

int semantic_analyze(ASTNode *root) {
    if (!root) return 0;

    g_symtab = symtab_create();
    if (!g_symtab) return 0;

    if (root->type == NODE_PROGRAM) {
        analyze_stmt_list(root->u.program.statements);
    } else {
        analyze_statement(root);
    }

    int errors = error_get_count();

    symtab_destroy(g_symtab);
    g_symtab = NULL;

    return errors;
}

int semantic_get_error_count(void) {
    return error_get_count();
}

static void analyze_stmt_list(StmtList *list) {
    int i;
    if (!list) return;
    for (i = 0; i < list->count; i++) {
        analyze_statement(list->items[i]);
    }
}

static void analyze_statement(ASTNode *node) {
    Symbol *sym;
    DataType expr_type;

    if (!node) return;

    switch (node->type) {
        case NODE_DECL:
            symtab_insert(g_symtab, node->u.decl.name,
                          node->u.decl.var_type, node->line);
            break;

        case NODE_ASSIGN:
            sym = symtab_lookup(g_symtab, node->u.assign.name);
            if (!sym) {
                error_report(ERR_SEMANTIC, node->line,
                             "Undeclared variable '%s'", node->u.assign.name);
            } else {
                expr_type = check_expression(node->u.assign.expr);
                if (expr_type != TYPE_UNKNOWN && sym->type != expr_type) {
                    if (!(sym->type == TYPE_BOOL && expr_type == TYPE_INT)) {
                        error_report(ERR_SEMANTIC, node->line,
                                     "Type mismatch in assignment to '%s': expected %s, got %s",
                                     node->u.assign.name,
                                     data_type_to_string(sym->type),
                                     data_type_to_string(expr_type));
                    }
                }
                node->data_type = sym->type;
            }
            break;

        case NODE_IF:
            expr_type = check_expression(node->u.if_stmt.condition);
            if (expr_type != TYPE_BOOL && expr_type != TYPE_UNKNOWN) {
                error_report(ERR_SEMANTIC, node->line,
                             "Invalid conditional expression: expected bool");
            }
            analyze_statement(node->u.if_stmt.then_branch);
            if (node->u.if_stmt.else_branch) {
                analyze_statement(node->u.if_stmt.else_branch);
            }
            break;

        case NODE_WHILE:
            expr_type = check_expression(node->u.while_stmt.condition);
            if (expr_type != TYPE_BOOL && expr_type != TYPE_UNKNOWN) {
                error_report(ERR_SEMANTIC, node->line,
                             "Invalid conditional expression in while: expected bool");
            }
            analyze_statement(node->u.while_stmt.body);
            break;

        case NODE_PRINT:
            check_expression(node->u.print_stmt.expr);
            break;

        case NODE_BLOCK:
            symtab_enter_scope(g_symtab);
            analyze_stmt_list(node->u.block.statements);
            symtab_exit_scope(g_symtab);
            break;

        default:
            break;
    }
}

static DataType check_expression(ASTNode *node) {
    Symbol *sym;
    DataType left_type;
    DataType right_type;

    if (!node) return TYPE_UNKNOWN;

    switch (node->type) {
        case NODE_IDENT:
            sym = symtab_lookup(g_symtab, node->u.ident.name);
            if (!sym) {
                error_report(ERR_SEMANTIC, node->line,
                             "Undeclared variable '%s'", node->u.ident.name);
                node->data_type = TYPE_UNKNOWN;
            } else {
                node->data_type = sym->type;
            }
            return node->data_type;

        case NODE_NUMBER:
            node->data_type = TYPE_INT;
            return TYPE_INT;

        case NODE_BINOP:
            left_type = check_expression(node->u.binop.left);
            right_type = check_expression(node->u.binop.right);
            if (left_type != TYPE_INT || right_type != TYPE_INT) {
                if (left_type != TYPE_UNKNOWN && right_type != TYPE_UNKNOWN) {
                    error_report(ERR_SEMANTIC, node->line,
                                 "Arithmetic operands must be of type int");
                }
            }
            node->data_type = TYPE_INT;
            return TYPE_INT;

        case NODE_RELOP:
            left_type = check_expression(node->u.relop.left);
            right_type = check_expression(node->u.relop.right);
            if (left_type != TYPE_INT || right_type != TYPE_INT) {
                if (left_type != TYPE_UNKNOWN && right_type != TYPE_UNKNOWN) {
                    error_report(ERR_SEMANTIC, node->line,
                                 "Relational operands must be of type int");
                }
            }
            node->data_type = TYPE_BOOL;
            return TYPE_BOOL;

        case NODE_UNARY:
            if (node->u.unary.op == UNOP_NOT) {
                right_type = check_expression(node->u.unary.operand);
                if (right_type != TYPE_BOOL && right_type != TYPE_UNKNOWN) {
                    error_report(ERR_SEMANTIC, node->line,
                                 "Logical NOT operand must be of type bool");
                }
                node->data_type = TYPE_BOOL;
                return TYPE_BOOL;
            } else {
                right_type = check_expression(node->u.unary.operand);
                if (right_type != TYPE_INT && right_type != TYPE_UNKNOWN) {
                    error_report(ERR_SEMANTIC, node->line,
                                 "Unary minus operand must be of type int");
                }
                node->data_type = TYPE_INT;
                return TYPE_INT;
            }

        default:
            return TYPE_UNKNOWN;
    }
}
