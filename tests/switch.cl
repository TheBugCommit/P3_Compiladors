int x

switch(3)  
    case 1: 
        switch(1)  
            case 1:
                x := 10 
                break;
            default:
                break;
        fswitch
        x := 1 
        break; 
    case 3:
        x := 2
        break;  
    default:
         break; 
fswitch

bool a
a := 1 = 2 or true
switch(a)
    case true:
        x := 1
        break;
    case false:
        x := 2
        break;
fswitch

/*
1:      GOTO 15
2:      GOTO 7
3:      x := 10
4:      GOTO 9
5:      GOTO 9
6:      GOTO 9
7:      IF 1 EQ 1 GOTO 3
8:      GOTO 5
9:      x := 1
10:     GOTO 18
11:     x := 2
12:     GOTO 18
13:     GOTO 18
14:     GOTO 18
15:     IF 3 EQ 1 GOTO 2
16:     IF 3 EQ 3 GOTO 11
17:     GOTO 13
18:     IF 1 EQ 2 GOTO 22
19:     GOTO 20
20:     IF 1 NE 0 GOTO 22
21:     GOTO 24
22:     $t01 := 1
23:     GOTO 25
24:     $t01 := 0
25:     a := $t01
26:     GOTO 32
27:     x := 1
28:     GOTO 34
29:     x := 2
30:     GOTO 34
31:     GOTO 34
32:     IF a EQ 1 GOTO 27
33:     IF a EQ 0 GOTO 29
34:     HALT
*/