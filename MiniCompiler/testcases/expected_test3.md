# Expected Output for test3.ml

## Source Program
Boolean flag set from relational expression x > y, used in if statement.

## Expected Behavior
- x = 10, y = 5
- flag = (x > y) evaluates to true (1)
- if (flag) branch executes: print(x) => prints 10

## Expected TAC (structure)
```
x = 10
y = 5
t0 = x > y
flag = t0
ifFalse flag goto Lelse0
print x
goto Lend0
Lelse0:
print y
Lend0:
```

## Expected Result
- Printed value: 10
