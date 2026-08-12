// Done by Muznabin Ahmed

/*
 * Team Members:
 * - Tanjim Tajwar Arnab (22701066)
 * - Hafiz Hasnat Sifat Jami (22701068)
 * - Muznabin Ahmed (22701069)
 * - Monir Hossain (21701009)
 *
 * Primary Contributor:
 * Muznabin Ahmed
 *
 * Contributors:
 * Tanjim Tajwar Arnab
 * Hafiz Hasnat Sifat Jami
 * Monir Hossain
 *
 * Abstract Syntax Tree construction and traversal.
 */

#include "ast.h"
#include <stdlib.h>
#include <string.h>

StmtList *stmt_list_create(void) {
    StmtList *list = (StmtList *)calloc(1, sizeof(StmtList));
    if (!list) return NULL;
    list->capacity = 8;
    list->items = (ASTNode **)calloc((size_t)list->capacity, sizeof(ASTNode *));
    return list;
}

void stmt_list_append(StmtList *list, ASTNode *node) {
    if (!list || !node) return;
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->items = (ASTNode **)realloc(list->items,
                                           (size_t)list->capacity * sizeof(ASTNode *));
    }
    list->items[list->count++] = node;
}

void stmt_list_free(StmtList *list) {
    int i;
    if (!list) return;
    for (i = 0; i < list->count; i++) {
        ast_free(list->items[i]);
    }
    free(list->items);
    free(list);
}

static ASTNode *ast_alloc(NodeType type, int line) {
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (node) {
        node->type = type;
        node->line = line;
        node->data_type = TYPE_UNKNOWN;
    }
    return node;
}

ASTNode *ast_program(StmtList *statements, int line) {
    ASTNode *node = ast_alloc(NODE_PROGRAM, line);
    if (node) node->u.program.statements = statements;
    return node;
}

ASTNode *ast_decl(char *name, DataType var_type, int line) {
    ASTNode *node = ast_alloc(NODE_DECL, line);
    if (node) {
        node->u.decl.name = name;
        node->u.decl.var_type = var_type;
        node->data_type = var_type;
    }
    return node;
}

ASTNode *ast_assign(char *name, ASTNode *expr, int line) {
    ASTNode *node = ast_alloc(NODE_ASSIGN, line);
    if (node) {
        node->u.assign.name = name;
        node->u.assign.expr = expr;
    }
    return node;
}

ASTNode *ast_if(ASTNode *condition, ASTNode *then_branch,
                ASTNode *else_branch, int line) {
    ASTNode *node = ast_alloc(NODE_IF, line);
    if (node) {
        node->u.if_stmt.condition = condition;
        node->u.if_stmt.then_branch = then_branch;
        node->u.if_stmt.else_branch = else_branch;
    }
    return node;
}

ASTNode *ast_while(ASTNode *condition, ASTNode *body, int line) {
    ASTNode *node = ast_alloc(NODE_WHILE, line);
    if (node) {
        node->u.while_stmt.condition = condition;
        node->u.while_stmt.body = body;
    }
    return node;
}

ASTNode *ast_create_print(ASTNode *expr, int line) {
    ASTNode *node = ast_alloc(NODE_PRINT, line);
    if (node) {
        node->u.print_stmt.expr = expr;
    }
    return node;
}

ASTNode *ast_block(StmtList *statements, int line) {
    ASTNode *node = ast_alloc(NODE_BLOCK, line);
    if (node) node->u.block.statements = statements;
    return node;
}

ASTNode *ast_ident(char *name, int line) {
    ASTNode *node = ast_alloc(NODE_IDENT, line);
    if (node) node->u.ident.name = name;
    return node;
}

ASTNode *ast_number(int value, int line) {
    ASTNode *node = ast_alloc(NODE_NUMBER, line);
    if (node) {
        node->u.number.value = value;
        node->data_type = TYPE_INT;
    }
    return node;
}

ASTNode *ast_binop(BinOpType op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = ast_alloc(NODE_BINOP, line);
    if (node) {
        node->u.binop.op = op;
        node->u.binop.left = left;
        node->u.binop.right = right;
        node->data_type = TYPE_INT;
    }
    return node;
}

ASTNode *ast_relop(RelOpType op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = ast_alloc(NODE_RELOP, line);
    if (node) {
        node->u.relop.op = op;
        node->u.relop.left = left;
        node->u.relop.right = right;
        node->data_type = TYPE_BOOL;
    }
    return node;
}

ASTNode *ast_unary(UnaryOpType op, ASTNode *operand, int line) {
    ASTNode *node = ast_alloc(NODE_UNARY, line);
    if (node) {
        node->u.unary.op = op;
        node->u.unary.operand = operand;
        if (op == UNOP_NOT) {
            node->data_type = TYPE_BOOL;
        } else if (operand && operand->data_type != TYPE_UNKNOWN) {
            node->data_type = operand->data_type;
        } else {
            node->data_type = TYPE_INT;
        }
    }
    return node;
}

ASTNode *ast_stmt_list(StmtList *statements, int line) {
    ASTNode *node = ast_alloc(NODE_STMT_LIST, line);
    if (node) node->u.stmt_list.statements = statements;
    return node;
}

const char *data_type_to_string(DataType type) {
    switch (type) {
        case TYPE_INT:  return "int";
        case TYPE_BOOL: return "bool";
        default:        return "unknown";
    }
}

const char *binop_to_string(BinOpType op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        default:     return "?";
    }
}

const char *relop_to_string(RelOpType op) {
    switch (op) {
        case REL_LT: return "<";
        case REL_GT: return ">";
        case REL_LE: return "<=";
        case REL_GE: return ">=";
        case REL_EQ: return "==";
        case REL_NE: return "!=";
        default:     return "?";
    }
}

static void print_indent(int indent) {
    int i;
    for (i = 0; i < indent; i++) printf("  ");
}

static void ast_print_stmt_list(StmtList *list, int indent);

void ast_print(ASTNode *node, int indent) {
    if (!node) return;

    print_indent(indent);

    switch (node->type) {
        case NODE_PROGRAM:
            printf("PROGRAM (line %d)\n", node->line);
            ast_print_stmt_list(node->u.program.statements, indent + 1);
            break;

        case NODE_DECL:
            printf("DECL %s : %s (line %d)\n",
                   node->u.decl.name,
                   data_type_to_string(node->u.decl.var_type),
                   node->line);
            break;

        case NODE_ASSIGN:
            printf("ASSIGN %s (line %d)\n", node->u.assign.name, node->line);
            ast_print(node->u.assign.expr, indent + 1);
            break;

        case NODE_IF:
            printf("IF (line %d)\n", node->line);
            ast_print(node->u.if_stmt.condition, indent + 1);
            ast_print(node->u.if_stmt.then_branch, indent + 1);
            if (node->u.if_stmt.else_branch) {
                print_indent(indent + 1);
                printf("ELSE\n");
                ast_print(node->u.if_stmt.else_branch, indent + 2);
            }
            break;

        case NODE_WHILE:
            printf("WHILE (line %d)\n", node->line);
            ast_print(node->u.while_stmt.condition, indent + 1);
            ast_print(node->u.while_stmt.body, indent + 1);
            break;

        case NODE_PRINT:
            printf("PRINT (line %d)\n", node->line);
            ast_print(node->u.print_stmt.expr, indent + 1);
            break;

        case NODE_BLOCK:
            printf("BLOCK (line %d)\n", node->line);
            ast_print_stmt_list(node->u.block.statements, indent + 1);
            break;

        case NODE_IDENT:
            printf("IDENT %s (line %d)\n", node->u.ident.name, node->line);
            break;

        case NODE_NUMBER:
            printf("NUMBER %d (line %d)\n", node->u.number.value, node->line);
            break;

        case NODE_BINOP:
            printf("BINOP %s (line %d)\n", binop_to_string(node->u.binop.op), node->line);
            ast_print(node->u.binop.left, indent + 1);
            ast_print(node->u.binop.right, indent + 1);
            break;

        case NODE_RELOP:
            printf("RELOP %s (line %d)\n", relop_to_string(node->u.relop.op), node->line);
            ast_print(node->u.relop.left, indent + 1);
            ast_print(node->u.relop.right, indent + 1);
            break;

        case NODE_UNARY:
            printf("UNARY %s (line %d)\n",
                   node->u.unary.op == UNOP_NEG ? "-" : "!", node->line);
            ast_print(node->u.unary.operand, indent + 1);
            break;

        case NODE_STMT_LIST:
            ast_print_stmt_list(node->u.stmt_list.statements, indent);
            break;

        default:
            printf("UNKNOWN NODE (line %d)\n", node->line);
            break;
    }
}

