#ifndef __SYSCALL_H_
#define __SYSCALL_H_

#define SYS_EXIT 60

extern "C" void syscall_handler(registers regs);
int handle_syscall(registers &regs, uint32_t call, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif // __SYSCALL_H_