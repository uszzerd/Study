#include <stdio.h>
#include <stddef.h>
#include <stdlib.h> // atoi, strtol

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    for(int i = 0; i < len; i++)
        printf(" %.2x",start[i]);
    printf("\n");
}

void show_int(int x)
{
    printf("int : ");
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x)
{
    printf("float : ");
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x)
{
    printf("pointer : ");
    show_bytes((byte_pointer) &x, sizeof(void*));
}

// 입력을 int, float 형으로 입력
int main(int argc, char **argv)
{
    int i = atoi(argv[1]);
    float f = atof(argv[2]);
    int* ip = &i;
    float* fp = &f;
    
    show_int(i);
    show_float(f);
    show_pointer(ip);
    show_pointer(fp);

    return 0;
}   