	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14	sdk_version 10, 14
	.section	__TEXT,__literal16,16byte_literals
	.p2align	4               ## -- Begin function main
LCPI0_0:
	.long	1127219200              ## 0x43300000
	.long	1160773632              ## 0x45300000
	.long	0                       ## 0x0
	.long	0                       ## 0x0
LCPI0_1:
	.quad	4841369599423283200     ## double 4503599627370496
	.quad	4985484787499139072     ## double 1.9342813113834067E+25
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3
LCPI0_2:
	.quad	4696837146684686336     ## double 1.0E+6
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	pushq	%rax
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	leaq	L_.str(%rip), %rdi
	xorl	%eax, %eax
	callq	_printf
	movl	$1, %r13d
	leaq	L_.str.1(%rip), %r14
	.p2align	4, 0x90
LBB0_1:                                 ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB0_7 Depth 2
                                        ##     Child Loop BB0_9 Depth 2
                                        ##     Child Loop BB0_11 Depth 2
	movl	$1048576, %eax          ## imm = 0x100000
	xorl	%edx, %edx
	divl	%r13d
	movl	%eax, %r12d
	callq	_clock
	movq	%rax, %r15
	testl	%r12d, %r12d
	jle	LBB0_2
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$262144, %r13d          ## imm = 0x40000
	jbe	LBB0_5
## %bb.4:                               ##   in Loop: Header=BB0_1 Depth=1
	xorl	%eax, %eax
	xorl	%ebx, %ebx
	jmp	LBB0_11
	.p2align	4, 0x90
LBB0_2:                                 ##   in Loop: Header=BB0_1 Depth=1
	xorl	%ebx, %ebx
	jmp	LBB0_12
	.p2align	4, 0x90
LBB0_5:                                 ##   in Loop: Header=BB0_1 Depth=1
	movl	%r12d, %eax
	andl	$2097148, %eax          ## imm = 0x1FFFFC
	movq	%r13, %xmm0
	pshufd	$68, %xmm0, %xmm0       ## xmm0 = xmm0[0,1,0,1]
	leal	-4(%rax), %esi
	movl	%esi, %edx
	shrl	$2, %edx
	incl	%edx
	movl	%edx, %ecx
	andl	$7, %ecx
	pxor	%xmm1, %xmm1
	pxor	%xmm2, %xmm2
	cmpl	$28, %esi
	jb	LBB0_8
## %bb.6:                               ##   in Loop: Header=BB0_1 Depth=1
	subl	%ecx, %edx
	pxor	%xmm1, %xmm1
	pxor	%xmm2, %xmm2
	.p2align	4, 0x90
LBB0_7:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	addl	$-8, %edx
	jne	LBB0_7
LBB0_8:                                 ##   in Loop: Header=BB0_1 Depth=1
	testl	%ecx, %ecx
	je	LBB0_10
	.p2align	4, 0x90
LBB0_9:                                 ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	paddq	%xmm0, %xmm1
	paddq	%xmm0, %xmm2
	decl	%ecx
	jne	LBB0_9
LBB0_10:                                ##   in Loop: Header=BB0_1 Depth=1
	paddq	%xmm2, %xmm1
	pshufd	$78, %xmm1, %xmm0       ## xmm0 = xmm1[2,3,0,1]
	paddq	%xmm1, %xmm0
	movq	%xmm0, %rbx
	cmpl	%eax, %r12d
	je	LBB0_12
	.p2align	4, 0x90
LBB0_11:                                ##   Parent Loop BB0_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	incl	%eax
	addq	%r13, %rbx
	cmpl	%r12d, %eax
	jb	LBB0_11
LBB0_12:                                ##   in Loop: Header=BB0_1 Depth=1
	callq	_clock
	subq	%r15, %rax
	movq	%rax, %xmm0
	punpckldq	LCPI0_0(%rip), %xmm0 ## xmm0 = xmm0[0],mem[0],xmm0[1],mem[1]
	subpd	LCPI0_1(%rip), %xmm0
	haddpd	%xmm0, %xmm0
	divsd	LCPI0_2(%rip), %xmm0
	movb	$1, %al
	movq	%r14, %rdi
	movl	%r13d, %esi
	movq	%rbx, %rdx
	callq	_printf
	incq	%r13
	cmpq	$17, %r13
	jne	LBB0_1
## %bb.13:
	xorl	%eax, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"#pages\t proc\t sum\n "

L_.str.1:                               ## @.str.1
	.asciz	"%d\t %.6f\t %ld\n"


.subsections_via_symbols
