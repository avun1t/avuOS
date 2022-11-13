#include <kernel/tasking/TaskManager.h>
#include <kernel/memory/kliballoc.h>
#include <kernel/kstddef.h>
#include <kernel/kstdio.h>
#include <kernel/pit.h>
#include <kernel/memory/paging.h>
#include <kernel/kmain.h>
#include <common/cstring.h>

Process *current_proc;
Process *kernel_proc;
uint32_t __cpid__ = 0;
bool tasking_enabled = false;

void kthread()
{
	tasking_enabled = true;
	kmain_late();
	while(1);
}

Process *TaskManager::process_for_pid(pid_t pid)
{
	Process *current = kernel_proc;

	do {
		if (current->pid() == pid) return current;
		current = current->next;
	} while (current != kernel_proc);

	return (Process *) nullptr;
}

void TaskManager::print_tasks()
{
	Process *current = kernel_proc;
	printf("Running processes: (PID, name, state, * = is current)\n");
	do{
		printf("%c[%d] '%s' %d\n", current == current_proc ? '*' : ' ', current->pid(), current->name().c_str(), current->state);
		current = current->next;
	}while(current != kernel_proc);
}

void TaskManager::preempt_now()
{
	if (!tasking_enabled) return;
	asm volatile("int $0x81");
}

pid_t TaskManager::get_new_pid()
{
	return ++__cpid__;
}

void TaskManager::init()
{
	kernel_proc = Process::create_kernel("yuku32", kthread);
	kernel_proc->next = kernel_proc;
	kernel_proc->prev = kernel_proc;
	current_proc = kernel_proc;
	kernel_proc->init();

	//pop all of the registers off of the stack and get started
	PANIC("Failed to init tasking", "Something went wrong..", true);
}

Process *TaskManager::current_process()
{
	return current_proc;
}

uint32_t TaskManager::add_process(Process *p)
{
	bool en = tasking_enabled;
	tasking_enabled = false;
	p->next = current_proc->next;
	p->next->prev = p;
	p->prev = current_proc;
	current_proc->next = p;
	tasking_enabled = en;
	return p->pid();
}

void TaskManager::notify(uint32_t sig)
{
	current_proc->notify(sig);
}

void TaskManager::kill(Process *p)
{
	if (process_for_pid(p->pid()) != NULL) {
		tasking_enabled = false;

		kfree((uint8_t *)p->stack);
		kfree(p);

		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->state = PROCESS_DEAD;

		tasking_enabled = true;
	}
}

void TaskManager::preempt()
{
	//push current_proc process' registers on to its stack
	asm volatile("push %eax");
	asm volatile("push %ebx");
	asm volatile("push %ecx");
	asm volatile("push %edx");
	asm volatile("push %esi");
	asm volatile("push %edi");
	asm volatile("push %ebp");
	asm volatile("push %ds");
	asm volatile("push %es");
	asm volatile("push %fs");
	asm volatile("push %gs");
	asm volatile("mov %%esp, %%eax":"=a"(current_proc->registers.esp));
	current_proc = current_proc->next;

	if (!current_proc->inited) {
		current_proc->init();
		return;
	}
	
	//pop all of next process' registers off of its stack
	asm volatile("mov %%eax, %%esp": :"a"(current_proc->registers.esp));
	asm volatile("pop %gs");
	asm volatile("pop %fs");
	asm volatile("pop %es");
	asm volatile("pop %ds");
	asm volatile("pop %ebp");
	asm volatile("pop %edi");
	asm volatile("pop %esi");
	asm volatile("movl %%eax, %%cr3": : "a"(current_proc->page_directory_loc)); //Load page directory for process
	asm volatile("pop %edx");
	asm volatile("pop %ecx");
	asm volatile("pop %ebx");
	asm volatile("pop %eax");
}