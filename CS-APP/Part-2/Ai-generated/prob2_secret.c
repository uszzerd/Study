/* ============================================
 * Problem 2: Secret Decoder (Reverse Engineering)
 * 
 * 컴파일: gcc -O1 -o prob2 prob2_secret.c
 * 
 * 목표: 
 *   (A) gcc -O1 -S 로 어셈블리를 생성한 뒤, check_secret 함수를 분석하여
 *       "Correct!" 를 출력하는 입력값을 찾아라.
 *   (B) GDB에서 disas check_secret 으로도 확인해보자.
 * 
 * 힌트: leaq, imulq, cmpq 등의 명령어에 주목하라.
 * ============================================ */

#include <stdio.h>
#include <stdlib.h>

int check_secret(long x) {
    long a = x * 7;
    long b = a + 13;
    long c = b ^ 0xFF;  /* XOR 연산 */
    
    if (c == 0x1C4) {
        return 1;
    }
    return 0;
}

int main() {
    long input;
    printf("Enter the secret number: ");
    scanf("%ld", &input);
    
    if (check_secret(input)) {
        printf("Correct!\n");
    } else {
        printf("Wrong!\n");
    }
    return 0;
}


/*
최적화 파일을 보면
	movl	$0, %eax
	call	__isoc99_scanf
	movl	$.LC2, %edi
	call	puts

.LC2:
	.string	"Wrong!"
	.text
	.globl	main
	.type	main, @function

사실상 return 0을 가정하고 컴파일을 진행했다.
-> 입력값 x = 302 / 7 이 정답인데 정수형이라 조건을 만족하는 값이 없다. round 되기 때문

그렇기에 컴파일러가 해당 코드는 항상 0을 return 한다고 가정하고 진행한다.

*/
