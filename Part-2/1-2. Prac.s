	.file	"1. Prac.c"
	.text
	.globl	mult2
	.type	mult2, @function
mult2:
.LFB13:
	.cfi_startproc
	movq	%rdi, %rax
	imulq	%rsi, %rax
	ret
	.cfi_endproc
.LFE13:
	.size	mult2, .-mult2
	.globl	multstore
	.type	multstore, @function
multstore:
.LFB12:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdx, %rbx
	call	mult2
	movq	%rax, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	multstore, .-multstore
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"2 * 3 --> %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	subq	$24, %rsp // 공간 확보, rsp = rsp - 24 , 24바이트 만큼의 공간 확보
	.cfi_def_cfa_offset 32
	leaq	8(%rsp), %rdx // rdx를 *(rsp + 8) 위치에 저장
	movl	$3, %esi // 두번째 인자
	movl	$2, %edi // 첫번째 인자
	call	multstore // (esi, edi, &rdx)
	movq	8(%rsp), %rsi // rsp 위치에 저장된 8바이트를 rsi에 넣는다
	movl	$.LC0, %edi // string, LC = local constant
	movl	$0, %eax // eax = 0, 부동소수점 인자 개수를 넣음, printf에서 부동소수점 사용 x
	call	printf // printf(edi, rsi)
	movl	$0, %eax // return 0
	addq	$24, %rsp // 사용한 공간 다시 돌려놓기
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.5.0 20240719 (Red Hat 11.5.0-11)"
	.section	.note.GNU-stack,"",@progbits