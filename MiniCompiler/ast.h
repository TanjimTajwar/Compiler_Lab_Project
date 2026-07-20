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
 * Abstract Syntax Tree definitions and traversal interface.
 */

#ifndef AST_H
#define AST_H

#include <stdio.h>

/* Data types supported by MiniLang */
typedef enum {
    TYPE_INT,
    TYPE_BOOL,
    TYPE_UNKNOWN
} DataType;

/* AST node kinds */
typedef enum {
    NODE_PROGRAM,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BLOCK,
    NODE_IDENT,
    NODE_NUMBER,
    NODE_BINOP,
    NODE_RELOP,
    NODE_UNARY,
    NODE_STMT_LIST
} NodeType;

/* Binary operators */
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV
} BinOpType;

/* Relational operators */
typedef enum {
    REL_LT,
    REL_GT,
    REL_LE,
    REL_GE,
    REL_EQ,
    REL_NE
} RelOpType;

/* Unary operators */
typedef enum {
    UNOP_NEG,
    UNOP_NOT
} UnaryOpType;

/* Forward declaration */
struct ASTNode;

/* Statement list container */
typedef struct StmtList {
    struct ASTNode **items;
    int count;
    int capacity;
} StmtList;

/* AST node structure */
typedef struct ASTNode {
    NodeType type;
    int line;
    DataType data_type;

    union {
        struct {
            StmtList *statements;
        } program;

        struct {
            char *name;
            DataType var_type;
        } decl;

        struct {
            char *name;
            struct ASTNode *expr;
        } assign;

        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;

        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_stmt;

        struct {
            struct ASTNode *expr;
        } print_stmt;

        struct {
            StmtList *statements;
        } block;

        struct {
            char *name;
        } ident;

        struct {
            int value;
        } number;

        struct {
            BinOpType op;
            struct ASTNode *left;
            struct ASTNode *right;
        } binop;

        struct {
            RelOpType op;
            struct ASTNode *left;
            struct ASTNode *right;
        } relop;

        struct {
            UnaryOpType op;
            struct ASTNode *operand;
        } unary;

        struct {
            StmtList *statements;
        } stmt_list;
    } u;
} ASTNode;

/* Three-address code instruction */
typedef struct TAC {
    char op[16];
    char arg1[64];
    char arg2[64];
    char result[64];
    struct TAC *next;
} TAC;

/* TAC list container */
typedef struct TACList {
    TAC *head;
    TAC *tail;
    int count;
} TACList;

/* AST construction */
StmtList *stmt_list_create(void);
void stmt_list_append(StmtList *list, ASTNode *node);
ASTNode *ast_program(StmtList *statements, int line);
ASTNode *ast_decl(char *name, DataType var_type, int line);
ASTNode *ast_assign(char *name, ASTNode *expr, int line);
ASTNode *ast_if(ASTNode *condition, ASTNode *then_branch, ASTNode *else_branch, int line);
ASTNode *ast_while(ASTNode *condition, ASTNode *body, int line);
ASTNode *ast_create_print(ASTNode *expr, int line);
ASTNode *ast_block(StmtList *statements, int line);
ASTNode *ast_ident(char *name, int line);
ASTNode *ast_number(int value, int line);
ASTNode *ast_binop(BinOpType op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_relop(RelOpType op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_unary(UnaryOpType op, ASTNode *operand, int line);
ASTNode *ast_stmt_list(StmtList *statements, int line);

/* AST traversal and cleanup */
void ast_print(ASTNode *node, int indent);
void ast_free(ASTNode *node);
void stmt_list_free(StmtList *list);

const char *data_type_to_string(DataType type);
const char *binop_to_string(BinOpType op);
const char *relop_to_string(RelOpType op);

/* TAC list helpers */
TACList *tac_list_create(void);
TAC *tac_emit(TACList *list, const char *op, const char *arg1,
              const char *arg2, const char *result);
void tac_list_free(TACList *list);
void tac_print(TACList *list, FILE *out);

#endif /* AST_H */
