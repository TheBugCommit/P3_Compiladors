int x
x := 0

do 
    x := 2 * 0
    do 
        x := x + 1
        if x = 2 then
            break;
        fi
    until x < 1
until x < 4

/*
1:      x := 0
2:      x := 0
3:      $t01 := x ADDI 1
4:      x := $t01
5:      IF x EQ 2 GOTO 7
6:      GOTO 8
7:      GOTO 10
8:      IF x LTI 1 GOTO 3
9:      GOTO 10
10:     IF x LTI 4 GOTO 2
11:     GOTO 12
12:     HALT
*/