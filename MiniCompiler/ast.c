/*
 * File: ast.c
 * Developed by: Tanjim Tajwar Arnab (ID: 22701066)
 * Contribution: AST Construction, Pretty-Print, Memory Management
 *
 * Complexity: Each constructor O(1); print_ast/free_ast O(n) nodes.
 */

#include "ast.h"
#include <stdlib.h>
#include <string.h>

int semantic_error_count = 0;
int syntax_error_count = 0;

static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

static ASTNode *mknode(NodeType type, int line) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    n->type = type;
    n->line = line;
    n->dtype = DTYPE_UNKNOWN;
    return n;
}

ASTNode *ast_program(ASTNode **stmts, int count, int line) {
    ASTNode *n = mknode(NODE_PROGRAM, line);
    n->u.program.stmts = stmts;
    n->u.program.count = count;
    return n;
}

ASTNode *ast_decl(char *name, DataType t, int line) {
    ASTNode *n = mknode(NODE_DECL, line);
    n->u.decl.name = name;
    n->u.decl.decl_type = t;
    n->dtype = t;
    return n;
}

ASTNode *ast_assign(char *name, ASTNode *val, int line) {
    ASTNode *n = mknode(NODE_ASSIGN, line);
    n->u.assign.name = name;
    n->u.assign.value = val;
    return n;
}

ASTNode *ast_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line) {
    ASTNode *n = mknode(NODE_IF, line);
    n->u.if_stmt.cond = cond;
    n->u.if_stmt.then_br = then_b;
    n->u.if_stmt.else_br = else_b;
    return n;
}

ASTNode *ast_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = mknode(NODE_WHILE, line);
    n->u.while_stmt.cond = cond;
    n->u.while_stmt.body = body;
    return n;
}

ASTNode *ast_print(ASTNode *expr, int line) {
    ASTNode *n = mknode(NODE_PRINT, line);
    n->u.print_stmt.expr = expr;
    return n;
}

ASTNode *ast_block(ASTNode **stmts, int count, int line) {
    ASTNode *n = mknode(NODE_BLOCK, line);
    n->u.block.stmts = stmts;
    n->u.block.count = count;
    return n;
}

ASTNode *ast_binop(BinOpKind op, ASTNode *l, ASTNode *r, int line) {
    ASTNode *n = mknode(NODE_BINOP, line);
    n->u.binop.op = op;
    n->u.binop.left = l;
    n->u.binop.right = r;
    return n;
}

ASTNode *ast_unop(BinOpKind op, ASTNode *operand, int line) {
    ASTNode *n = mknode(NODE_UNOP, line);
    n->u.unop.op = op;
    n->u.unop.operand = operand;
    return n;
}

ASTNode *ast_int(int v, int line) {
    ASTNode *n = mknode(NODE_INT, line);
    n->u.intval.value = v;
    n->dtype = DTYPE_INT;
    return n;
}

ASTNode *ast_bool(int v, int line) {
    ASTNode *n = mknode(NODE_BOOL, line);
    n->u.boolval.value = v ? 1 : 0;
    n->dtype = DTYPE_BOOL;
    return n;
}

ASTNode *ast_id(char *name, int line) {
    ASTNode *n = mknode(NODE_ID, line);
    n->u.id.name = name;
    return n;
}

void ast_set_type(ASTNode *n, DataType t) {
    if (n) n->dtype = t;
}

static const char *dtype_str(DataType t) {
    switch (t) {
        case DTYPE_INT:  return "int";
        case DTYPE_BOOL: return "bool";
        default:         return "?";
    }
}

static const char *op_str(BinOpKind op) {
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
        case OP_NOT: return "!";
        case OP_NEG: return "-";
        default:     return "?";
    }
}

static void indent_print(int indent) {
    for (int i = 0; i < indent; i++) putchar(' ');
}

