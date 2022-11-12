#ifndef TASKING_H
#define TASKING_H

#include <kernel/kstddef.h>
#include "Process.h"

#define PROCESS_ALIVE 0
#define PROCESS_ZOMBIE 1
#define PROCESS_DEAD 2

#define SIGTERM 15
#define SIGILL 4
#define SIGSEGV 11

void init_tasking();
void print_tasks();
uint32_t add_process(Process *p);
Process *get_current_process();
void preempt_now();
Process *create_process(char *name,  uint32_t loc);
Process *get_process(uint32_t pid);
extern "C" void preempt();
void notify(uint32_t sig);
void kill(Process *p);

extern void _iret();

#endif