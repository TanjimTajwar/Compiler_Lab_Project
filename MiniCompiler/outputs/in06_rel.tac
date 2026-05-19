; Three Address Code (MiniLang Compiler)
0: a = 5
1: b = 5
2: t0 = == a, b
3: eq = t0
4: t1 = != a, b
5: ne = t1
6: if_false eq goto L0
7: param a
8: call print, 1
L0:
10: halt
