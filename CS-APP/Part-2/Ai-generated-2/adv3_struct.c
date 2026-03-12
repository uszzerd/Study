/* ============================================
 * Problem 3: Struct Smash — Function Pointer Hijack
 * 
 * 컴파일: gcc -O1 -fno-stack-protector -no-pie -o adv3 adv3_struct.c
 * 
 * 목표: 이 프로그램은 struct 안의 function pointer (함수 포인터)를
 *       통해 간접 호출을 수행한다.
 *       struct의 메모리 배치를 분석하여, 정상적인 입력만으로
 *       admin_panel()이 호출되도록 만들어라.
 * 
 * 이 문제는 return address를 건드리지 않는다.
 * struct의 필드 배치와 padding을 정확히 이해해야 한다.
 * 
 * 힌트: struct의 각 필드가 메모리에 어떤 순서로,
 *       어떤 alignment로 배치되는지 생각하라.
 *       sizeof와 offsetof를 활용하거나 GDB에서 직접 확인하라.
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef void (*action_fn)(void);

void user_greeting() {
    printf("Welcome, regular user!\n");
}

void admin_panel() {
    printf("=== ADMIN PANEL ACCESSED ===\n");
    printf("You hijacked the function pointer!\n");
}

struct User {
    char name[24];
    int privilege;       /* 0 = user, 1 = admin */
    action_fn action;    /* function pointer */
};

void process_user(struct User *u) {
    printf("Processing user: %s\n", u->name);
    printf("Privilege level: Q%d\n", u->privilege);
    
    /* 함수 포인터를 통한 간접 호출 */
    printf("Executing action...\n");
    u->action();
}

int main() {
    struct User user;
    
    /* 초기화 */
    user.privilege = 0;
    user.action = user_greeting;
    
    printf("=== Struct Smash Challenge ===\n");
    printf("Enter username: ");
    gets(user.name);
    
    /* privilege가 변경됐는지 검사 */
    if (user.privilege != 0) {
        printf("[ALERT] Privilege corruption detected! (val=%d)\n", 
               user.privilege);
    }
    
    process_user(&user);
    
    return 0;
}


/*
gdb로 확인시 

test esi, esi
jne ...
로 실제 0x18 부근의 data는 0이여야 한다.

\x00 로 입력 시 get은 char 0이 아닌 digit 0으로 인식

8byte 정렬을 확인
0x18 ~ 20 이므로 총 48byte사용, 0x20 -> ptr로 주소 저장
즉 8의 배수 32byte이 후 ptr 저장

rdi = rsp로 복사 후 process_user 실행
process 에서 rbx = rbi로 복사

총 32byte 입력 후 원하는 주소 입력 시 process_user에서 call ... [rbx+0x20]으로 admin_panel 작동
-> Accessed

*/