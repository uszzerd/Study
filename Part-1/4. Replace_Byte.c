#include <stdio.h>
#include <stdlib.h>

typedef unsigned char* byte; 

void replace_byte(byte x, int i, unsigned char* b);

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("usage : %s <byte_index> <hex_byte>\n", argv[0]);
        return 1;
    }
    int x = 0x12345678;
    printf("before : %.8x\n", x);
    byte bx = (byte)&x;

    replace_byte(bx, atoi(argv[1]), argv[2]);
    printf("after  : %.8x\n", x);

    return 0;
}

void replace_byte(byte x, int i, unsigned char* b)
{
    if (i < 0 || i >= sizeof(int)) 
    {
        printf("Range Out\n");
        return;
    }

    char *endptr;
    int value = strtol(b, &endptr, 16);

    if (*endptr != '\0' || value < 0 || value > 0xFF) 
    {
        printf("invalid hex byte\n");
        return;
    }

    x[i] = (unsigned char)value;
}

// 목표 : w비트 워드를 0부터 w/8-1 까지 번호를 매긴다고 가정, unsigned의 i번째 byte를 b로 교체한 값을 return
// Little endian 기준, byte기 때문에 00~ff를 넘어가는 수는 Error 발생