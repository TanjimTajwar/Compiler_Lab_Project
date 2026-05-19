; Three Address Code (MiniLang Compiler)
0: p = 7
1: q = 3
2: t0 = > p, q
3: ok = t0
4: if_false ok goto L0
5: param p
6: call print, 1
7: goto L1
L0:
9: param q
10: call print, 1
L1:
12: halt
