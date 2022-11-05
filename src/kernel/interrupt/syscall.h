#ifndef __SYSCALL_H_
#define __SYSCALL_H_

struct syscall {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

extern "C" void int_syscall_handler(uint32_t eax, uint32_t ebx);

#endif // __SYSCALL_H_