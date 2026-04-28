#ifndef CODEGEN_H // Start code generation include guard.
#define CODEGEN_H // Define code generation include guard.

#include "ast.h" // Include AST declarations for generation.

void generateIntermediateAndTarget(MiniAstNode *root, const char *tacFileName, const char *asmFileName); // Generate TAC and pseudo assembly files.

#endif // End code generation include guard.
