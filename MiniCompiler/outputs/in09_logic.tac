; Three Address Code (MiniLang Compiler)
0: a = 1
1: b = 0
2: t0 = ! b
3: t1 = && a, t0
4: c = t1
5: t2 = || c, b
6: if_false t2 goto L0
7: param 1
8: call print, 1
9: goto L1
L0:
11: param 0
12: call print, 1
L1:
14: halt
