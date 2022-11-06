[bits 32]
[extern int_syscall_handler]
global syscall_handler
syscall_handler:
	push ebx
	push eax
	call int_syscall_handler
	pop eax
	pop ebx
	iret