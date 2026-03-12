	.file	"prob1_login.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter password: "
.LC1:
	.string	"Access Granted!"
.LC2:
	.string	"Access Denied."
	.text
	.globl	login
	.type	login, @function
login:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	leaq	-12(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	gets
	cmpl	$0, -4(%rbp) // gets에서 stack overflow 발생시켜서 -4(rbp) != 0 으로 만들어야함
	je	.L2
	movl	$.LC1, %edi
	call	puts
	jmp	.L4
.L2:
	movl	$.LC2, %edi
	call	puts
.L4:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	login, .-login
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	login
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.5.0 20240719 (Red Hat 11.5.0-11)"
	.section	.note.GNU-stack,"",@progbits
