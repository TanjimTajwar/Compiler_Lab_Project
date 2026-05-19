; Three Address Code (MiniLang Compiler)
0: i = 0
1: sum = 0
2: done = 0
L0:
4: t0 = ! done
5: if_false t0 goto L1
6: t1 = + sum, i
7: sum = t1
8: t2 = + i, 1
9: i = t2
10: t3 = == i, 5
11: if_false t3 goto L2
12: done = 1
L2:
14: goto L0
L1:
16: param sum
17: call print, 1
18: halt