static void ast_print_stmt_list(StmtList *list, int indent) {
    int i;
    if (!list) return;
    for (i = 0; i < list->count; i++) {
        ast_print(list->items[i], indent);
    }
}

void ast_free(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            stmt_list_free(node->u.program.statements);
            break;
        case NODE_DECL:
            free(node->u.decl.name);
            break;
        case NODE_ASSIGN:
            free(node->u.assign.name);
            ast_free(node->u.assign.expr);
            break;
        case NODE_IF:
            ast_free(node->u.if_stmt.condition);
            ast_free(node->u.if_stmt.then_branch);
            ast_free(node->u.if_stmt.else_branch);
            break;
        case NODE_WHILE:
            ast_free(node->u.while_stmt.condition);
            ast_free(node->u.while_stmt.body);
            break;
        case NODE_PRINT:
            ast_free(node->u.print_stmt.expr);
            break;
        case NODE_BLOCK:
            stmt_list_free(node->u.block.statements);
            break;
        case NODE_IDENT:
            free(node->u.ident.name);
            break;
        case NODE_BINOP:
            ast_free(node->u.binop.left);
            ast_free(node->u.binop.right);
            break;
        case NODE_RELOP:
            ast_free(node->u.relop.left);
            ast_free(node->u.relop.right);
            break;
        case NODE_UNARY:
            ast_free(node->u.unary.operand);
            break;
        case NODE_STMT_LIST:
            stmt_list_free(node->u.stmt_list.statements);
            break;
        default:
            break;
    }
    free(node);
}

TACList *tac_list_create(void) {
    return (TACList *)calloc(1, sizeof(TACList));
}

TAC *tac_emit(TACList *list, const char *op, const char *arg1,
              const char *arg2, const char *result) {
    TAC *instr;

    if (!list) return NULL;

    instr = (TAC *)calloc(1, sizeof(TAC));
    if (!instr) return NULL;

    if (op)     strncpy(instr->op, op, sizeof(instr->op) - 1);
    if (arg1)   strncpy(instr->arg1, arg1, sizeof(instr->arg1) - 1);
    if (arg2)   strncpy(instr->arg2, arg2, sizeof(instr->arg2) - 1);
    if (result) strncpy(instr->result, result, sizeof(instr->result) - 1);

    if (!list->head) {
        list->head = instr;
        list->tail = instr;
    } else {
        list->tail->next = instr;
        list->tail = instr;
    }
    list->count++;
    return instr;
}

void tac_list_free(TACList *list) {
    TAC *cur;
    TAC *next;
    if (!list) return;
    cur = list->head;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

void tac_print(TACList *list, FILE *out) {
    TAC *cur;
    if (!list || !out) return;

    for (cur = list->head; cur; cur = cur->next) {
        if (strcmp(cur->op, "label") == 0) {
            fprintf(out, "%s:\n", cur->result);
            continue;
        }
        if (strcmp(cur->op, "goto") == 0) {
            fprintf(out, "goto %s\n", cur->result);
            continue;
        }
        if (strcmp(cur->op, "ifFalse") == 0) {
            fprintf(out, "ifFalse %s goto %s\n", cur->arg1, cur->result);
            continue;
        }
        if (strcmp(cur->op, "ifTrue") == 0) {
            fprintf(out, "ifTrue %s goto %s\n", cur->arg1, cur->result);
            continue;
        }
        if (strcmp(cur->op, "print") == 0) {
            fprintf(out, "print %s\n", cur->arg1);
            continue;
        }
        if (cur->arg2[0] != '\0') {
            fprintf(out, "%s = %s %s %s\n",
                    cur->result, cur->arg1, cur->op, cur->arg2);
        } else if (cur->arg1[0] != '\0') {
            fprintf(out, "%s = %s\n", cur->result, cur->arg1);
        } else {
            fprintf(out, "%s\n", cur->op);
        }
    }
}
