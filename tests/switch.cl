int x

/*switch(1+2)  
    case 1: 
        x := 1 
        break; 
    case 3:
        x := 2
        break;  
    default:
         break; 
fswitch*/
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
/*
bool a
a := 1 = 2 or true
switch(a)
    case true:
        x := 1
        break;
    case false:
        x := 2
        break;
    default:
        break;
fswitch*/