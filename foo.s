	.file	"foo.c"
	.intel_syntax noprefix
# GNU C17 (Ubuntu 11.4.0-1ubuntu1~22.04) version 11.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 11.4.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.24-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -masm=intel -mtune=generic -march=x86-64 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata
.LC0:
	.string	"OK"
	.text
	.globl	foo1
	.type	foo1, @function
foo1:
.LFB0:
	.cfi_startproc
	endbr64	
	push	rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp	#,
	.cfi_def_cfa_register 6
# foo.c:3:   printf("OK\n");
	lea	rax, .LC0[rip]	# tmp82,
	mov	rdi, rax	#, tmp82
	call	puts@PLT	#
# foo.c:4: }
	nop	
	pop	rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE0:
	.size	foo1, .-foo1
	.section	.rodata
.LC1:
	.string	"%d\n"
	.text
	.globl	foo2
	.type	foo2, @function
foo2:
.LFB1:
	.cfi_startproc
	endbr64	
	push	rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp	#,
	.cfi_def_cfa_register 6
	sub	rsp, 16	#,
	mov	DWORD PTR -4[rbp], edi	# x, x
	mov	DWORD PTR -8[rbp], esi	# y, y
# foo.c:7:   printf("%d\n", x+y);
	mov	edx, DWORD PTR -4[rbp]	# tmp83, x
	mov	eax, DWORD PTR -8[rbp]	# tmp84, y
	add	eax, edx	# _1, tmp83
	mov	esi, eax	#, _1
	lea	rax, .LC1[rip]	# tmp85,
	mov	rdi, rax	#, tmp85
	mov	eax, 0	#,
	call	printf@PLT	#
# foo.c:8: }
	nop	
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE1:
	.size	foo2, .-foo2
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
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
