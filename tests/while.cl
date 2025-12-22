int i
int j 

i := 0
while i < 5 do
    j := 0
    while j < 10 do
        j := j + 1
    done
    if i > 3 then
        i := 10
        break;
    fi
    i := i + 1
done

/*
1:      i := 0
2:      IF i LTI 5 GOTO 4
3:      GOTO 17
4:      j := 0
5:      IF j LTI 10 GOTO 7
6:      GOTO 10
7:      $t01 := j ADDI 1
8:      j := $t01
9:      GOTO 5
10:     IF i GTI 3 GOTO 12
11:     GOTO 14
12:     i := 10
13:     GOTO 17
14:     $t01 := i ADDI 1
15:     i := $t01
16:     GOTO 2
17:     HALT
*/