static void print_ast_rec(ASTNode *n, int indent) {
    if (!n) return;
    indent_print(indent);
    printf("[L%d] ", n->line);

    switch (n->type) {
        case NODE_PROGRAM:
            printf("PROGRAM (%d stmts)\n", n->u.program.count);
            for (int i = 0; i < n->u.program.count; i++)
                print_ast_rec(n->u.program.stmts[i], indent + 2);
            break;
        case NODE_DECL:
            printf("DECL %s : %s\n", n->u.decl.name, dtype_str(n->u.decl.decl_type));
            break;
        case NODE_ASSIGN:
            printf("ASSIGN %s\n", n->u.assign.name);
            print_ast_rec(n->u.assign.value, indent + 2);
            break;
        case NODE_IF:
            printf("IF\n");
            print_ast_rec(n->u.if_stmt.cond, indent + 2);
            indent_print(indent + 2); printf("THEN\n");
            print_ast_rec(n->u.if_stmt.then_br, indent + 4);
            if (n->u.if_stmt.else_br) {
                indent_print(indent + 2); printf("ELSE\n");
                print_ast_rec(n->u.if_stmt.else_br, indent + 4);
            }
            break;
        case NODE_WHILE:
            printf("WHILE\n");
            print_ast_rec(n->u.while_stmt.cond, indent + 2);
            print_ast_rec(n->u.while_stmt.body, indent + 2);
            break;
        case NODE_PRINT:
            printf("PRINT\n");
            print_ast_rec(n->u.print_stmt.expr, indent + 2);
            break;
        case NODE_BLOCK:
            printf("BLOCK (%d)\n", n->u.block.count);
            for (int i = 0; i < n->u.block.count; i++)
                print_ast_rec(n->u.block.stmts[i], indent + 2);
            break;
        case NODE_BINOP:
            printf("BINOP %s <%s>\n", op_str(n->u.binop.op), dtype_str(n->dtype));
            print_ast_rec(n->u.binop.left, indent + 2);
            print_ast_rec(n->u.binop.right, indent + 2);
            break;
        case NODE_UNOP:
            printf("UNOP %s <%s>\n", op_str(n->u.unop.op), dtype_str(n->dtype));
            print_ast_rec(n->u.unop.operand, indent + 2);
            break;
        case NODE_INT:
            printf("INT %d\n", n->u.intval.value);
            break;
        case NODE_BOOL:
            printf("BOOL %s\n", n->u.boolval.value ? "true" : "false");
            break;
        case NODE_ID:
            printf("ID %s <%s>\n", n->u.id.name, dtype_str(n->dtype));
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}

void print_ast(ASTNode *root, int indent) {
    if (!root) {
        printf("(empty AST)\n");
        return;
    }
    printf("=== Abstract Syntax Tree ===\n");
    print_ast_rec(root, indent);
    printf("============================\n");
}

static void free_stmt_list(ASTNode **stmts, int count) {
    for (int i = 0; i < count; i++) free_ast(stmts[i]);
    free(stmts);
}

void free_ast(ASTNode *n) {
    if (!n) return;
    switch (n->type) {
        case NODE_PROGRAM:
            free_stmt_list(n->u.program.stmts, n->u.program.count);
            break;
        case NODE_DECL:
            free(n->u.decl.name);
            break;
        case NODE_ASSIGN:
            free(n->u.assign.name);
            free_ast(n->u.assign.value);
            break;
        case NODE_IF:
            free_ast(n->u.if_stmt.cond);
            free_ast(n->u.if_stmt.then_br);
            if (n->u.if_stmt.else_br)
                free_ast(n->u.if_stmt.else_br);
            break;
        case NODE_WHILE:
            free_ast(n->u.while_stmt.cond);
            free_ast(n->u.while_stmt.body);
            break;
        case NODE_PRINT:
            free_ast(n->u.print_stmt.expr);
            break;
        case NODE_BLOCK:
            free_stmt_list(n->u.block.stmts, n->u.block.count);
            break;
        case NODE_BINOP:
            free_ast(n->u.binop.left);
            free_ast(n->u.binop.right);
            break;
        case NODE_UNOP:
            free_ast(n->u.unop.operand);
            break;
        case NODE_ID:
            free(n->u.id.name);
            break;
        default:
            break;
    }
    free(n);
}

/* ---------- TAC helpers ---------- */
TACList *tac_create(void) {
    TACList *t = (TACList *)calloc(1, sizeof(TACList));
    return t;
}

char *tac_new_temp(TACList *t) {
    char buf[32];
    snprintf(buf, sizeof(buf), "t%d", t->temp_count++);
    return strdup_safe(buf);
}

char *tac_new_label(TACList *t) {
    char buf[32];
    snprintf(buf, sizeof(buf), "L%d", t->label_count++);
    return strdup_safe(buf);
}

static TACInstr *mk_instr(int line, const char *op,
                          const char *a1, const char *a2, const char *res) {
    TACInstr *in = (TACInstr *)calloc(1, sizeof(TACInstr));
    in->line = line;
    in->op = strdup_safe(op);
    in->arg1 = strdup_safe(a1);
    in->arg2 = strdup_safe(a2);
    in->result = strdup_safe(res);
    return in;
}

void tac_emit(TACList *t, int line, const char *op,
              const char *a1, const char *a2, const char *res) {
    TACInstr *in = mk_instr(line, op, a1, a2, res);
    in->index = t->count++;
    if (!t->head) {
        t->head = t->tail = in;
    } else {
        t->tail->next = in;
        t->tail = in;
    }
}

void tac_emit_label(TACList *t, int line, const char *label) {
    TACInstr *in = mk_instr(line, "LABEL", NULL, NULL, label);
    in->is_label = 1;
    in->index = t->count++;
    if (!t->head) t->head = t->tail = in;
    else { t->tail->next = in; t->tail = in; }
}

void tac_free(TACList *t) {
    if (!t) return;
    TACInstr *cur = t->head;
    while (cur) {
        TACInstr *n = cur->next;
        free(cur->op);
        free(cur->arg1);
        free(cur->arg2);
        free(cur->result);
        free(cur);
        cur = n;
    }
    free(t);
}

void tac_print(FILE *fp, TACList *t) {
    if (!t) return;
    fprintf(fp, "; Three Address Code (MiniLang Compiler)\n");
    for (TACInstr *p = t->head; p; p = p->next) {
        if (p->dead) continue;
        if (p->is_label) {
            fprintf(fp, "%s:\n", p->result ? p->result : "");
            continue;
        }
        fprintf(fp, "%d: ", p->index);
        if (p->op && !strcmp(p->op, "=") && p->result && p->arg1) {
            fprintf(fp, "%s = %s\n", p->result, p->arg1);
            continue;
        }
        if (p->op && !strcmp(p->op, "goto") && p->result) {
            fprintf(fp, "goto %s\n", p->result);
            continue;
        }
        if (p->op && !strcmp(p->op, "if_false") && p->arg1 && p->result) {
            fprintf(fp, "if_false %s goto %s\n", p->arg1, p->result);
            continue;
        }
        if (p->op && !strcmp(p->op, "param") && p->arg1) {
            fprintf(fp, "param %s\n", p->arg1);
            continue;
        }
        if (p->op && !strcmp(p->op, "call")) {
            fprintf(fp, "call %s", p->arg1 ? p->arg1 : "");
            if (p->arg2) fprintf(fp, ", %s", p->arg2);
            fprintf(fp, "\n");
            continue;
        }
        if (p->op && !strcmp(p->op, "halt")) {
            fprintf(fp, "halt\n");
            continue;
        }
        if (p->result && p->result[0])
            fprintf(fp, "%s = ", p->result);
        fprintf(fp, "%s", p->op ? p->op : "");
        if (p->arg1 && p->arg1[0]) fprintf(fp, " %s", p->arg1);
        if (p->arg2 && p->arg2[0]) fprintf(fp, ", %s", p->arg2);
        fprintf(fp, "\n");
    }
}

int tac_save(const char *path, TACList *t) {
    FILE *fp = fopen(path, "w");
    if (!fp) {
        perror("output.tac");
        return -1;
    }
    tac_print(fp, t);
    fclose(fp);
    return 0;
}
