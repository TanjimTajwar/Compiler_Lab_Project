/*
 * File: semantic.c
 * Developed by: Tanjim Tajwar Arnab (ID: 22701066)
 * Contribution: Semantic Analysis, Type Checking, Symbol Integration
 *
 * Complexity: O(n) AST visit; symbol lookups O(d*s) worst case (depth * symbols).
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void sem_error(int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Semantic error at line %d: ", line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    semantic_error_count++;
}

static DataType check_expr(ASTNode *n);

static void analyze_stmt(ASTNode *n) {
    if (!n) return;

    switch (n->type) {
        case NODE_DECL: {
            char *name = n->u.decl.name;
            if (!symtab_insert(name, n->u.decl.decl_type, n->line)) {
                sem_error(n->line,
                    "redeclaration of '%s'", name);
                fprintf(stderr,
                    "  Suggestion: use a unique variable name in this scope.\n");
            }
            break;
        }
        case NODE_ASSIGN: {
            Symbol *sym = symtab_lookup(n->u.assign.name);
            if (!sym) {
                sem_error(n->line,
                    "undeclared variable '%s'", n->u.assign.name);
                fprintf(stderr,
                    "  Suggestion: declare '%s' before use.\n", n->u.assign.name);
                break;
            }
            DataType rt = check_expr(n->u.assign.value);
            if (rt != DTYPE_UNKNOWN && rt != sym->type) {
                sem_error(n->line,
                    "type mismatch in assignment to '%s' (expected %s)",
                    n->u.assign.name,
                    sym->type == DTYPE_INT ? "int" : "bool");
                fprintf(stderr,
                    "  Suggestion: cast or change the expression type.\n");
            }
            break;
        }
        case NODE_IF:
            check_expr(n->u.if_stmt.cond);
            analyze_stmt(n->u.if_stmt.then_br);
            if (n->u.if_stmt.else_br)
                analyze_stmt(n->u.if_stmt.else_br);
            break;
        case NODE_WHILE:
            check_expr(n->u.while_stmt.cond);
            analyze_stmt(n->u.while_stmt.body);
            break;
        case NODE_PRINT:
            check_expr(n->u.print_stmt.expr);
            break;
        case NODE_BLOCK:
            symtab_enter_scope();
            for (int i = 0; i < n->u.block.count; i++)
                analyze_stmt(n->u.block.stmts[i]);
            symtab_exit_scope();
            break;
        default:
            break;
    }
}

static DataType result_type_binop(BinOpKind op, DataType l, DataType r, int line) {
    if (l == DTYPE_UNKNOWN || r == DTYPE_UNKNOWN) return DTYPE_UNKNOWN;

    switch (op) {
        case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
            if (l != DTYPE_INT || r != DTYPE_INT) {
                sem_error(line, "arithmetic operators require int operands");
                fprintf(stderr, "  Suggestion: use int expressions only.\n");
                return DTYPE_UNKNOWN;
            }
            return DTYPE_INT;
        case OP_LT: case OP_GT: case OP_EQ: case OP_NE:
            if (l != r) {
                sem_error(line, "relational comparison requires same types");
                fprintf(stderr, "  Suggestion: compare values of the same type.\n");
                return DTYPE_UNKNOWN;
            }
            return DTYPE_BOOL;
        case OP_AND: case OP_OR:
            if (l != DTYPE_BOOL || r != DTYPE_BOOL) {
                sem_error(line, "logical operators require bool operands");
                fprintf(stderr, "  Suggestion: use bool expressions with && or ||.\n");
                return DTYPE_UNKNOWN;
            }
            return DTYPE_BOOL;
        default:
            return DTYPE_UNKNOWN;
    }
}

static DataType check_expr(ASTNode *n) {
    if (!n) return DTYPE_UNKNOWN;

    switch (n->type) {
        case NODE_INT:
            ast_set_type(n, DTYPE_INT);
            return DTYPE_INT;
        case NODE_BOOL:
            ast_set_type(n, DTYPE_BOOL);
            return DTYPE_BOOL;
        case NODE_ID: {
            Symbol *sym = symtab_lookup(n->u.id.name);
            if (!sym) {
                sem_error(n->line, "undeclared identifier '%s'", n->u.id.name);
                fprintf(stderr, "  Suggestion: declare '%s' first.\n", n->u.id.name);
                return DTYPE_UNKNOWN;
            }
            ast_set_type(n, sym->type);
            return sym->type;
        }
        case NODE_BINOP: {
            DataType l = check_expr(n->u.binop.left);
            DataType r = check_expr(n->u.binop.right);
            DataType t = result_type_binop(n->u.binop.op, l, r, n->line);
            ast_set_type(n, t);
            return t;
        }
        case NODE_UNOP: {
            DataType o = check_expr(n->u.unop.operand);
            if (n->u.unop.op == OP_NOT) {
                if (o != DTYPE_BOOL && o != DTYPE_UNKNOWN) {
                    sem_error(n->line, "operator '!' requires bool operand");
                    fprintf(stderr, "  Suggestion: use ! on a bool expression.\n");
                }
                ast_set_type(n, DTYPE_BOOL);
                return DTYPE_BOOL;
            }
            if (n->u.unop.op == OP_NEG) {
                if (o != DTYPE_INT && o != DTYPE_UNKNOWN) {
                    sem_error(n->line, "unary '-' requires int operand");
                    fprintf(stderr, "  Suggestion: negate an int expression.\n");
                }
                ast_set_type(n, DTYPE_INT);
                return DTYPE_INT;
            }
            return DTYPE_UNKNOWN;
        }
        default:
            return DTYPE_UNKNOWN;
    }
}

int semantic_analyze(ASTNode *root) {
    semantic_error_count = 0;
    symtab_init();
    if (!root || root->type != NODE_PROGRAM) return -1;

    for (int i = 0; i < root->u.program.count; i++)
        analyze_stmt(root->u.program.stmts[i]);

    return semantic_error_count;
}
