%{
#include <stdio.h> /* Include standard IO for error printing. */
#include <stdlib.h> /* Include utility declarations. */
#include <string.h> /* Include string duplication helper. */
#include "parser.tab.h" /* Include parser token declarations. */

int miniLexErrorCount = 0; /* Track lexical error count. */
%}

%option noyywrap
%option yylineno

DIGIT      [0-9]
ID_START   [a-zA-Z_]
ID_REST    [a-zA-Z0-9_]*

%%

"int"                { return INT; /* Return int keyword token. */ }
"bool"               { return BOOL; /* Return bool keyword token. */ }
"if"                 { return IF; /* Return if keyword token. */ }
"else"               { return ELSE; /* Return else keyword token. */ }
"while"              { return WHILE; /* Return while keyword token. */ }
"print"              { return PRINT; /* Return print keyword token. */ }
"true"               { return TRUE; /* Return true keyword token. */ }
"false"              { return FALSE; /* Return false keyword token. */ }

"=="                 { return EQ; /* Return equality operator token. */ }
"!="                 { return NE; /* Return inequality operator token. */ }
"<"                  { return LT; /* Return less-than operator token. */ }
">"                  { return GT; /* Return greater-than operator token. */ }
"="                  { return ASSIGN; /* Return assignment token. */ }
"+"                  { return PLUS; /* Return plus token. */ }
"-"                  { return MINUS; /* Return minus token. */ }
"*"                  { return MUL; /* Return multiply token. */ }
"/"                  { return DIV; /* Return divide token. */ }

"("                  { return LP; /* Return left parenthesis token. */ }
")"                  { return RP; /* Return right parenthesis token. */ }
"{"                  { return LB; /* Return left brace token. */ }
"}"                  { return RB; /* Return right brace token. */ }
";"                  { return SEMI; /* Return semicolon token. */ }
","                  { return COMMA; /* Return comma token. */ }

{DIGIT}+             { yylval.number = atoi(yytext); return NUMBER; /* Parse integer token. */ }
{ID_START}{ID_REST}  { yylval.text = strdup(yytext); return ID; /* Parse identifier token. */ }

"//".*               { /* Ignore single-line comments. */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore multi-line comments. */ }

[ \t\r\n]+           { /* Ignore whitespace. */ }

.                    { fprintf(stderr, "Lexical Error (line %d): unknown character '%s'\n", yylineno, yytext); miniLexErrorCount++; /* Report bad token. */ }

%%
