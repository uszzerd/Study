	.file	"3._if-else.c"
	.text
	.globl	test
	.type	test, @function
test:
.LFB11:
	.cfi_startproc
	testw	%di, %di
	js	.L6 // if(di < 0) goto L6 di = x
	cmpw	$9, %si 
	jg	.L5 // if(si > 9) goto L5, si =  y
	leal	12(%rsi), %eax // eax = rsi(si)
	ret // return eax
.L6:
	movl	%esi, %eax 
	orl	%edi, %eax // eax = x | y
	cmpw	$9, %si 
	jg	.L1 // if(y >= 10) goto L1 == if(y < 10) Keep going 
	movswl	%di, %eax // eax = x
	movswl	%si, %esi // esi = y
	cltd // convert long to double 나눗셈 전 나옴, 나머지 edx에 저장
	idivl	%esi // eax / esi, 나머지 ebx
	ret
.L5:
	movl	%edi, %eax // eax = x
	imull	%esi, %eax // eax *= y
.L1:
	ret
	.cfi_endproc
.LFE11:
	.size	test, .-test
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	movl	$0, %eax // return 전 0만들기
	ret // eax 값 return
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.5.0 20240719 (Red Hat 11.5.0-11)"
	.section	.note.GNU-stack,"",@progbits
