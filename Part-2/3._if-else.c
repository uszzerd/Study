#include <stdio.h>


short test(short x, short y)
{
    short val = y + 12;
    if(x < 0)
    {
        if(y < 10)
            val = x / y;
        else
            val = x | y;
    }
    else if(y >= 10)
        val = x * y;
    return val;
}

int main()
{
    short x = 10;
    short y = 2;
    short val = (x, y);
    
    return 0;
}