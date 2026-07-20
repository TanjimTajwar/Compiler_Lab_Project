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
 * Bison parser for MiniLang with AST construction.
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "errors.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;

void yyerror(const char *s);

ASTNode *ast_root = NULL;
int parse_error_flag = 0;
%}

%define parse.error verbose
%locations

%union {
    int ival;
    char *sval;
    DataType dtype;
    ASTNode *node;
    StmtList *stmt_list;
    BinOpType bop;
    RelOpType rop;
    UnaryOpType uop;
}

%token INT BOOL IF ELSE WHILE PRINT
%token <sval> IDENTIFIER
%token <ival> INTEGER
%token LE GE EQ NE LT GT
%token PLUS MINUS STAR SLASH NOT
%token ASSIGN SEMICOLON COMMA
%token LPAREN RPAREN LBRACE RBRACE

%type <stmt_list> program statement_list
%type <node> statement block_statement
%type <node> if_statement while_statement print_statement
%type <node> declaration assignment_statement
%type <dtype> type_spec
%type <node> expression
%type <node> rel_expr add_expr mul_expr unary_expr primary_expr
%type <bop> add_op mul_op
%type <rop> rel_op

%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%right NOT UMINUS

%%

program
    : statement_list
        {
            ast_root = ast_program($1, @1.first_line);
        }
    ;

statement_list
    : statement_list statement
        {
            $$ = $1;
            stmt_list_append($$, $2);
        }
    | /* empty */
        {
            $$ = stmt_list_create();
        }
    ;

statement
    : declaration           { $$ = $1; }
    | assignment_statement  { $$ = $1; }
    | if_statement          { $$ = $1; }
    | while_statement       { $$ = $1; }
    | print_statement       { $$ = $1; }
    | block_statement       { $$ = $1; }
    ;

declaration
    : type_spec IDENTIFIER SEMICOLON
        {
            $$ = ast_decl($2, $1, @1.first_line);
        }
    ;

type_spec
    : INT  { $$ = TYPE_INT; }
    | BOOL { $$ = TYPE_BOOL; }
    ;

assignment_statement
    : IDENTIFIER ASSIGN expression SEMICOLON
        {
            $$ = ast_assign($1, $3, @1.first_line);
        }
    ;

if_statement
    : IF LPAREN expression RPAREN statement
        {
            $$ = ast_if($3, $5, NULL, @1.first_line);
        }
    | IF LPAREN expression RPAREN statement ELSE statement
        {
            $$ = ast_if($3, $5, $7, @1.first_line);
        }
    ;

while_statement
    : WHILE LPAREN expression RPAREN statement
        {
            $$ = ast_while($3, $5, @1.first_line);
        }
    ;

print_statement
    : PRINT LPAREN expression RPAREN SEMICOLON
        {
            $$ = ast_create_print($3, @1.first_line);
        }
    ;

block_statement
    : LBRACE statement_list RBRACE
        {
            $$ = ast_block($2, @1.first_line);
        }
    ;

expression
    : rel_expr { $$ = $1; }
    ;

rel_expr
    : add_expr { $$ = $1; }
    | rel_expr rel_op add_expr
        {
            $$ = ast_relop($2, $1, $3, @2.first_line);
        }
    ;

rel_op
    : LT { $$ = REL_LT; }
    | GT { $$ = REL_GT; }
    | LE { $$ = REL_LE; }
    | GE { $$ = REL_GE; }
    | EQ { $$ = REL_EQ; }
    | NE { $$ = REL_NE; }
    ;

add_expr
    : mul_expr { $$ = $1; }
    | add_expr add_op mul_expr
        {
            $$ = ast_binop($2, $1, $3, @2.first_line);
        }
    ;

add_op
    : PLUS { $$ = OP_ADD; }
    | MINUS { $$ = OP_SUB; }
    ;

mul_expr
    : unary_expr { $$ = $1; }
    | mul_expr mul_op unary_expr
        {
            $$ = ast_binop($2, $1, $3, @2.first_line);
        }
    ;

mul_op
    : STAR  { $$ = OP_MUL; }
    | SLASH { $$ = OP_DIV; }
    ;

unary_expr
    : primary_expr { $$ = $1; }
    | MINUS unary_expr %prec UMINUS
        {
            $$ = ast_unary(UNOP_NEG, $2, @1.first_line);
        }
    | NOT unary_expr
        {
            $$ = ast_unary(UNOP_NOT, $2, @1.first_line);
        }
    ;

primary_expr
    : IDENTIFIER
        {
            $$ = ast_ident($1, @1.first_line);
        }
    | INTEGER
        {
            $$ = ast_number($1, @1.first_line);
        }
    | LPAREN expression RPAREN
        {
            $$ = $2;
        }
    ;

%%

void yyerror(const char *s) {
    error_report(ERR_SYNTAX, yylineno, "%s", s);
    parse_error_flag = 1;
}
