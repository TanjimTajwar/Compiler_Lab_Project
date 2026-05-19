#include "semantic.h" // Include semantic analysis interface.
#include "symbol_table.h" // Include symbol table operations.
#include <stdio.h> // Include IO for error printing.
#include <string.h> // Include string comparison helpers.

static int semanticErrorCount = 0; // Track total semantic errors count.

static MiniDataType checkExpression(MiniAstNode *Ans); // Forward declare expression checker.
static void checkStatementList(MiniAstNode *head); // Forward declare statement list checker.

static void reportSemanticError(int lineNo, const char *message) { // Print unified semantic error message.
    fprintf(stderr, "Semantic Error (line %d): %s\n", lineNo, message); // Print line-aware error text.
    semanticErrorCount++; // Increase error counter.
} // End semantic error reporter.

static MiniDataType checkExpression(MiniAstNode *Ans) { // Validate expression node recursively.
    if (Ans == NULL) { // Protect against null node.
        return TYPE_UNKNOWN; // Return unknown for null expression.
    } // Close null guard.
    switch (Ans->nodeKind) { // Branch by expression node type.
        case NODE_NUMBER: // Handle integer literal case.
            Ans->dataType = TYPE_INT; // Mark node as integer type.
            return TYPE_INT; // Return integer type.
        case NODE_BOOL: // Handle boolean literal case.
            Ans->dataType = TYPE_BOOL; // Mark node as boolean type.
            return TYPE_BOOL; // Return boolean type.
        case NODE_IDENTIFIER: { // Handle variable reference case.
            MiniDataType detectedType = TYPE_UNKNOWN; // Prepare lookup output type.
            if (!lookupVariable(Ans->textValue, &detectedType)) { // Validate variable exists.
                char buffer[256]; // Create small buffer for error text.
                snprintf(buffer, sizeof(buffer), "'%s' used before declaration.", Ans->textValue); // Build undeclared message.
                reportSemanticError(Ans->lineNumber, buffer); // Report undeclared variable error.
                Ans->dataType = TYPE_UNKNOWN; // Keep unknown type after error.
                return TYPE_UNKNOWN; // Return unknown type on failure.
            } // Close undeclared check.
            Ans->dataType = detectedType; // Save found variable type.
            return detectedType; // Return found variable type.
        } // End identifier handling block.
        case NODE_BINARY: { // Handle binary expression case.
            MiniDataType leftType = checkExpression(Ans->left); // Check left operand type.
            MiniDataType rightType = checkExpression(Ans->right); // Check right operand type.
            if (strcmp(Ans->textValue, "+") == 0 || strcmp(Ans->textValue, "-") == 0 || strcmp(Ans->textValue, "*") == 0 || strcmp(Ans->textValue, "/") == 0) { // Match arithmetic operators.
                if (leftType != TYPE_INT || rightType != TYPE_INT) { // Verify both operands are int.
                    reportSemanticError(Ans->lineNumber, "Arithmetic operators require int operands."); // Report invalid arithmetic types.
                    Ans->dataType = TYPE_UNKNOWN; // Mark expression type unknown.
                    return TYPE_UNKNOWN; // Return unknown after error.
                } // Close arithmetic type check.
                Ans->dataType = TYPE_INT; // Arithmetic result is integer.
                return TYPE_INT; // Return integer type.
            } // Close arithmetic branch.
            if (strcmp(Ans->textValue, "<") == 0 || strcmp(Ans->textValue, ">") == 0) { // Match ordered relational operators.
                if (leftType != TYPE_INT || rightType != TYPE_INT) { // Ensure ordered compare uses integers.
                    reportSemanticError(Ans->lineNumber, "Relational operators '<' and '>' require int operands."); // Report mismatch.
                    Ans->dataType = TYPE_UNKNOWN; // Mark unknown type.
                    return TYPE_UNKNOWN; // Return unknown after error.
                } // Close int requirement check.
                Ans->dataType = TYPE_BOOL; // Relational expression returns bool.
                return TYPE_BOOL; // Return boolean type.
            } // Close ordered relation branch.
            if (strcmp(Ans->textValue, "==") == 0 || strcmp(Ans->textValue, "!=") == 0) { // Match equality operators.
                if (leftType == TYPE_UNKNOWN || rightType == TYPE_UNKNOWN || leftType != rightType) { // Ensure both operands share same known type.
                    reportSemanticError(Ans->lineNumber, "Equality operators require matching operand types."); // Report equality mismatch.
                    Ans->dataType = TYPE_UNKNOWN; // Mark unknown after mismatch.
                    return TYPE_UNKNOWN; // Return unknown type.
                } // Close equality type check.
                Ans->dataType = TYPE_BOOL; // Equality returns bool.
                return TYPE_BOOL; // Return boolean type.
            } // Close equality branch.
            reportSemanticError(Ans->lineNumber, "Unknown binary operator."); // Handle unsupported operator text.
            Ans->dataType = TYPE_UNKNOWN; // Mark unknown when unsupported.
            return TYPE_UNKNOWN; // Return unknown for unknown operator.
        } // End binary handling block.
        default: // Handle non-expression node default case.
            return TYPE_UNKNOWN; // Return unknown for unsupported expression node.
    } // Close switch statement.
} // End expression checker.

