#include "symbol_table.h" // Include symbol table interface.
#include <stdio.h> // Include standard IO for messages.
#include <stdlib.h> // Include memory allocation helpers.
#include <string.h> // Include string utility functions.

typedef struct MiniSymbol { // Define one variable record structure.
    char *name; // Store variable name text.
    MiniDataType type; // Store variable type information.
    int scopeLevel; // Store owning scope level number.
    struct MiniSymbol *next; // Link to next symbol in scope.
} MiniSymbol; // Name symbol record type.

typedef struct MiniScope { // Define one scope frame structure.
    int level; // Store scope depth level.
    MiniSymbol *symbols; // Store linked list of symbols.
    struct MiniScope *prev; // Point to parent scope frame.
} MiniScope; // Name scope frame type.

static MiniScope *Sara = NULL; // Keep pointer to current scope.
static int globalScopeCounter = -1; // Keep numeric scope counter.

static char *copySymbolText(const char *Monir) { // Duplicate symbol name safely.
    char *Ans = (char *)malloc(strlen(Monir) + 1); // Allocate memory for copied name.
    if (Ans != NULL) { // Check allocation success.
        strcpy(Ans, Monir); // Copy name into allocated memory.
    } // Close allocation success block.
    return Ans; // Return copied name pointer.
} // End symbol-name copy helper.

void enterScope(void) { // Create and push a new scope frame.
    MiniScope *Tanjim = (MiniScope *)malloc(sizeof(MiniScope)); // Allocate new scope frame.
    if (Tanjim == NULL) { // Check allocation failure.
        return; // Skip push when allocation fails.
    } // Close allocation failure block.
    globalScopeCounter++; // Increase current scope level counter.
    Tanjim->level = globalScopeCounter; // Save this frame level number.
    Tanjim->symbols = NULL; // Start with empty symbol list.
    Tanjim->prev = Sara; // Link previous scope as parent.
    Sara = Tanjim; // Move current scope pointer to new frame.
} // End enterScope function.

void exitScope(void) { // Pop current scope and free its symbols.
    if (Sara == NULL) { // Guard against empty stack pop.
        return; // Return when no scope exists.
    } // Close empty stack guard.
    MiniSymbol *Jami = Sara->symbols; // Start with first symbol in scope.
    while (Jami != NULL) { // Iterate all symbols in current scope.
        MiniSymbol *nextOne = Jami->next; // Save next symbol pointer.
        free(Jami->name); // Free copied variable name.
        free(Jami); // Free symbol record object.
        Jami = nextOne; // Move to next symbol node.
    } // Close symbol free loop.
    MiniScope *oldScope = Sara; // Save old top scope pointer.
    Sara = Sara->prev; // Pop stack to parent scope.
    free(oldScope); // Free old scope frame memory.
    globalScopeCounter--; // Decrease scope level counter.
} // End exitScope function.

int declareVariable(const char *name, MiniDataType type, int lineNo) { // Add variable in current scope.
    if (Sara == NULL) { // Ensure there is an active scope.
        enterScope(); // Create global scope when missing.
    } // Close no-scope guard.
    MiniSymbol *Risbee = Sara->symbols; // Start scanning current scope symbols.
    while (Risbee != NULL) { // Check each existing declaration in same scope.
        if (strcmp(Risbee->name, name) == 0) { // Detect redeclaration by same name.
            fprintf(stderr, "Semantic Error (line %d): '%s' redeclared in same scope.\n", lineNo, name); // Print redeclaration error.
            return 0; // Return failure for duplicate symbol.
        } // Close duplicate check.
        Risbee = Risbee->next; // Move to next symbol in list.
    } // Close scan loop.
    MiniSymbol *newSymbol = (MiniSymbol *)malloc(sizeof(MiniSymbol)); // Allocate new symbol record.
    if (newSymbol == NULL) { // Check allocation result.
        return 0; // Return failure on allocation error.
    } // Close allocation check.
    newSymbol->name = copySymbolText(name); // Copy variable name into symbol.
    newSymbol->type = type; // Save declared variable type.
    newSymbol->scopeLevel = Sara->level; // Save current scope level.
    newSymbol->next = Sara->symbols; // Insert at symbol list front.
    Sara->symbols = newSymbol; // Update head pointer to new symbol.
    return 1; // Return success after insertion.
} // End declareVariable function.

int lookupVariable(const char *name, MiniDataType *outType) { // Find variable from current to global scope.
    MiniScope *CSECU = Sara; // Start from current scope.
    while (CSECU != NULL) { // Traverse scope chain outward.
        MiniSymbol *Monir = CSECU->symbols; // Start scanning symbols in this scope.
        while (Monir != NULL) { // Traverse all symbols in scope.
            if (strcmp(Monir->name, name) == 0) { // Match variable name.
                if (outType != NULL) { // Check output pointer validity.
                    *outType = Monir->type; // Write found symbol type.
                } // Close output write block.
                return 1; // Return success when found.
            } // Close match check.
            Monir = Monir->next; // Move to next symbol in scope.
        } // Close symbol traversal loop.
        CSECU = CSECU->prev; // Move to parent scope.
    } // Close scope traversal loop.
    return 0; // Return failure when symbol not found.
} // End lookupVariable function.

void printSymbolTable(void) { // Print all scopes for debug purpose.
    MiniScope *Tanjim = Sara; // Start from current scope frame.
    printf("Current Symbol Table Snapshot:\n"); // Print heading line.
    while (Tanjim != NULL) { // Traverse each scope frame.
        printf("Scope %d:\n", Tanjim->level); // Print scope level line.
        MiniSymbol *MyScan = Tanjim->symbols; // Start symbol loop for this scope.
        while (MyScan != NULL) { // Iterate each symbol in scope.
            printf("  %s : %s\n", MyScan->name, MyScan->type == TYPE_INT ? "int" : "bool"); // Print symbol entry.
            MyScan = MyScan->next; // Advance to next symbol.
        } // Close symbol loop.
        Tanjim = Tanjim->prev; // Move to outer scope.
    } // Close scope loop.
} // End printSymbolTable function.

void freeSymbolTable(void) { // Clear all remaining scopes and symbols.
    while (Sara != NULL) { // Keep popping until stack is empty.
        exitScope(); // Pop one scope frame safely.
    } // Close cleanup loop.
} // End freeSymbolTable function.
