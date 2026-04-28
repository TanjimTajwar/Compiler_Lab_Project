#include <stdio.h> // Include file IO declarations.
#include <stdlib.h> // Include utility declarations.
#include "ast.h" // Include AST declarations.
#include "semantic.h" // Include semantic checker declarations.
#include "codegen.h" // Include code generator declarations.

extern FILE *yyin; // Import lexer input file handle.
extern int yyparse(void); // Import parser entry function.
extern MiniAstNode *miniRoot; // Import parser-produced AST root.
extern int miniLexErrorCount; // Import lexer error counter.

int main(int argc, char **argv) { // Start compiler driver main function.
    if (argc != 2) { // Validate expected command-line usage.
        fprintf(stderr, "Usage: %s <input.ml>\n", argv[0]); // Print usage hint on wrong args.
        return 1; // Exit with failure code.
    } // Close argument validation block.
    yyin = fopen(argv[1], "r"); // Open MiniLang input source file.
    if (yyin == NULL) { // Check file open success.
        fprintf(stderr, "Error: cannot open input file '%s'\n", argv[1]); // Print file open error.
        return 1; // Exit with failure when file missing.
    } // Close file-open check.
    int parseStatus = yyparse(); // Run parser and AST builder.
    fclose(yyin); // Close input file after parsing.
    yyin = NULL; // Reset global lexer file pointer.
    if (miniLexErrorCount > 0) { // Stop when lexical errors were found.
        fprintf(stderr, "Compilation stopped: %d lexical error(s).\n", miniLexErrorCount); // Print lexical summary.
        freeAST(miniRoot); // Free AST memory before exit.
        return 1; // Exit with lexical failure status.
    } // Close lexical error branch.
    if (parseStatus != 0 || miniRoot == NULL) { // Stop when syntax parsing fails.
        fprintf(stderr, "Compilation stopped: syntax analysis failed.\n"); // Print syntax failure summary.
        freeAST(miniRoot); // Free any partial AST memory.
        return 1; // Exit with syntax failure code.
    } // Close syntax error branch.
    if (!runSemanticAnalysis(miniRoot)) { // Run semantic analysis and check result.
        freeAST(miniRoot); // Free AST memory after semantic failure.
        return 1; // Exit with semantic failure code.
    } // Close semantic check branch.
    generateIntermediateAndTarget(miniRoot, "output.tac", "output.asm"); // Generate TAC and pseudo assembly outputs.
    printf("Compilation succeeded.\n"); // Print successful compilation message.
    printf("Generated: output.tac and output.asm\n"); // Print generated output files message.
    freeAST(miniRoot); // Release AST memory on success.
    return 0; // Exit with success status.
} // End compiler driver function.
