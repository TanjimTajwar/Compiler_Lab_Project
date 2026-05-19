#ifndef SYMBOL_TABLE_H // Start symbol table include guard.
#define SYMBOL_TABLE_H // Define symbol table include guard.

#include "ast.h" // Include shared type definitions.

void enterScope(void); // Push a new scope level.
void exitScope(void); // Pop current scope level.
int declareVariable(const char *name, MiniDataType type, int lineNo); // Insert variable declaration.
int lookupVariable(const char *name, MiniDataType *outType); // Lookup variable across scopes.
void printSymbolTable(void); // Print debugging view of symbol table.
void freeSymbolTable(void); // Release all symbol table memory.

#endif // End symbol table include guard.
