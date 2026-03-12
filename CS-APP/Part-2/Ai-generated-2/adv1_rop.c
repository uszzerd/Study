/* ============================================
 * Problem 1: ROP Chain — Gadget Chaining
 * 
 * 컴파일: gcc -O1 -fno-stack-protector -no-pie -o adv1 adv1_rop.c
 * 
 * 목표: NX bit가 켜져 있어서 스택에서 코드 실행이 불가능하다.
 *       ROP gadget을 연결하여 win(0xDEAD, 0xBEEF)를 호출하라.
 *       두 인자가 모두 맞아야 "VICTORY!" 가 출력된다.
 * 
 * 힌트: x86-64 calling convention에서 첫 번째 인자는 %rdi,
 *       두 번째 인자는 %rsi 에 들어간다.
 *       objdump -d 또는 ROPgadget으로 pop rdi; ret 과
 *       pop rsi; ret 가젯을 찾아라.
 * ============================================ */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 이 함수들은 유용한 ROP gadget을 바이너리에 남기기 위한 것 */
void gadget_box() {
    /* pop rdi; ret 에 해당하는 바이트 시퀀스 */
    asm volatile(
        "pop %rdi\n"
        "ret\n"
    );
}

void gadget_box2() {
    /* pop rsi; ret 에 해당하는 바이트 시퀀스 */
    asm volatile(
        "pop %rsi\n"
        "ret\n"
    );
}

void win(long a, long b) {
    if (a == 0xDEAD && b == 0xBEEF) {
        printf("VICTORY! You mastered ROP chaining!\n");
    } else {
        printf("win() called, but wrong args: a=0x%lx, b=0x%lx\n", a, b);
    }
}

void vulnerable() {
    char buf[32];
    printf("Input: ");
    gets(buf);
    printf("You said: %s\n", buf);
}

int main() {
    printf("=== ROP Challenge ===\n");
    printf("Hint: win(0xDEAD, 0xBEEF) to get the flag.\n");
    vulnerable();
    return 0;
}


/*
ROP (Return-Oriented Programming)
pop을 rsp에 저장해 두는 방식을 이용해서 rsp에 내가 원하는 값 + 주소를 넣어두는 기법

pop rdi(first argument), rsi(second argument)  = $rsp + 0x08 -> rsp += 8
ret => rsp += 8

해당 조건을 이용

gadget을 이용해서
pop
ret

순으로 있는 구조를 찾음 -> 원하는 값 및 주소를 넣을 수 있음

해당 문제에서는 win() 에 가야 하는데, rdi = 0xdead, rsi = 0xbeef 를 갖고 있어야 ROP chaining이 print됨

*/
