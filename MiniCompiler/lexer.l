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
 * Flex lexical analyzer for MiniLang.
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "errors.h"
#include "parser.tab.h"

extern YYSTYPE yylval;
extern int parse_error_flag;

static char *strdup_safe(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy) memcpy(copy, s, len);
    return copy;
}
%}

%option yylineno
%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INTEGER     {DIGIT}+

%%

"int"           { return INT; }
"bool"          { return BOOL; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"print"         { return PRINT; }

{ID}            {
                    yylval.sval = strdup_safe(yytext);
                    return IDENTIFIER;
                }

{INTEGER}       {
                    yylval.ival = atoi(yytext);
                    return INTEGER;
                }

"<="            { return LE; }
">="            { return GE; }
"=="            { return EQ; }
"!="            { return NE; }
"<"             { return LT; }
">"             { return GT; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return STAR; }
"/"             { return SLASH; }
"!"             { return NOT; }

"="             { return ASSIGN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }

"//".*          { /* single-line comment */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* multi-line comment */ }

[ \t\r\n]+      { /* ignore whitespace */ }

.               {
                    error_report(ERR_LEXICAL, yylineno,
                                 "Invalid character '%s'", yytext);
                    return yytext[0];
                }

%%
