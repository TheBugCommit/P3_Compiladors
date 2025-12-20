for i in 1..3 do
    if i < 2 then
        i := 5
    else
        break;
    fi
done

int x
x := 3
for i in 1..x+2 do
    if i < 2 then
        i := 5
    else
        break;
    fi
done

/*
1:      i := 1
2:      $t01 := i
3:      IF i LTI 2 GOTO 5
4:      GOTO 7
5:      i := 5
6:      GOTO 8
7:      GOTO 22
8:      $t01 := $t01 ADDI 1
9:      i := $t01
10:     IF i LTI 2 GOTO 12
11:     GOTO 14
12:     i := 5
13:     GOTO 15
14:     GOTO 22
15:     $t01 := $t01 ADDI 1
16:     i := $t01
17:     IF i LTI 2 GOTO 19
18:     GOTO 21
19:     i := 5
20:     GOTO 22
21:     GOTO 22
22:     x := 3
23:     $t01 := x ADDI 2
24:     i := 1
25:     $t02 := i
26:     IF i LEI $t01 GOTO 28
27:     GOTO 36
28:     IF i LTI 2 GOTO 30
29:     GOTO 32
30:     i := 5
31:     GOTO 33
32:     GOTO 36
33:     $t02 := $t02 ADDI 1
34:     i := $t02
35:     GOTO 26
36:     HALT
*/