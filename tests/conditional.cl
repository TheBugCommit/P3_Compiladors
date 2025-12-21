int x
if 2 > 1 then 
    if false then
        x := 1 
    fi
else 
    x := 2 
fi

/*
1:      IF 2 GTI 1 GOTO 3
2:      GOTO 7
3:      IF 0 NE 0 GOTO 5
4:      GOTO 6
5:      x := 1
6:      GOTO 8
7:      x := 2
8:      HALT
*/