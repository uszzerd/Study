#include <stdio.h>

int main()
{
    int test = -1;
    test = (test >> 1);
    if(test == -1)
        printf("Arthmetic Shift, %i\n",test);
    else
        printf("Logical Shift, %i\n", test);
    return 0;
}




// 목표 : 해당 머신이 Arithmetic shift인지 확인

/*
Logical right shift    : 오른쪽으로 shift할 때 왼쪽 빈 자리를 0으로 채움
Arithmetic right shift : 오른쪽으로 shift할 때 왼쪽 빈 자리를 sign bit로 채움

-1을 사용하면 어떤 방식의 shift인지 쉽게 확인할 수 있다.
2의 보수 표현에서 -1은 모든 비트가 1이기 때문이다.

예: 32-bit int
-1 = 11111111 11111111 11111111 11111111

1) Arithmetic right shift
   -1 >> 1
= 11111111 11111111 11111111 11111111
= -1

2) Logical right shift
   -1 >> 1
= 01111111 11111111 11111111 11111111
= 큰 양수

왜 right shift를 테스트하나?
- sign bit 처리 방식이 결과에 바로 드러나기 때문
- left shift는 둘 다 오른쪽에 0이 들어가므로 구분에 적합하지 않음
*/