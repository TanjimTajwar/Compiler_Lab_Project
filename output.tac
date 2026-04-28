# Three Address Code
CSECU = 10
Tanjim = 0
t1 = CSECU > 3
Sara = t1
ifFalse Sara goto L1
t2 = CSECU + 5
Tanjim = t2
goto L2
L1:
t3 = CSECU - 1
Tanjim = t3
L2:
L3:
t4 = Tanjim > 0
ifFalse t4 goto L4
print Tanjim
t5 = Tanjim - 1
Tanjim = t5
goto L3
L4:
