bits 32

global start
extern main

section .text

start:
	cli
	mov esp, stack
	call main
	hlt

section .bss

resb 8192
stack:
