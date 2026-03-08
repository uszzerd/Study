#include <stdio.h>


void multstore(long, long, long*);
long mult2(long, long);

int main()
{
    long d;
    multstore(2,3,&d);
    printf("2 * 3 --> %ld\n", d);
    return 0;
}

void multstore(long a, long b, long* dest)
{
    long t = mult2(a, b);
    *dest = t;
}

long mult2(long a, long b)
{
    long s = a * b;
    return s;
}


// gcc -Og -S 파일이름 -> assmbly 파일 생성
// gcc -Og -C 파일이름 -> binary 형식이라 직접 볼 수 없음
