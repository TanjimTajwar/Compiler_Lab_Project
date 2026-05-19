/*
 * File: lexer.l
 * Developed by: Sabrina Sultana (ID: 22701067)
 * Contribution: Lexical Analysis, Token Definitions, Line Tracking
 *
 * Complexity: O(n) over input length n (single pass DFA scan).
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

extern int syntax_error_count;
extern YYSTYPE yylval;
extern int yylineno;

static char *strdup_lex(const char *s) {
    size_t n = strlen(s) + 1;
    char *d = (char *)malloc(n);
    if (d) memcpy(d, s, n);
    return d;
}

static void lex_error(const char *msg) {
    fprintf(stderr, "Lexical error at line %d: %s\n", yylineno, msg);
    fprintf(stderr, "  Suggestion: check for invalid characters or unclosed strings.\n");
    syntax_error_count++;
}
%}

%option noyywrap
%option yylineno
%option nounput
%option noinput

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*

%%

[ \t\r]+             ;   /* skip whitespace */
\n                   ;   /* lineno handled by flex */

"//".*               ;   /* single-line comment */
"/\*"([^*]|\*+[^*/])*\*+"/" ;  /* block comment */

"int"                { return INT; }
"bool"               { return BOOL; }
"if"                 { return IF; }
"else"               { return ELSE; }
"while"              { return WHILE; }
"print"              { return PRINT; }
"true"               { yylval.ival = 1; return TRUE_LIT; }
"false"              { yylval.ival = 0; return FALSE_LIT; }

"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return STAR; }
"/"                  { return SLASH; }
"<"                  { return LT; }
">"                  { return GT; }
"=="                 { return EQ; }
"!="                 { return NE; }
"&&"                 { return AND; }
"||"                 { return OR; }
"!"                  { return NOT; }

"="                  { return ASSIGN; }
";"                  { return SEMI; }
"("                  { return LPAREN; }
")"                  { return RPAREN; }
"{"                  { return LBRACE; }
"}"                  { return RBRACE; }

{ID}                 { yylval.sval = strdup_lex(yytext); return IDENT; }
{DIGIT}+             { yylval.ival = atoi(yytext); return INT_LIT; }

.                    { char buf[128];
                       snprintf(buf, sizeof(buf), "unexpected character '%s'", yytext);
                       lex_error(buf); }

%%
