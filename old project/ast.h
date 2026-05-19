#ifndef AST_H // Define include guard start.
#define AST_H // Define include guard macro.

#include <stdio.h> // Include standard IO for print helpers.

typedef enum { // Define supported data types in MiniLang.
    TYPE_INT, // Represent integer type.
    TYPE_BOOL, // Represent boolean type.
    TYPE_UNKNOWN // Represent unknown type during error recovery.
} MiniDataType; // Name this enum for later usage.

typedef enum { // Define every AST node category.
    NODE_PROGRAM, // Represent whole program root.
    NODE_BLOCK, // Represent scoped statement block.
    NODE_DECL, // Represent declaration statement.
    NODE_ASSIGN, // Represent assignment statement.
    NODE_IF, // Represent if or if-else statement.
    NODE_WHILE, // Represent while loop statement.
    NODE_PRINT, // Represent print statement.
    NODE_BINARY, // Represent binary expression.
    NODE_NUMBER, // Represent integer literal.
    NODE_BOOL, // Represent boolean literal.
    NODE_IDENTIFIER, // Represent variable reference.
    NODE_EMPTY // Represent empty statement placeholder.
} MiniNodeKind; // Name this enum for AST node kinds.

typedef struct MiniAstNode { // Create AST node struct definition.
    MiniNodeKind nodeKind; // Store kind of current node.
    MiniDataType dataType; // Store semantic type when known.
    char *textValue; // Store identifier name or operator text.
    int numberValue; // Store numeric value for literals.
    int boolValue; // Store boolean value as 0 or 1.
    int lineNumber; // Store source line for diagnostics.
    struct MiniAstNode *left; // Store first child pointer.
    struct MiniAstNode *right; // Store second child pointer.
    struct MiniAstNode *third; // Store third child pointer.
    struct MiniAstNode *next; // Store linked-list sibling pointer.
} MiniAstNode; // Name this struct type.

MiniAstNode *createNode(MiniNodeKind CSECU, int lineNo); // Create generic node.
MiniAstNode *createNumberNode(int value, int lineNo); // Create integer literal node.
MiniAstNode *createBoolNode(int value, int lineNo); // Create boolean literal node.
MiniAstNode *createIdentifierNode(const char *name, int lineNo); // Create identifier node.
MiniAstNode *createBinaryNode(const char *op, MiniAstNode *L, MiniAstNode *R, int lineNo); // Create binary node.
MiniAstNode *appendNode(MiniAstNode *head, MiniAstNode *item); // Append node to linked list.
void printAST(MiniAstNode *root, int depth); // Print AST tree recursively.
void freeAST(MiniAstNode *root); // Release allocated AST recursively.

#endif // End include guard.
