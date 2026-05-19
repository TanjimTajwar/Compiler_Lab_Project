/*
 * File: parser.y
 * Developed by: Hafiz Hasnat Sifat Jami (ID: 22701068)
 * Contribution: Syntax Analysis, Grammar Rules, AST Construction
 *
 * Complexity: O(n) for n tokens (LALR parse); AST build O(n).
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylineno;
extern int yylex(void);
extern int yydebug;
extern int syntax_error_count;

void yyerror(const char *s);
ASTNode *g_ast_root = NULL;

static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}
%}

/* win_bison / classic bison: use yylineno in actions (no %locations) */

%union {
    int ival;
    char *sval;
    ASTNode *node;
    NodeList nlist;
}

%token INT BOOL IF ELSE WHILE PRINT
%token <ival> TRUE_LIT FALSE_LIT INT_LIT
%token <sval> IDENT
%token PLUS MINUS STAR SLASH LT GT EQ NE AND OR NOT
%token ASSIGN SEMI LPAREN RPAREN LBRACE RBRACE

%type <node> program stmt decl assign if_stmt while_stmt print_stmt block expr
%type <nlist> stmt_list stmt_list_nonempty
%type <ival> type_spec

%left OR
%left AND
%left EQ NE
%left LT GT
%left PLUS MINUS
%left STAR SLASH
%right NOT UMINUS

%start program
%expect 1

%%

program
    : stmt_list { g_ast_root = ast_program($1.nlist, $1.count, yylineno); $$ = g_ast_root; }
    ;

stmt_list
    : stmt_list_nonempty { $$.nlist = $1.nlist; $$.count = $1.count; }
    | /* empty */       { $$.nlist = NULL; $$.count = 0; }
    ;

stmt_list_nonempty
    : stmt_list_nonempty stmt
        {
            $$.count = $1.count + 1;
            $$.nlist = (ASTNode **)realloc($1.nlist, sizeof(ASTNode *) * $$.count);
            $$.nlist[$$.count - 1] = $2;
        }
    | stmt
        {
            $$.count = 1;
            $$.nlist = (ASTNode **)malloc(sizeof(ASTNode *));
            $$.nlist[0] = $1;
        }
    ;

stmt
    : decl       { $$ = $1; }
    | assign     { $$ = $1; }
    | if_stmt    { $$ = $1; }
    | while_stmt { $$ = $1; }
    | print_stmt { $$ = $1; }
    | block      { $$ = $1; }
    ;

decl
    : type_spec IDENT SEMI
        { $$ = ast_decl(dup_str($2), (DataType)$1, yylineno); free($2); }
    ;

type_spec
    : INT  { $$ = DTYPE_INT; }
    | BOOL { $$ = DTYPE_BOOL; }
    ;

assign
    : IDENT ASSIGN expr SEMI
        { $$ = ast_assign(dup_str($1), $3, yylineno); free($1); }
    ;

if_stmt
    : IF LPAREN expr RPAREN stmt ELSE stmt
        { $$ = ast_if($3, $5, $7, yylineno); }
    | IF LPAREN expr RPAREN stmt
        { $$ = ast_if($3, $5, NULL, yylineno); }
    ;

while_stmt
    : WHILE LPAREN expr RPAREN stmt
        { $$ = ast_while($3, $5, yylineno); }
    ;

print_stmt
    : PRINT LPAREN expr RPAREN SEMI
        { $$ = ast_print($3, yylineno); }
    ;

block
    : LBRACE stmt_list RBRACE
        { $$ = ast_block($2.nlist, $2.count, yylineno); }
    ;

expr
    : INT_LIT
        { $$ = ast_int($1, yylineno); }
    | TRUE_LIT
        { $$ = ast_bool($1, yylineno); }
    | FALSE_LIT
        { $$ = ast_bool($1, yylineno); }
    | IDENT
        { $$ = ast_id(dup_str($1), yylineno); free($1); }
    | expr PLUS expr
        { $$ = ast_binop(OP_ADD, $1, $3, yylineno); }
    | expr MINUS expr
        { $$ = ast_binop(OP_SUB, $1, $3, yylineno); }
    | expr STAR expr
        { $$ = ast_binop(OP_MUL, $1, $3, yylineno); }
    | expr SLASH expr
        { $$ = ast_binop(OP_DIV, $1, $3, yylineno); }
    | expr LT expr
        { $$ = ast_binop(OP_LT, $1, $3, yylineno); }
    | expr GT expr
        { $$ = ast_binop(OP_GT, $1, $3, yylineno); }
    | expr EQ expr
        { $$ = ast_binop(OP_EQ, $1, $3, yylineno); }
    | expr NE expr
        { $$ = ast_binop(OP_NE, $1, $3, yylineno); }
    | expr AND expr
        { $$ = ast_binop(OP_AND, $1, $3, yylineno); }
    | expr OR expr
        { $$ = ast_binop(OP_OR, $1, $3, yylineno); }
    | NOT expr %prec NOT
        { $$ = ast_unop(OP_NOT, $2, yylineno); }
    | MINUS expr %prec UMINUS
        { $$ = ast_unop(OP_NEG, $2, yylineno); }
    | LPAREN expr RPAREN
        { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
    fprintf(stderr, "  Suggestion: check semicolons, braces, and parentheses.\n");
    syntax_error_count++;
}
