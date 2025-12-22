bool a, b

a := 1 = 1
a := 1 > 2
a := 1 < 2
a := 1 <= 2
a := 1 >= 2
a := 1 <> 2

a := 1 < 2 and 5 = 4
b := 1 < 2 or 5 = 4 or 1
a := not a

/*
1:      IF 1 EQ 1 GOTO 3
2:      GOTO 5
3:      $t01 := 1
4:      GOTO 6
5:      $t01 := 0
6:      a := $t01
7:      IF 1 GTI 2 GOTO 9
8:      GOTO 11
9:      $t01 := 1
10:     GOTO 12
11:     $t01 := 0
12:     a := $t01
13:     IF 1 LTI 2 GOTO 15
14:     GOTO 17
15:     $t01 := 1
16:     GOTO 18
17:     $t01 := 0
18:     a := $t01
19:     IF 1 LEI 2 GOTO 21
20:     GOTO 23
21:     $t01 := 1
22:     GOTO 24
23:     $t01 := 0
24:     a := $t01
25:     IF 1 GEI 2 GOTO 27
26:     GOTO 29
27:     $t01 := 1
28:     GOTO 30
29:     $t01 := 0
30:     a := $t01
31:     IF 1 NE 2 GOTO 33
32:     GOTO 35
33:     $t01 := 1
34:     GOTO 36
35:     $t01 := 0
36:     a := $t01
37:     IF 1 LTI 2 GOTO 39
38:     GOTO 43
39:     IF 5 EQ 4 GOTO 41
40:     GOTO 43
41:     $t01 := 1
42:     GOTO 44
43:     $t01 := 0
44:     a := $t01
45:     IF 1 LTI 2 GOTO 51
46:     GOTO 47
47:     IF 5 EQ 4 GOTO 51
48:     GOTO 49
49:     IF 1 NE 0 GOTO 51
50:     GOTO 53
51:     $t01 := 1
52:     GOTO 54
53:     $t01 := 0
54:     b := $t01
55:     IF a NE 0 GOTO 59
56:     GOTO 57
57:     $t01 := 1
58:     GOTO 60
59:     $t01 := 0
60:     a := $t01
61:     HALT
*/