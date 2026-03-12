/* ============================================
 * Problem 1: Login Bypass (Buffer Overflow)
 * 
 * 컴파일: gcc -O1 -fno-stack-protector -no-pie -o prob1 prob1_login.c
 * 
 * 목표: 올바른 비밀번호를 모르는 상태에서 "Access Granted!" 를 출력시켜라.
 * 힌트: 스택 프레임의 지역 변수 배치를 생각해보자.
 * ============================================ */

#include <stdio.h>
#include <string.h>

void login() {
    int authenticated = 0;
    char password[8];

    printf("Enter password: ");
    gets(password);  /* 위험한 함수! 왜 위험한지 생각해보자 */

    if (authenticated) {
        printf("Access Granted!\n");
    } else {
        printf("Access Denied.\n");
    } 
}

int main() {
    login();
    return 0;
}

/*
gets
- gets@plt 에서 실제 호출될 때 PLT/GOT를 통해 libc의 gets로 간다.
  예:
    jmp QWORD PTR [rip + ...]
    push 0x2
  이건 호출/동적링킹 과정이고, 취약점의 원인은 아님.

- 실제 취약점 원인:
  gets(char *s)는 입력 길이 제한 없이 읽는다.
  그래서 password[8] 같은 작은 버퍼를 넘어서도 계속 쓴다.

- 내부 동작은 개념적으로:
    *dest++ = c;
  처럼 현재 위치에 쓰고 다음 주소로 이동하는 방식으로 이해하면 된다.

- 스택 버퍼 오버플로우가 나면
  password[8] 다음의 saved rbp, return address까지 덮을 수 있다.

- 함수가 끝날 때 ret는
    RIP = [RSP]
  로 동작하므로,
  overflow로 저장된 return address가 바뀌면
  원래 복귀 주소 대신 공격자가 덮어쓴 주소로 점프한다.

- "0x401175 -> 0x40117b 이므로 +6"은
  '주소값에 6을 더한다'가 아니라
  'return address를 0x40117b로 바꾼다'는 의미로 이해해야 한다.
  즉 원하는 주소를 스택의 return address 자리에 그대로 써 넣는 것이다.

- 정수값으로 입력하는 경우 4294967311 같은 수를 넣어서 되는 것은
  프로그램이 그 입력을 문자열로 받아서,
  내부에서 어떤 방식으로 메모리에 저장/해석하느냐에 따라 달라진다.
  단순히 "8byte 최대값" 논리만으로 설명하면 부정확하다.
  특히
    0x40117b = 4198779
  이고,
    4294967311 = 0x10000000f
  이라서 둘은 직접 같은 값이 아니다.
  따라서 그 값이 왜 먹혔는지는
  '자료형 변환', '하위 바이트만 사용', 'int/long 해석', 'little-endian 배치'
  중 무엇 때문인지 코드를 보고 따져야 한다.

- char = 1 byte 는 맞다.
  하지만
    "char 최대치 = 0b1111"
  는 틀리다.
  1 byte는 8 bit이므로 최대 비트패턴은:
    0b11111111
  이다.
  unsigned char 최대값은 255 (0xff)다.

- 그리고
    ff ff ff ff = 4,294,967,295
  는 4바이트 unsigned 값이다.
  8바이트 최대값은:
    ff ff ff ff ff ff ff ff
  = 18446744073709551615
  이다.

- 디버깅용 컴파일은 보통:
    gcc -g -O0
  를 쓴다.
  -Og도 디버깅 친화적 최적화 옵션이지만,
  실습/분석에서는 레이아웃이 덜 바뀌는 -O0이 더 직관적이다.
*/