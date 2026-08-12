//Done by Tanjim Tajwar Arnab

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
 * MiniLang compiler driver program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "errors.h"
#include "semantic.h"
#include "codegen.h"
#include "optimizer.h"
#include "targetcode.h"

extern ASTNode *ast_root;
extern int yyparse(void);
extern FILE *yyin;
extern int yylineno;

extern int parse_error_flag;

static void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <source_file.ml>\n", prog);
    fprintf(stderr, "  Compiles MiniLang source and produces output.tac and output.asm\n");
}

int main(int argc, char *argv[]) {
    FILE *tac_file;
    FILE *asm_file;
    TACList *tac;
    int semantic_errors;

    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", argv[1]);
        return 1;
    }

    error_reset();
    yylineno = 1;
    parse_error_flag = 0;

    printf("=== MiniLang Compiler ===\n");
    printf("Compiling: %s\n\n", argv[1]);

    if (yyparse() != 0 || parse_error_flag) {
        fprintf(stderr, "\nCompilation aborted due to syntax errors.\n");
        fclose(yyin);
        return 1;
    }

    fclose(yyin);

    if (!ast_root) {
        fprintf(stderr, "Error: No AST generated.\n");
        return 1;
    }

    printf("=== Abstract Syntax Tree ===\n");
    ast_print(ast_root, 0);
    printf("\n");

    semantic_errors = semantic_analyze(ast_root);
    if (semantic_errors > 0 || error_get_count() > 0) {
        fprintf(stderr, "\nCompilation aborted due to semantic errors.\n");
        ast_free(ast_root);
        return 1;
    }

    printf("=== Semantic Analysis: PASSED ===\n\n");

    tac = codegen_generate(ast_root);
    if (!tac) {
        fprintf(stderr, "Error: TAC generation failed.\n");
        ast_free(ast_root);
        return 1;
    }

    printf("=== Generated TAC (before optimization) ===\n");
    tac_print(tac, stdout);
    printf("\n");

    optimizer_run(tac);

    printf("=== Optimized TAC ===\n");
    tac_print(tac, stdout);
    printf("\n");

    tac_file = fopen("output.tac", "w");
    if (!tac_file) {
        fprintf(stderr, "Error: Cannot create output.tac\n");
        tac_list_free(tac);
        ast_free(ast_root);
        return 1;
    }
    tac_print(tac, tac_file);
    fclose(tac_file);

    asm_file = fopen("output.asm", "w");
    if (!asm_file) {
        fprintf(stderr, "Error: Cannot create output.asm\n");
        tac_list_free(tac);
        ast_free(ast_root);
        return 1;
    }
    targetcode_generate(tac, asm_file);
    fclose(asm_file);

    printf("=== Output Files ===\n");
    printf("  output.tac - Three-address code\n");
    printf("  output.asm - Pseudo assembly\n");
    printf("\nCompilation successful.\n");

    tac_list_free(tac);
    ast_free(ast_root);
    return 0;
}
