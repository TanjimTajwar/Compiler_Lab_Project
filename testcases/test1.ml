int CSECU; // Declare integer variable CSECU.
int Tanjim; // Declare integer variable Tanjim.
bool Sara; // Declare boolean variable Sara.

CSECU = 10; // Assign initial value to CSECU.
Tanjim = 0; // Assign initial value to Tanjim.
Sara = CSECU > 3; // Compute relational expression into bool.

if (Sara) { // Start if block with bool condition.
    Tanjim = CSECU + 5; // Compute arithmetic expression in then block.
} else { // Start else block when condition false.
    Tanjim = CSECU - 1; // Compute alternate arithmetic path.
} // End if-else block.

while (Tanjim > 0) { // Start while loop with relational condition.
    print(Tanjim); // Print current loop value.
    Tanjim = Tanjim - 1; // Decrement loop control variable.
} // End while loop.
