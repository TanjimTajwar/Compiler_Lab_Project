; Three Address Code (MiniLang Compiler)
0: flag = 1
1: other = 0
2: if_false flag goto L0
3: param 1
4: call print, 1
5: goto L1
L0:
7: param 0
8: call print, 1
L1:
10: halt
