#ifndef __AVUOS_UNISTD_H_
#define __AVUOS_UNISTD_H_

extern "C" {
	int syscall(int call, int arg1 = 0, int arg2 = 0, int arg3 = 0);
	__attribute__((noreturn)) void _exit(int status);
}

#endif // __AVUOS_UNISTD_H_