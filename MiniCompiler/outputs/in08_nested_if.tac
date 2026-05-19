; Three Address Code (MiniLang Compiler)
0: n = 10
1: t0 = > n, 0
2: pos = t0
3: if_false pos goto L0
4: t1 = == n, 10
5: if_false t1 goto L2
6: param n
7: call print, 1
8: goto L3
L2:
10: param 0
11: call print, 1
L3:
13: goto L1
L0:
15: t2 = -1
16: param t2
17: call print, 1
L1:
19: halt
