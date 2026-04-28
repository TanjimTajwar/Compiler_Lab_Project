#include "codegen.h" // Include code generation interface.
#include <stdio.h> // Include file IO functions.
#include <stdlib.h> // Include general utility helpers.
#include <string.h> // Include string helper functions.

static int tempCounter = 0; // Track generated temporary variable count.
static int labelCounter = 0; // Track generated label count.
static FILE *tacOut = NULL; // Store TAC output file handle.
static FILE *asmOut = NULL; // Store pseudo assembly file handle.

static char *newTempName(void) { // Create a new temporary variable name.
    char buffer[32]; // Keep local buffer for formatted temp text.
    snprintf(buffer, sizeof(buffer), "t%d", ++tempCounter); // Build next temp name.
    char *Ans = (char *)malloc(strlen(buffer) + 1); // Allocate exact string memory.
    if (Ans != NULL) { // Validate allocation before copy.
        strcpy(Ans, buffer); // Copy generated temp name.
    } // Close allocation check.
    return Ans; // Return allocated temp name.
} // End temporary name helper.

static char *newLabelName(void) { // Create a new label text.
    char buffer[32]; // Keep local buffer for formatted label text.
    snprintf(buffer, sizeof(buffer), "L%d", ++labelCounter); // Build next label name.
    char *Ans = (char *)malloc(strlen(buffer) + 1); // Allocate exact string memory.
    if (Ans != NULL) { // Validate allocation before copy.
        strcpy(Ans, buffer); // Copy generated label string.
    } // Close allocation check.
    return Ans; // Return allocated label name.
} // End label helper.

static int isNumberText(const char *text) { // Check whether text is an integer literal.
    if (text == NULL || *text == '\0') { // Guard null or empty string.
        return 0; // Return false for invalid text.
    } // Close input guard.
    int start = 0; // Track starting index for digit scan.
    if (text[0] == '-') { // Handle optional negative sign.
        start = 1; // Skip sign character.
    } // Close sign check.
    for (int Tanjim = start; text[Tanjim] != '\0'; Tanjim++) { // Scan all remaining characters.
        if (text[Tanjim] < '0' || text[Tanjim] > '9') { // Detect non-digit characters.
            return 0; // Return false when non-digit appears.
        } // Close non-digit check.
    } // Close digit scan loop.
    return 1; // Return true when all characters are digits.
} // End number-text checker.

static char *foldConstantsIfPossible(const char *op, const char *left, const char *right) { // Perform simple constant folding.
    if (!isNumberText(left) || !isNumberText(right)) { // Require both operands as numeric text.
        return NULL; // Return null when folding is not possible.
    } // Close fold precondition check.
    int L = atoi(left); // Convert left operand to integer.
    int R = atoi(right); // Convert right operand to integer.
    int result = 0; // Prepare folded result variable.
    if (strcmp(op, "+") == 0) { result = L + R; } // Fold addition operation.
    else if (strcmp(op, "-") == 0) { result = L - R; } // Fold subtraction operation.
    else if (strcmp(op, "*") == 0) { result = L * R; } // Fold multiplication operation.
    else if (strcmp(op, "/") == 0) { if (R == 0) { return NULL; } result = L / R; } // Fold division with zero guard.
    else if (strcmp(op, "<") == 0) { result = (L < R); } // Fold less-than relation.
    else if (strcmp(op, ">") == 0) { result = (L > R); } // Fold greater-than relation.
    else if (strcmp(op, "==") == 0) { result = (L == R); } // Fold equality relation.
    else if (strcmp(op, "!=") == 0) { result = (L != R); } // Fold inequality relation.
    else { return NULL; } // Return null for unsupported operators.
    char buffer[32]; // Keep local result buffer.
    snprintf(buffer, sizeof(buffer), "%d", result); // Convert folded result to text.
    char *Ans = (char *)malloc(strlen(buffer) + 1); // Allocate folded result string.
    if (Ans != NULL) { // Check allocation success.
        strcpy(Ans, buffer); // Copy folded result text.
    } // Close copy block.
    return Ans; // Return folded text pointer.
} // End constant folding helper.

static char *generateExpression(MiniAstNode *Monir); // Forward declare expression code generator.
static void generateStatementList(MiniAstNode *Jami); // Forward declare statement list generator.

static char *generateExpression(MiniAstNode *Monir) { // Generate TAC for expression and return place name.
    if (Monir == NULL) { // Guard against null expression.
        return NULL; // Return null for missing expression.
    } // Close null guard.
    if (Monir->nodeKind == NODE_NUMBER) { // Handle integer literal node.
        char buffer[32]; // Create literal conversion buffer.
        snprintf(buffer, sizeof(buffer), "%d", Monir->numberValue); // Convert integer value to text.
        char *Ans = (char *)malloc(strlen(buffer) + 1); // Allocate literal text memory.
        if (Ans != NULL) { strcpy(Ans, buffer); } // Copy literal text when allocated.
        return Ans; // Return literal text as place.
    } // Close numeric literal branch.
    if (Monir->nodeKind == NODE_BOOL) { // Handle boolean literal node.
        char *Ans = (char *)malloc(2); // Allocate tiny bool text buffer.
        if (Ans != NULL) { strcpy(Ans, Monir->boolValue ? "1" : "0"); } // Map bool to 1 or 0.
        return Ans; // Return boolean text place.
    } // Close boolean literal branch.
    if (Monir->nodeKind == NODE_IDENTIFIER) { // Handle identifier reference node.
        char *Ans = (char *)malloc(strlen(Monir->textValue) + 1); // Allocate identifier name buffer.
        if (Ans != NULL) { strcpy(Ans, Monir->textValue); } // Copy identifier text.
        return Ans; // Return identifier name as place.
    } // Close identifier branch.
    if (Monir->nodeKind == NODE_BINARY) { // Handle binary expression node.
        char *leftPlace = generateExpression(Monir->left); // Recursively generate left operand.
        char *rightPlace = generateExpression(Monir->right); // Recursively generate right operand.
        char *folded = foldConstantsIfPossible(Monir->textValue, leftPlace, rightPlace); // Try constant folding optimization.
        if (folded != NULL) { // Check if folding succeeded.
            free(leftPlace); // Free left operand temporary text.
            free(rightPlace); // Free right operand temporary text.
            return folded; // Return folded constant directly.
        } // Close folding success branch.
        char *tempPlace = newTempName(); // Allocate new temporary destination.
        fprintf(tacOut, "%s = %s %s %s\n", tempPlace, leftPlace, Monir->textValue, rightPlace); // Emit TAC binary instruction.
        if (strcmp(Monir->textValue, "+") == 0) { fprintf(asmOut, "ADD %s, %s, %s\n", tempPlace, leftPlace, rightPlace); } // Emit pseudo assembly ADD.
        else if (strcmp(Monir->textValue, "-") == 0) { fprintf(asmOut, "SUB %s, %s, %s\n", tempPlace, leftPlace, rightPlace); } // Emit pseudo assembly SUB.
        else if (strcmp(Monir->textValue, "*") == 0) { fprintf(asmOut, "MUL %s, %s, %s\n", tempPlace, leftPlace, rightPlace); } // Emit pseudo assembly MUL.
        else if (strcmp(Monir->textValue, "/") == 0) { fprintf(asmOut, "DIV %s, %s, %s\n", tempPlace, leftPlace, rightPlace); } // Emit pseudo assembly DIV.
        else { fprintf(asmOut, "CMP %s, %s, %s ; %s\n", tempPlace, leftPlace, rightPlace, Monir->textValue); } // Emit pseudo compare line.
        free(leftPlace); // Free left operand place text.
        free(rightPlace); // Free right operand place text.
        return tempPlace; // Return destination temporary.
    } // Close binary branch.
    return NULL; // Return null for unsupported node kind.
} // End expression generator.

