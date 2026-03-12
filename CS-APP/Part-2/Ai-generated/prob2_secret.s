	.file	"prob2_secret.c"
	.text
	.globl	check_secret
	.type	check_secret, @function
check_secret:
.LFB22:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE22:
	.size	check_secret, .-check_secret
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Enter the secret number: "
.LC1:
	.string	"%ld"
.LC2:
	.string	"Wrong!"
	.text
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leaq	8(%rsp), %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	movl	$.LC2, %edi
	call	puts
	movl	$0, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.5.0 20240719 (Red Hat 11.5.0-11)"
	.section	.note.GNU-stack,"",@progbits
