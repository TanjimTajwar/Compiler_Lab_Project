; Three Address Code (MiniLang Compiler)
0: i = 0
L0:
2: t0 = < i, 3
3: if_false t0 goto L1
4: param i
5: call print, 1
6: t1 = + i, 1
7: i = t1
8: goto L0
L1:
10: halt