static void checkStatementList(MiniAstNode *head) { // Validate each statement in linked list.
    MiniAstNode *Tanjim = head; // Start list traversal pointer.
    while (Tanjim != NULL) { // Iterate all statements.
        switch (Tanjim->nodeKind) { // Branch by statement node kind.
            case NODE_DECL: // Handle declaration statement.
                declareVariable(Tanjim->textValue, Tanjim->dataType, Tanjim->lineNumber); // Insert declaration in current scope.
                break; // Exit declaration branch.
            case NODE_ASSIGN: { // Handle assignment statement.
                MiniDataType declaredType = TYPE_UNKNOWN; // Prepare declared type holder.
                if (!lookupVariable(Tanjim->textValue, &declaredType)) { // Ensure assignment target exists.
                    char buffer[256]; // Create message buffer.
                    snprintf(buffer, sizeof(buffer), "'%s' assigned before declaration.", Tanjim->textValue); // Compose target error message.
                    reportSemanticError(Tanjim->lineNumber, buffer); // Report undeclared assignment target.
                } // Close target lookup check.
                MiniDataType exprType = checkExpression(Tanjim->left); // Evaluate right-hand expression type.
                if (declaredType != TYPE_UNKNOWN && exprType != TYPE_UNKNOWN && declaredType != exprType) { // Compare assignment type compatibility.
                    reportSemanticError(Tanjim->lineNumber, "Assignment type mismatch."); // Report assignment mismatch.
                } // Close assignment compatibility check.
                break; // Exit assignment branch.
            } // End assignment handling block.
            case NODE_PRINT: // Handle print statement.
                checkExpression(Tanjim->left); // Validate printed expression type.
                break; // Exit print branch.
            case NODE_IF: { // Handle if or if-else statement.
                MiniDataType conditionType = checkExpression(Tanjim->left); // Validate condition expression.
                if (conditionType != TYPE_BOOL) { // Ensure condition is boolean.
                    reportSemanticError(Tanjim->lineNumber, "If condition must be bool."); // Report invalid condition type.
                } // Close if-condition check.
                enterScope(); // Open new scope for then-block.
                checkStatementList(Tanjim->right ? Tanjim->right->left : NULL); // Analyze then-block statements.
                exitScope(); // Close then-block scope.
                if (Tanjim->third != NULL) { // Check existence of else-block.
                    enterScope(); // Open new scope for else-block.
                    checkStatementList(Tanjim->third->left); // Analyze else-block statements.
                    exitScope(); // Close else-block scope.
                } // Close else-block handling.
                break; // Exit if branch.
            } // End if handling block.
            case NODE_WHILE: { // Handle while statement.
                MiniDataType loopType = checkExpression(Tanjim->left); // Validate loop condition type.
                if (loopType != TYPE_BOOL) { // Require bool loop condition.
                    reportSemanticError(Tanjim->lineNumber, "While condition must be bool."); // Report while condition mismatch.
                } // Close while-condition check.
                enterScope(); // Open new loop body scope.
                checkStatementList(Tanjim->right ? Tanjim->right->left : NULL); // Analyze loop body statements.
                exitScope(); // Close loop body scope.
                break; // Exit while branch.
            } // End while handling block.
            case NODE_BLOCK: // Handle explicit block node.
                enterScope(); // Open nested block scope.
                checkStatementList(Tanjim->left); // Analyze nested block statements.
                exitScope(); // Close nested block scope.
                break; // Exit block branch.
            default: // Handle other nodes that are not statements.
                break; // Ignore non-statement kinds safely.
        } // Close switch block.
        Tanjim = Tanjim->next; // Move to next statement node.
    } // Close statement traversal loop.
} // End statement list checker.

int runSemanticAnalysis(MiniAstNode *root) { // Run full semantic analysis pipeline.
    semanticErrorCount = 0; // Reset global semantic error count.
    enterScope(); // Create initial global scope.
    if (root != NULL && root->nodeKind == NODE_PROGRAM) { // Ensure program root is valid.
        checkStatementList(root->left); // Analyze top-level statements list.
    } // Close root validation check.
    exitScope(); // Close initial global scope.
    freeSymbolTable(); // Ensure symbol table memory is fully released.
    if (semanticErrorCount > 0) { // Check if any semantic errors were found.
        fprintf(stderr, "Semantic analysis failed with %d error(s).\n", semanticErrorCount); // Print semantic failure summary.
        return 0; // Return failure status.
    } // Close semantic error summary branch.
    return 1; // Return success when no semantic errors.
} // End semantic analysis entry function.
