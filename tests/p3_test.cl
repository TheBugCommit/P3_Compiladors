float total
int i

i := 1
total := (1.0 + 0 * 1) - i
while total < 1000.0 do
    if (i = 1) then
        total := total * 2
    else
        total := total - 1
    fi
    i := -i
    if total > 666.6 or total = 500.0 then
        i := 111
    fi
done
total

/*
1:      i := 1
2:      $t01 := I2F i
3:      $t01 := 1.0 SUBF $t01
4:      total := $t01
5:      IF total LTF 1000.0 GOTO 7
6:      GOTO 22
7:      IF i EQ 1 GOTO 9
8:      GOTO 12
9:      $t01 := total ADDF total
10:     total := $t01
11:     GOTO 14
12:     $t01 := total SUBF 1.0
13:     total := $t01
14:     $t01 := CHSI i
15:     i := $t01
16:     IF total GTF 666.6 GOTO 20
17:     GOTO 18
18:     IF total EQ 500.0 GOTO 20
19:     GOTO 21
20:     i := 111
21:     GOTO 5
22:     PARAM total
23:     CALL PUTF, 1
24:     HALT
*/