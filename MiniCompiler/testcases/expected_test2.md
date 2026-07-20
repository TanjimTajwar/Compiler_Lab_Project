# Expected Output for test2.ml

## Source Program
While loop accumulating sum 0+1+2+3+4 = 10, then if-else on sum > 10.

## Expected Behavior
- sum evaluates to 10 after the while loop
- Condition sum > 10 is false (10 > 10 is false)
- else branch executes: print(0)

## Expected TAC (structure)
```
i = 0
sum = 0
goto Lcond0
Lbody0:
sum = sum + i
t0 = i + 1
i = t0
Lcond0:
t1 = i < 5
ifTrue t1 goto Lbody0
t2 = sum > 10
ifFalse t2 goto Lelse0
print sum
goto Lend0
Lelse0:
print 0
Lend0:
```

## Expected Result
- Printed value: 0
