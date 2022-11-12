[bits 32]
[extern syscall_handler]
global asm_syscall_handler
asm_syscall_handler:
	push 0
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call syscall_handler
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 4
	iret