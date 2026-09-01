	.file	"dentroLoop.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d "
.LC1:
	.string	"\nSum: %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -52(%rbp)
	movq	$0, -48(%rbp)
	jmp	.L2
.L3:
	movq	-48(%rbp), %rax
	movl	%eax, %edx
	movq	-48(%rbp), %rax
	movl	%edx, -32(%rbp,%rax,4)
	addq	$1, -48(%rbp)
.L2:
	cmpq	$3, -48(%rbp)
	jbe	.L3
	movq	$0, -40(%rbp)
	jmp	.L4
.L5:
	movq	-40(%rbp), %rax
	movl	-32(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movq	-40(%rbp), %rax
	movl	-32(%rbp,%rax,4), %eax
	addl	%eax, -52(%rbp)
	addq	$1, -40(%rbp)
.L4:
	cmpq	$3, -40(%rbp)
	jbe	.L5
	movl	-52(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 14.3.0-12ubuntu1~24~ppa1) 14.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
