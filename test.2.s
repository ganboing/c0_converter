	.file	"test.2.c"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	call	abort
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.text
	.p2align 4,,15
	.globl	test
	.type	test, @function
test:
.LFB1:
	.cfi_startproc
	subl	$12, %esp
	.cfi_def_cfa_offset 16
	call	commit
	.cfi_endproc
.LFE1:
	.size	test, .-test
	.comm	y,20,4096
	.comm	x,20,4096
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
