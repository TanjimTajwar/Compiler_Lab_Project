int CSECU; // Declare integer variable CSECU.
bool Jami; // Declare boolean variable Jami.
int CSECU; // Trigger redeclaration semantic error.

CSECU = true; // Trigger assignment type mismatch error.
Monir = 5; // Trigger undeclared variable assignment error.

if (CSECU) { // Trigger invalid non-bool condition error.
    print(CSECU); // Keep a valid print statement inside block.
} // End if block.

Jami = 9 > ; // Trigger syntax error with incomplete expression.
