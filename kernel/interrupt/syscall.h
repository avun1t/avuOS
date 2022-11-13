#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#define SYS_EXIT 1
#define SYS_FORK 2
#define SYS_READ 3
#define SYS_WRITE 4

extern "C" void syscall_handler(Registers regs);
int handle_syscall(Registers &regs, uint32_t call, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif // __SYSCALL_H_