/*
 * File: main.c
 * Developed by: Tanjim Tajwar Arnab (ID: 22701066)
 * Contribution: Driver, Phase Integration, Debug Mode, CLI
 *
 * Complexity: overall O(n) dominated by parse + semantic + codegen passes.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern ASTNode *g_ast_root;
extern int yyparse(void);
extern FILE *yyin;

static int debug_mode = 0;

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-d] <source.ml>\n", prog);
    fprintf(stderr, "  -d    debug mode (AST, symbol table, TAC to stdout)\n");
}

int main(int argc, char **argv) {
    const char *input = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            debug_mode = 1;
        else if (argv[i][0] != '-' || !strcmp(argv[i], "-"))
            input = argv[i];
        else {
            usage(argv[0]);
            return 1;
        }
    }

    if (!input) {
        usage(argv[0]);
        return 1;
    }

    yyin = fopen(input, "r");
    if (!yyin) {
        perror(input);
        return 1;
    }

    printf("MiniLang Compiler — processing '%s'\n", input);
    syntax_error_count = 0;

    if (yyparse() != 0 || syntax_error_count > 0) {
        fprintf(stderr, "Compilation failed: %d syntax error(s).\n", syntax_error_count);
        fclose(yyin);
        return 1;
    }
    fclose(yyin);

    if (!g_ast_root) {
        fprintf(stderr, "Error: empty program.\n");
        return 1;
    }

    if (debug_mode) {
        print_ast(g_ast_root, 0);
    }

    int sem_errs = semantic_analyze(g_ast_root);
    if (debug_mode) {
        symtab_print();
    }
    if (sem_errs > 0) {
        fprintf(stderr, "Compilation failed: %d semantic error(s).\n", sem_errs);
        free_ast(g_ast_root);
        symtab_free();
        return 1;
    }

    TACList *tac = codegen_generate(g_ast_root);
    tac = optimize_tac(tac);

    if (tac_save("output.tac", tac) != 0) {
        free_ast(g_ast_root);
        tac_free(tac);
        symtab_free();
        return 1;
    }
    printf("TAC written to output.tac\n");

    if (debug_mode) {
        printf("\n--- Optimized TAC ---\n");
        tac_print(stdout, tac);
    }

    if (target_generate(tac, "output.asm") != 0) {
        free_ast(g_ast_root);
        tac_free(tac);
        symtab_free();
        return 1;
    }
    printf("Target code written to output.asm\n");
    printf("Compilation successful.\n");

    free_ast(g_ast_root);
    tac_free(tac);
    symtab_free();
    return 0;
}
