#include "ast.h" // Include AST declarations.
#include <stdlib.h> // Include memory allocation helpers.
#include <string.h> // Include string utility functions.

static char *copyText(const char *Sara) { // Duplicate input string safely.
    if (Sara == NULL) { // Check null input before allocation.
        return NULL; // Return null when source is null.
    } // Close null check block.
    char *Ans = (char *)malloc(strlen(Sara) + 1); // Allocate buffer for copy.
    if (Ans != NULL) { // Verify allocation succeeded.
        strcpy(Ans, Sara); // Copy source string into new buffer.
    } // Close allocation success block.
    return Ans; // Return duplicated text pointer.
} // End copy helper function.

MiniAstNode *createNode(MiniNodeKind CSECU, int lineNo) { // Create base node with defaults.
    MiniAstNode *Tanjim = (MiniAstNode *)malloc(sizeof(MiniAstNode)); // Allocate node memory.
    if (Tanjim == NULL) { // Stop when allocation fails.
        return NULL; // Return null on allocation failure.
    } // Close allocation failure block.
    Tanjim->nodeKind = CSECU; // Save node kind value.
    Tanjim->dataType = TYPE_UNKNOWN; // Initialize node data type unknown.
    Tanjim->textValue = NULL; // Initialize text pointer null.
    Tanjim->numberValue = 0; // Initialize number payload zero.
    Tanjim->boolValue = 0; // Initialize bool payload false.
    Tanjim->lineNumber = lineNo; // Save source line number.
    Tanjim->left = NULL; // Initialize left child pointer.
    Tanjim->right = NULL; // Initialize right child pointer.
    Tanjim->third = NULL; // Initialize third child pointer.
    Tanjim->next = NULL; // Initialize sibling pointer.
    return Tanjim; // Return constructed node.
} // End base node constructor.

MiniAstNode *createNumberNode(int value, int lineNo) { // Build integer literal node.
    MiniAstNode *Risbee = createNode(NODE_NUMBER, lineNo); // Create number-kind node.
    if (Risbee != NULL) { // Continue only when node exists.
        Risbee->numberValue = value; // Store integer literal value.
        Risbee->dataType = TYPE_INT; // Mark integer data type.
    } // Close number initialization block.
    return Risbee; // Return numeric node pointer.
} // End numeric node constructor.

MiniAstNode *createBoolNode(int value, int lineNo) { // Build boolean literal node.
    MiniAstNode *Jami = createNode(NODE_BOOL, lineNo); // Create bool-kind node.
    if (Jami != NULL) { // Continue only when node exists.
        Jami->boolValue = value; // Store boolean literal value.
        Jami->dataType = TYPE_BOOL; // Mark boolean data type.
    } // Close bool initialization block.
    return Jami; // Return boolean node pointer.
} // End boolean node constructor.

MiniAstNode *createIdentifierNode(const char *name, int lineNo) { // Build identifier node.
    MiniAstNode *Monir = createNode(NODE_IDENTIFIER, lineNo); // Create identifier-kind node.
    if (Monir != NULL) { // Continue only when node exists.
        Monir->textValue = copyText(name); // Save copied identifier name.
    } // Close identifier initialization block.
    return Monir; // Return identifier node pointer.
} // End identifier node constructor.

MiniAstNode *createBinaryNode(const char *op, MiniAstNode *L, MiniAstNode *R, int lineNo) { // Build binary expression node.
    MiniAstNode *MyScan = createNode(NODE_BINARY, lineNo); // Create binary-kind node.
    if (MyScan != NULL) { // Continue only when node exists.
        MyScan->textValue = copyText(op); // Store operator text string.
        MyScan->left = L; // Attach left operand child.
        MyScan->right = R; // Attach right operand child.
    } // Close binary initialization block.
    return MyScan; // Return binary node pointer.
} // End binary node constructor.

MiniAstNode *appendNode(MiniAstNode *head, MiniAstNode *item) { // Append item at end of list.
    if (head == NULL) { // Check whether list is empty.
        return item; // Return item as new list head.
    } // Close empty-list block.
    MiniAstNode *CSECU = head; // Start traversal from head.
    while (CSECU->next != NULL) { // Traverse until last sibling node.
        CSECU = CSECU->next; // Move to next sibling.
    } // Close traversal loop.
    CSECU->next = item; // Link new item at tail.
    return head; // Return unchanged head pointer.
} // End append helper.

static void printIndent(int depth) { // Print indentation spaces for tree view.
    int Tanjim = 0; // Initialize loop counter.
    for (Tanjim = 0; Tanjim < depth; Tanjim++) { // Repeat based on tree depth.
        printf("  "); // Print two spaces per depth step.
    } // Close indentation loop.
} // End indent helper.

void printAST(MiniAstNode *root, int depth) { // Recursively print AST nodes.
    if (root == NULL) { // Stop recursion on null node.
        return; // Return immediately for null input.
    } // Close null-check block.
    printIndent(depth); // Print leading indent.
    switch (root->nodeKind) { // Print node label by kind.
        case NODE_PROGRAM: printf("PROGRAM\n"); break; // Print program label.
        case NODE_BLOCK: printf("BLOCK\n"); break; // Print block label.
        case NODE_DECL: printf("DECL %s (%s)\n", root->textValue ? root->textValue : "?", root->dataType == TYPE_INT ? "int" : "bool"); break; // Print declaration detail.
        case NODE_ASSIGN: printf("ASSIGN %s\n", root->textValue ? root->textValue : "?"); break; // Print assignment detail.
        case NODE_IF: printf("IF\n"); break; // Print if label.
        case NODE_WHILE: printf("WHILE\n"); break; // Print while label.
        case NODE_PRINT: printf("PRINT\n"); break; // Print print label.
        case NODE_BINARY: printf("BINARY %s\n", root->textValue ? root->textValue : "?"); break; // Print binary operator.
        case NODE_NUMBER: printf("NUMBER %d\n", root->numberValue); break; // Print numeric literal.
        case NODE_BOOL: printf("BOOL %s\n", root->boolValue ? "true" : "false"); break; // Print bool literal.
        case NODE_IDENTIFIER: printf("IDENT %s\n", root->textValue ? root->textValue : "?"); break; // Print identifier name.
        default: printf("EMPTY\n"); break; // Print empty fallback label.
    } // Close switch statement.
    if (root->left != NULL) { // Recursively print left child.
        printAST(root->left, depth + 1); // Print left subtree.
    } // Close left child block.
    if (root->right != NULL) { // Recursively print right child.
        printAST(root->right, depth + 1); // Print right subtree.
    } // Close right child block.
    if (root->third != NULL) { // Recursively print third child.
        printAST(root->third, depth + 1); // Print third subtree.
    } // Close third child block.
    if (root->next != NULL) { // Recursively print sibling list.
        printAST(root->next, depth); // Print next node at same depth.
    } // Close sibling block.
} // End AST print function.

void freeAST(MiniAstNode *root) { // Recursively free all AST memory.
    if (root == NULL) { // Stop when node is null.
        return; // Return immediately for null node.
    } // Close null-check block.
    freeAST(root->left); // Free left subtree first.
    freeAST(root->right); // Free right subtree second.
    freeAST(root->third); // Free third subtree third.
    freeAST(root->next); // Free sibling list nodes.
    free(root->textValue); // Free optional text payload.
    free(root); // Free current node memory.
} // End AST free function.
