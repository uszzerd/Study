/* ============================================
 * Problem 2: Canary Leak — Format String + Buffer Overflow
 * 
 * 컴파일: gcc -O1 -no-pie -o adv2 adv2_canary.c
 *         (주의: -fno-stack-protector를 붙이지 않는다!)
 * 
 * 목표: 이번엔 stack canary가 켜져 있다.
 *       하지만 프로그램에 format string vulnerability가 있다.
 *       1단계: format string으로 canary 값을 leak 하라.
 *       2단계: leak한 canary를 올바른 위치에 넣어서
 *              buffer overflow로 secret()을 호출하라.
 * 
 * 힌트: printf(user_input)이 왜 위험한지 생각해보자.
 *       %lx 포맷 스페시파이어로 스택 내용을 읽을 수 있다.
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void secret() {
    printf("=== CANARY BYPASSED ===\n");
    printf("You defeated the stack protector!\n");
}

void echo() {
    char name[32];
    
    printf("What is your name? ");
    fgets(name, sizeof(name), stdin);
    
    /* 위험! printf의 첫 번째 인자로 사용자 입력을 직접 전달 */
    printf("Hello, ");
    printf(name);
    printf("\n");
}

void vulnerable() {
    char buf[32];
    
    printf("Leave a message: ");
    gets(buf);
    printf("Received: %s\n", buf);
}

int main() {
    printf("=== Canary Leak Challenge ===\n");
    printf("Phase 1: Leak the canary.\n");
    echo();
    
    printf("\nPhase 2: Exploit with the canary.\n");
    vulnerable();
    
    return 0;
}
