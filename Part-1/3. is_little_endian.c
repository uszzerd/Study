#include <stdio.h>

typedef unsigned char* byte_pointer;

int main()
{
    int x = 1;
    printf("int : ");
    
    byte_pointer temp = (byte_pointer)&x;
    int result = (temp[0] == 1);

    for(int i = 0; i < sizeof(int); i++)
        printf(" %.2x", temp[i]); // 2자리 hex로 출력

    printf("\nresult : %i \n", result);
}

// 목표 : Little endian인지 확인 true = 1, false = 0 return
/*
Little endian : LSB가 낮은 주소에 저장 -> MSB가 높은 주소에 저장
Big endian : MSB가 낮은 주소에 저장

MSB : Most Significant Byte, 가장 중요한 정보
LSB : Least Significant Byte, 가장 안중요한 정보

ex)
16진수, 32-bit integer 가정
입력 : 1
00 00 00 01 : Big endian
01 00 00 00 : Little endian

출력해보면 temp[0] = 1로 C는 Little endian이다
 -> 라고 생각했는데 찾아보니 나의 CPU/플랫폼이 little-endian 이다.

 C는 특정 endian을 갖지 않고, Hardware Architecture가 endian을 갖게 되는데, 대부분의 x86,x86-64 : little-endian이다.

*/