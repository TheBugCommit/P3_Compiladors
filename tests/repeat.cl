int x
x := 2

repeat x do
    switch(x)
        case 1:
            x := x + 2
            repeat x do
                1 + 1
            done
            break;
        default:
            x := x + 1
    fswitch
done

/*
1:      x := 2
2:      $t01 := 0
3:      $t02 := x
4:      IF $t01 LTI $t02 GOTO 6
5:      GOTO 25
6:      GOTO 21
7:      $t03 := x ADDI 2
8:      x := $t03
9:      $t03 := 0
10:     $t04 := x
11:     IF $t03 LTI $t04 GOTO 13
12:     GOTO 17
13:     PARAM 2
14:     CALL PUTI, 1
15:     $t03 := $t03 ADDI 1
16:     GOTO 11
17:     GOTO 23
18:     $t04 := x ADDI 1
19:     x := $t04
20:     GOTO 23
21:     IF x EQ 1 GOTO 7
22:     GOTO 18
23:     $t01 := $t01 ADDI 1
24:     GOTO 4
25:     HALT
*/