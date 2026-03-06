#include <stdio.h>

// case 1
float sum_elements(float a[], unsigned length)
{
    int i;
    float result = 0;
    for (i = 0; i<=length-1;i++)
        result += a[i];
    return result;
}
// length는 unsigned 이기에 length - 1 = Neg Overflow가 발생한다.
// 1. i <= length - 1 부분에서 implicit 하게 signed int로 바뀌길 기도하거나
// 2. unsigned 가 아닌 signed로 입력을 받으면 된다.


// case 2
size_t strlen(const char *s);
int strlonger(char *s, char *t)
{
    return strlen(s) - strlen(t) > 0;
}

// strlen이 return 할 때 size_t = unsigned integer로 반환된다.
// 만약 3 - 5 = ?  로 Neg Overflow로 매우 큰 숫자가 나올 것 이다.
// size_t -> int로 바꾸면 해결 가능
// 혹은 -가 아닌 <, > 형식으로 비교하면 false, true로 정상 작동 가능


// case 3
int tadd_ok(int x, int y)
{
    int temp = x + y;
    if(x < 0 && y < 0 && temp >= 0) // Neg Overflow
        return 0;
    if(x > 0 && y > 0 && temp <= 0) // Pos Overflow
        return 0;
    return 1;
}
// TMIN + TMIN = 0, TMAX + TMAX = 0
// 0 + TMIN = TMIN, Max도 마찬가지 이므로 x와 Y가 서로 부호가 다르다면 Overflow x
// Neg, Pos Overflow일 경우 0, Overflow 없이 연산 가능할 경우 1


// case 4
int tadd_ok(int x, int y)
{
    int sum = x + y;
    return (sum - x == y) && (sum - y == x);
}
// 해당 코드는 아예 overflow를 예상하지 않고 코드를 작성하였다.


// case 5
int tsub_ok(int x, int y)
{
    return tadd_ok(x, -y);
}

// TMIN = -2^(w-1)
// TMAX =  2^(w-1) - 1
// y = TMIN일 경우, tsub_ok에서 -y를 만드는 순간 y == TMIN이 깨지게됨
// two's complement에서 -TMIN = TMIN 이기 때문에 단순 변환은 항상 안전하지 않다.
// 기존 작성한 tadd_ok로는 판별이 불가능함으로 새로 작성해야 한다

int tadd_ok(int x, int y)
{
    int sum = x + y;
    return !(((x ^ sum) & (y ^ sum)) >> 31);
}
// x와 y가 덧셈 후 부호가 sum result와 다르다면 overflow
// int는 32bit 이므로 31bit shift 시 sign bit를 검출할 수 있음


// case 6 (two's complement 가정)
int tmult_ok(int x, int y)
{
    int p = x * y;
    return !x || p/x == y;
}

// x != 0 이고 overflow가 없으면, 수학적 곱 P = x*y 가 int 범위에 들어가며
// p = P 이므로 p/x == y 가 성립한다
// p ≡ P (mod 2^w)
// P = p + t*2^w
// overflow 없음 ⇔ t = 0 ⇔ P = p
// overflow 있음 ⇔ t ≠ 0
// x==0이면 p/x가 0으로 나누기가 되므로, overflow도 없으니 바로 OK로 처리
// longlong 변수를 사용하면 나누기 없이 & 연산자로 검출 가능

// 앞으로 Shift (x << w)시 x * 2^w 연산 진행
// x * 24 = (x << 5) - (x << 3) 과 같이 shift 연산으로 쉽게 구현 가능


// case 7 (Shift로 나눗셈 만들기)
float div16(int x)
{
    return x >> 3;
}
// two's complement 나눗셈은 분기가 2개로 나뉜다 (dividend, divisor)
// C division rule: quotient is truncated toward 0 (division에서의 trunc 정의)
// q = trunc(x / y);

// 1. sign = sign(x) XOR sign(y)
// 2. |x| / |y| 로 몫 계산
// 3. sign 적용

// sign(x) XOR sign(y) -> sign = (x < 0) ^ (y < 0);
// INT_MIN / -1 일 경우 Overflow가 발생한다. (undefined behavior)

// Truncation이란 일정 부분의 bit를 버리는 것이다
// 1~21 -> 1~16
// Sign Extention은 앞의 Sign bit를 처음의 Sign bit와 같은 bit를 앞에 추가하는 것이다.
// 1...11(16) -> 111...11(20)  == Value 유지


/*
Floting Point
IEEE 754 normalized number
x = (-1)^s x (1.f) x 2^(E)

s = sign bit, 부호
f = fraction, 실제 사용 값
E : 1. normalized, 2. denormalized 의 값이 다름
1. exp != 0...0 -> e-Bais
2. exp == 0...0 -> 1-Bais

exponent = exp = 소수점(.)을 어디로 이동시킬지 결정하는 값 

exp == 1...1 && frac == 0...0 -> sign bit에 따라 ∞, -∞ 로 나뉨
exp == 1...1 && frac != 0...0 -> Not a Number, NaN 범위

*/


/*
case 8

A. n비트 비율을 사용하는 어떤 부동소수점 형식에서 정확하게 표시할 수 없는 최소
양의 정수 값을 식으로 표시하시오. 지수 필드는 k비트이다.
-> Overflow가 일어나는 최소 정수 값

sign | exponent(k bit) | fraction(n bit)
sign bit = 1 // 부호 비트로 상관 X
exponent bit = k // k는 충분히 크다고 가정
hidden bit = 1 // 실수는 1.xxx로 시작하며 앞 1은 저장하지 않고 implicit하게 가정
fraction field bit = n

정수 = 2^(n+1), 최대 저장 가능
-> overflow가 일어나는 양의 정수 2^(n+1) + 1

case 9 
B. float 형식으로 n = 23일 경우는?

위의 결과에 따라 2^(23+1)+1, 2^24+1 일 경우
*/


/*
Rounding

IEEE 754 

mantissa가 3bit이라 가정했을 때,
1.101101_2 -> 1.101 or 1.110 으로 변환해야 한다.

이때 Rounding Rule이 사용된다.

Round to Nearest, Ties to Even
가까운 값으로 반올림 하고, 중간이면 마지막 비트를 짝수로 만든다.

1.1011 → 1.110
1.1001 → 1.100

1.10100 -> 중간값
1.100 선택
1.110

다른 Rounding 방식
Round toward zero
Round toward +∞
Round toward −∞

float 기준
26bit -> 23 bit
3bit는 Rounding 진행

실제 구현 (Guard / Round / Sticky bit)
mantissa | G | R | S

Guard	mantissa 바로 다음 비트
Round	그 다음 비트
Sticky	이후 모든 비트 OR

case 1
G = 0 -> 유지

case 2
G = 1
R = 1 or S = 1 
-> 올림

case 3 (tie)
G = 1
R = 0
S = 0
-> 정확히 중간값
mantissa 마지막 비트가
짝수 → 유지
홀수 → 올림

ex)
1.1010 | 1 | 0 | 1
GRS = 101
G = 1, S = 1 -> 올림
=> 1.1011

Sticky는 뒤에 버려지는 비트 중 하나라도 1이면 1이 된다. -> or 연산

*/
