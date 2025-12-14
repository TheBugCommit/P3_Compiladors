/*int x, y, z
x := 1
y := 1
repeat 4 do
    z := x
    repeat 3 do 
        y := x * 2
    done
    x := x + 1
    y := y + x
done*/

int x
x := 2

repeat x do
    switch(x)
        case 1:
            x := x + 2
            switch(x)
                case 2:
                    repeat x do
                        1 + 1
                    done
                    break;
            fswitch
            break;
        case 1:
            x := x + 2
            break;
        case 1:
            x := x + 2
            break;
    fswitch
done

/*
x := 1
y := 1
$t01 := 0
IF $t01 LTI 4 GOTO 6
GOTO 20
z := x
$t02 := 0
IF $t02 LTI 3 GOTO 10
GOTO 14
$t03 := x ADDI x
y := $t03
$t02 := $t02 ADDI 1
GOTO 8
$t02 := x ADDI 1
x := $t02
$t02 := y ADDI x
y := $t02
$t01 := $t01 ADDI 1
GOTO 4
HALT
*/