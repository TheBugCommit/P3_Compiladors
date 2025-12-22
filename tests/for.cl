int x[10], y
y := 9
for i in 1..y do
    for j in 0..2 do
        x[i] := i + j    
    done
done

/*
1:      y := 9
2:      i := 1
3:      $t01 := i
4:      IF i LEI y GOTO 6
5:      GOTO 27
6:      j := 0
7:      $t02 := j
8:      $t03 := i ADDI j
9:      $t04 := i MULI 4
10:     $t04 := 25 ADDI $t04
11:     x[$t04] := $t03
12:     $t02 := $t02 ADDI 1
13:     j := $t02
14:     $t03 := i ADDI j
15:     $t04 := i MULI 4
16:     $t04 := 25 ADDI $t04
17:     x[$t04] := $t03
18:     $t02 := $t02 ADDI 1
19:     j := $t02
20:     $t03 := i ADDI j
21:     $t04 := i MULI 4
22:     $t04 := 25 ADDI $t04
23:     x[$t04] := $t03
24:     $t01 := $t01 ADDI 1
25:     i := $t01
26:     GOTO 4
27:     HALT
*/