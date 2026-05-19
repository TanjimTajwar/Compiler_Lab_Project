/*
 * File: ast.h
 * Developed by: Tanjim Tajwar Arnab (ID: 22701066)
 * Contribution: AST Design, Core Data Structures, Integration Headers
 *
 * Complexity: AST node creation O(1) per node; pretty-print O(n) for n nodes;
 *             free_ast O(n).
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>

/* ---------- Types ---------- */
typedef enum { DTYPE_INT, DTYPE_BOOL, DTYPE_UNKNOWN } DataType;

typedef enum {
    NODE_PROGRAM,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BLOCK,
    NODE_BINOP,
    NODE_UNOP,
    NODE_INT,
    NODE_BOOL,
    NODE_ID
} NodeType;

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_LT, OP_GT, OP_EQ, OP_NE,
    OP_AND, OP_OR, OP_NOT,
    OP_NEG
} BinOpKind;

/* ---------- AST Node ---------- */
typedef struct {
    struct ASTNode **nlist;
    int count;
} NodeList;

typedef struct ASTNode {
    NodeType type;
    int line;
    DataType dtype;

    union {
        struct { struct ASTNode **stmts; int count; } program;
        struct { char *name; DataType decl_type; } decl;
        struct { char *name; struct ASTNode *value; } assign;
        struct {
            struct ASTNode *cond;
            struct ASTNode *then_br;
            struct ASTNode *else_br;
        } if_stmt;
        struct { struct ASTNode *cond; struct ASTNode *body; } while_stmt;
        struct { struct ASTNode *expr; } print_stmt;
        struct { struct ASTNode **stmts; int count; } block;
        struct {
            BinOpKind op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binop;
        struct { BinOpKind op; struct ASTNode *operand; } unop;
        struct { int value; } intval;
        struct { int value; } boolval;
        struct { char *name; } id;
    } u;
} ASTNode;

/* ---------- TAC (Three Address Code) ---------- */
typedef struct TACInstr {
    int index;
    int line;
    char *op;
    char *arg1;
    char *arg2;
    char *result;
    int is_label;       /* 1 if result holds label name */
    int dead;           /* marked by optimizer */
    struct TACInstr *next;
} TACInstr;

typedef struct {
    TACInstr *head;
    TACInstr *tail;
    int count;
    int temp_count;
    int label_count;
} TACList;

/* ---------- Symbol Table (API used across modules) ---------- */
typedef struct Symbol {
    char *name;
    DataType type;
    int scope_level;
    int line_declared;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *symbols;
    int level;
    struct Scope *parent;
} Scope;

/* ---------- Global flags ---------- */
extern int semantic_error_count;
extern int syntax_error_count;
extern ASTNode *g_ast_root;

/* ---------- AST API ---------- */
ASTNode *ast_program(ASTNode **stmts, int count, int line);
ASTNode *ast_decl(char *name, DataType t, int line);
ASTNode *ast_assign(char *name, ASTNode *val, int line);
ASTNode *ast_if(ASTNode *cond, ASTNode *then_b, ASTNode *else_b, int line);
ASTNode *ast_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_print(ASTNode *expr, int line);
ASTNode *ast_block(ASTNode **stmts, int count, int line);
ASTNode *ast_binop(BinOpKind op, ASTNode *l, ASTNode *r, int line);
ASTNode *ast_unop(BinOpKind op, ASTNode *operand, int line);
ASTNode *ast_int(int v, int line);
ASTNode *ast_bool(int v, int line);
ASTNode *ast_id(char *name, int line);

void ast_set_type(ASTNode *n, DataType t);
void print_ast(ASTNode *root, int indent);
void free_ast(ASTNode *root);

/* ---------- TAC API ---------- */
TACList *tac_create(void);
char *tac_new_temp(TACList *t);
char *tac_new_label(TACList *t);
void tac_emit(TACList *t, int line, const char *op,
              const char *a1, const char *a2, const char *res);
void tac_emit_label(TACList *t, int line, const char *label);
void tac_free(TACList *t);
void tac_print(FILE *fp, TACList *t);
int tac_save(const char *path, TACList *t);

/* ---------- Symbol Table API ---------- */
void symtab_init(void);
void symtab_enter_scope(void);
void symtab_exit_scope(void);
int symtab_insert(char *name, DataType type, int line);
Symbol *symtab_lookup(char *name);
Symbol *symtab_lookup_current(char *name);
void symtab_print(void);
void symtab_free(void);

/* ---------- Phase APIs ---------- */
int semantic_analyze(ASTNode *root);
TACList *codegen_generate(ASTNode *root);
TACList *optimize_tac(TACList *in);
int target_generate(TACList *tac, const char *path);

#endif /* AST_H */