static void generateStatementList(MiniAstNode *Jami) { // Generate TAC and assembly for statement list.
    MiniAstNode *CSECU = Jami; // Start traversal from first statement.
    while (CSECU != NULL) { // Iterate over each statement node.
        if (CSECU->nodeKind == NODE_ASSIGN) { // Handle assignment statement.
            char *valuePlace = generateExpression(CSECU->left); // Generate right-hand expression code.
            if (valuePlace != NULL) { // Ensure expression generation worked.
                fprintf(tacOut, "%s = %s\n", CSECU->textValue, valuePlace); // Emit assignment TAC.
                fprintf(asmOut, "MOV %s, %s\n", CSECU->textValue, valuePlace); // Emit MOV pseudo assembly.
                free(valuePlace); // Free generated expression place.
            } // Close assignment code block.
        } else if (CSECU->nodeKind == NODE_PRINT) { // Handle print statement.
            char *printPlace = generateExpression(CSECU->left); // Generate printed expression.
            if (printPlace != NULL) { // Verify expression place exists.
                fprintf(tacOut, "print %s\n", printPlace); // Emit TAC print instruction.
                fprintf(asmOut, "PRINT %s\n", printPlace); // Emit pseudo assembly print line.
                free(printPlace); // Free generated place text.
            } // Close print code block.
        } else if (CSECU->nodeKind == NODE_IF) { // Handle if or if-else statement.
            char *condPlace = generateExpression(CSECU->left); // Generate condition expression code.
            char *elseLabel = newLabelName(); // Allocate else label name.
            char *endLabel = newLabelName(); // Allocate end label name.
            if (strcmp(condPlace, "0") == 0 && CSECU->third == NULL) { // Apply tiny dead code removal for always-false if without else.
                free(condPlace); // Free condition place text.
                free(elseLabel); // Free unused else label.
                free(endLabel); // Free unused end label.
            } else { // Emit normal control-flow code.
                fprintf(tacOut, "ifFalse %s goto %s\n", condPlace, elseLabel); // Emit conditional jump TAC.
                fprintf(asmOut, "IF %s JMP %s\n", condPlace, elseLabel); // Emit pseudo conditional jump.
                generateStatementList(CSECU->right ? CSECU->right->left : NULL); // Generate then-block statements.
                fprintf(tacOut, "goto %s\n", endLabel); // Emit jump to if-end TAC.
                fprintf(asmOut, "JMP %s\n", endLabel); // Emit unconditional jump assembly.
                fprintf(tacOut, "%s:\n", elseLabel); // Emit else label TAC.
                fprintf(asmOut, "LABEL %s\n", elseLabel); // Emit else label assembly.
                if (CSECU->third != NULL) { // Check else-block existence.
                    generateStatementList(CSECU->third->left); // Generate else-block statements.
                } // Close else generation block.
                fprintf(tacOut, "%s:\n", endLabel); // Emit end label TAC.
                fprintf(asmOut, "LABEL %s\n", endLabel); // Emit end label assembly.
                free(condPlace); // Free condition place text.
                free(elseLabel); // Free else label text.
                free(endLabel); // Free end label text.
            } // Close if control-flow branch.
        } else if (CSECU->nodeKind == NODE_WHILE) { // Handle while loop statement.
            char *startLabel = newLabelName(); // Allocate loop start label.
            char *endLabel = newLabelName(); // Allocate loop end label.
            fprintf(tacOut, "%s:\n", startLabel); // Emit loop start label TAC.
            fprintf(asmOut, "LABEL %s\n", startLabel); // Emit loop start label assembly.
            char *condPlace = generateExpression(CSECU->left); // Generate loop condition expression.
            fprintf(tacOut, "ifFalse %s goto %s\n", condPlace, endLabel); // Emit loop exit check TAC.
            fprintf(asmOut, "IF %s JMP %s\n", condPlace, endLabel); // Emit loop exit check assembly.
            generateStatementList(CSECU->right ? CSECU->right->left : NULL); // Generate loop body statements.
            fprintf(tacOut, "goto %s\n", startLabel); // Emit jump back to loop start TAC.
            fprintf(asmOut, "JMP %s\n", startLabel); // Emit jump back assembly.
            fprintf(tacOut, "%s:\n", endLabel); // Emit loop end label TAC.
            fprintf(asmOut, "LABEL %s\n", endLabel); // Emit loop end label assembly.
            free(startLabel); // Free start label text.
            free(endLabel); // Free end label text.
            free(condPlace); // Free condition place text.
        } else if (CSECU->nodeKind == NODE_BLOCK) { // Handle nested block statement.
            generateStatementList(CSECU->left); // Generate nested block statements.
        } // Close statement-kind branching.
        CSECU = CSECU->next; // Move to next statement in list.
    } // Close statement traversal loop.
} // End statement list generator.

void generateIntermediateAndTarget(MiniAstNode *root, const char *tacFileName, const char *asmFileName) { // Generate both output files.
    tempCounter = 0; // Reset temporary counter before generation.
    labelCounter = 0; // Reset label counter before generation.
    tacOut = fopen(tacFileName, "w"); // Open TAC file for writing.
    asmOut = fopen(asmFileName, "w"); // Open pseudo assembly file for writing.
    if (tacOut == NULL || asmOut == NULL) { // Validate file handles.
        fprintf(stderr, "Code generation error: cannot open output files.\n"); // Print output file error.
        if (tacOut != NULL) { fclose(tacOut); } // Close TAC handle if opened.
        if (asmOut != NULL) { fclose(asmOut); } // Close ASM handle if opened.
        return; // Stop generation when files are unavailable.
    } // Close file-open check.
    fprintf(tacOut, "# Three Address Code\n"); // Write TAC header comment.
    fprintf(asmOut, "# Pseudo Assembly\n"); // Write assembly header comment.
    if (root != NULL && root->nodeKind == NODE_PROGRAM) { // Confirm valid program root.
        generateStatementList(root->left); // Generate code for top-level statements.
    } // Close root guard block.
    fclose(tacOut); // Close TAC output file.
    fclose(asmOut); // Close assembly output file.
    tacOut = NULL; // Reset TAC handle pointer.
    asmOut = NULL; // Reset assembly handle pointer.
} // End code generation entry function.
