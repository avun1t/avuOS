/*
	This file is part of avuOS.
	avuOS is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	avuOS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with avuOS.  If not, see <https://www.gnu.org/licenses/>.
	Copyright (c) avun1t 2022. All rights reserved.
*/

#include <kernel/memory/kliballoc.h>
#include <kernel/filesystem/VFS.h>
#include <common/defines.h>
#include <kernel/device/TTYDevice.h>
#include "Process.h"
#include "TaskManager.h"
#include "elf.h"

Process *Process::create_kernel(const DC::string &name, void (*func)())
{
	return new Process(name, (size_t)func, true);
}

ResultRet<Process *> Process::create_user(const DC::string &executable_loc)
{
	auto fd_or_error = VFS::inst().open(executable_loc, O_RDONLY, 0, VFS::inst().root_ref());
	if (fd_or_error.is_error()) return fd_or_error.code();

	auto fd = fd_or_error.value();
	auto *header = new ELF::elf32_header;

	fd->read((uint8_t *)header, sizeof(ELF::elf32_header));
	if (!ELF::can_execute(header)) {
		delete header;
		return -ENOEXEC;
	}

	TaskManager::enabled() = false;

	Process *proc = new Process(executable_loc, header->program_entry_position, false);

	bool success = proc->load_elf(fd, header);

	// Load the page directory of the current process back
	asm volatile("movl %0, %%cr3" ::"r"(TaskManager::current_process()->page_directory_loc));

	TaskManager::enabled() = true;

	delete header;
	if (!success) {
		delete proc;
		return -ENOEXEC;
	}

	return proc;
}

bool Process::load_elf(const DC::shared_ptr<FileDescriptor> &fd, ELF::elf32_header *header)
{
	// FIXME: Dealloc phys pages if creation fails

	uint32_t pheader_loc = header->program_header_table_position;
	uint32_t pheader_size = header->program_header_table_entry_size;
	uint32_t num_pheaders = header->program_header_table_entries;

	fd->seek(pheader_loc, SEEK_SET);
	auto *program_headers = new ELF::elf32_segment_header[num_pheaders];
	fd->read((uint8_t *)program_headers, pheader_size * num_pheaders);

	for (auto i = 0; i < num_pheaders; i++) {
		auto pheader = &program_headers[i];
		
		if (pheader->p_type == ELF_PT_LOAD) {
			size_t loadloc_pagealigned = (pheader->p_vaddr/PAGE_SIZE) * PAGE_SIZE;

			if (!page_directory->allocate_pages(loadloc_pagealigned, pheader->p_memsz, pheader->p_flags & ELF_PF_W)) {
				delete[] program_headers;
				return false;
			}

			fd->seek(pheader->p_offset, SEEK_SET);
			fd->read((uint8_t *)pheader->p_vaddr, pheader->p_filesz);

			// Zero out rest of section
			if (pheader->p_filesz < pheader->p_memsz) {
				auto *zero_start = (uint8_t *)(pheader->p_vaddr + pheader->p_filesz);
				memset(zero_start, 0, (int)(pheader->p_memsz - pheader->p_filesz));
			}

			if (current_brk < pheader->p_vaddr + pheader->p_memsz)
				current_brk = pheader->p_vaddr + pheader->p_memsz;
		}
	}

	delete[] program_headers;
	return true;
}

pid_t Process::pid()
{
	return _pid;
}

DC::string Process::name()
{
	return _name;
}

Process::Process(const DC::string &name, size_t entry_point, bool kernel) : _name(name), _pid(TaskManager::get_new_pid()), state(PROCESS_ALIVE), kernel(kernel), ring(kernel ? 0 : 3)
{
	if (!kernel) {
		auto ttydesc = DC::make_shared<FileDescriptor>(TTYDevice::current_tty());
		ttydesc->set_options(O_RDONLY | O_WRONLY);

		stdin = ttydesc;
		stdout = ttydesc;
	}

	_kernel_stack_base = Paging::PageDirectory::k_alloc_pages(PROCESS_STACK_SIZE);
	_kernel_stack_size = PROCESS_STACK_SIZE;

	size_t stack_base;
	if (!kernel) {
		page_directory = new Paging::PageDirectory();
		page_directory_loc = page_directory->entries_physaddr();

		// Load the page directory of the new process
		asm volatile("movl %0, %%cr3" ::"r"(page_directory_loc));

		if (!page_directory->allocate_pages(0, PROCESS_STACK_SIZE)) PANIC("NEW_PROC_STACK_ALLOC_FAIL", "Was unable to allocate virtual memory for a new process' stack", true);
		stack_base = 0;
	} else {
		page_directory_loc = Paging::kernel_page_directory.entries_physaddr();
		stack_base = (size_t)_kernel_stack_base;
	}

	registers.eip = entry_point;
	auto *stack = (uint32_t*)(stack_base + PROCESS_STACK_SIZE);

	*--stack = 0; // These first four values are placeholders for the argc, argv, and env that programs pop off the stack
	*--stack = 0;
	*--stack = 0;
	*--stack = 0;

	// If this is a usermode process, push the correct ss and stack pointer for switching rings during iret
	if (!kernel) {
		auto cstack = (size_t)stack;
		*--stack = 0x23;
		*--stack = cstack;
	}

	*--stack = 0x202; // eflags
	*--stack = kernel ? 0x8 : 0x1B; // cs
	*--stack = entry_point; // eip

	// We push iret if the pid isn't 1 (kernel), because in the preempt_asm function, we return to preempt in
	// the C code. However, when the process is first set up, the ret location isn't in the stack, so we push a function
	// that calls iret.
	if (_pid != 1) {
		*--stack = 0; // eax
		*--stack = 0; // ebx
		*--stack = 0; // ecx
		*--stack = 0; // edx
		*--stack = (size_t)TaskManager::proc_first_preempt;
	}

	// Push everything that's popped off in preempt_asm
	*--stack = stack_base; //ebp
	*--stack = 0; // edi
	*--stack = 0; // esi

	if (kernel) {
		*--stack = 0x10; // fs
		*--stack = 0x10; // gs
	} else {
		*--stack = 0x23; // fs
		*--stack = 0x23;
	}

	// Set the ESP to the stack's location
	registers.esp = (size_t) stack;
}

Process::Process(Process *to_fork, Registers &regs): _name(to_fork->_name), _pid(TaskManager::get_new_pid()), state(PROCESS_ALIVE), kernel(false), ring(3), stdin(to_fork->stdin), stdout(to_fork->stdout)
{
	if (to_fork->kernel) PANIC("KRNL_PROCESS_FORK", "Kernel processes cannot be forked.",  true);

	_kernel_stack_base = Paging::PageDirectory::k_alloc_pages(PROCESS_STACK_SIZE);
	_kernel_stack_size = PROCESS_STACK_SIZE;
	page_directory = new Paging::PageDirectory();
	page_directory_loc = page_directory->entries_physaddr();

	//Load the page directory of the new process
	asm volatile("movl %0, %%cr3" :: "r"(page_directory_loc));

	//Fork the old page directory
	page_directory->fork_from(to_fork->page_directory);

	registers.eip = regs.eip;

	auto *stack = (uint32_t*) regs.useresp;

	//Push the correct ss and stack pointer for switching rings during iret
	auto cstack = (size_t) stack;
	*--stack = 0x23;
	*--stack = cstack;

	//Push EFLAGS, CS, and EIP for iret
	*--stack = 0x202; // eflags
	*--stack = kernel ? 0x8 : 0x1B; // cs
	*--stack = regs.eip; // eip

	//We push iret if the pid isn't 1 (the kernel process), because in the preempt_asm function, we return to preempt in
	//the C code. However, when the process is first set up, the ret location isn't in the stack, so we push a function
	//that calls iret.
	*--stack = 0; // fork() in the child returns 0
	*--stack = regs.ebx;
	*--stack = regs.ecx;
	*--stack = regs.edx;
	*--stack = (size_t) TaskManager::proc_first_preempt;

	//Push everything that's popped off in preempt_asm
	*--stack = regs.ebp; //ebp
	*--stack = 0; //edi
	*--stack = 0; //esi
	if (kernel) {
		*--stack = 0x10; // fs
		*--stack = 0x10; // gs
	} else {
		*--stack = 0x23; // fs
		*--stack = 0x23; // gs
	}

	//Set the ESP to the stack's location
	registers.esp = (size_t) stack;

	//Load back the page directory of the current process
	asm volatile("movl %0, %%cr3" :: "r"(TaskManager::current_process()->page_directory_loc));
}

Process::~Process()
{
	delete page_directory;
	if (_kernel_stack_base) Paging::PageDirectory::k_free_pages(_kernel_stack_base, PROCESS_STACK_SIZE);
}

void Process::notify(uint32_t sig)
{
	switch (sig) {
		case SIGTERM:
			kill();
			break;
		case SIGILL:
			printf("\n(PID %d) Illegal operation! Aborting.\n", _pid);
			kill();
			break;
		case SIGSEGV:
			printf("\n(PID %d) Segmentation fault! Aborting.\n", _pid);
			kill();
			break;
	}
}

void Process::kill()
{
	if (_pid != 1) {
		TaskManager::enabled() = false;
		prev->next = next;
		next->prev = prev;
		state = PROCESS_DEAD;
		delete this;
		TaskManager::enabled() = true;
		TaskManager::preempt_now();
	} else {
		PANIC("KERNEL KILLED", "EVERYONE PANIC THIS ISN'T GOOD", true);
	}
}

/************
 * SYSCALLS *
 ************/

ssize_t Process::sys_read(int fd, uint8_t *buf, size_t count)
{
	if ((size_t)buf + count > HIGHER_HALF) return -EFAULT;
	if (fd == 0) { // stdin
		return stdin->read(buf, count);
	} else if (fd == 1) { // stdout
		return stdout->read(buf, count);
	}

	return -EBADF;
}

ssize_t Process::sys_write(int fd, uint8_t *buf, size_t count)
{
	if ((size_t)buf + count > HIGHER_HALF) return -EFAULT;
	if (fd == 0) { // stdin
		return stdin->write(buf, count);
	} else if (fd == 1) { // stdout
		return stdout->write(buf, count);
	}

	return -EBADF;
}

size_t Process::sys_sbrk(int amount)
{
	size_t current_brk_page = current_brk / PAGE_SIZE;

	if (amount > 0) {
		size_t new_brk_page = (current_brk + amount) / PAGE_SIZE;
		if (new_brk_page != current_brk_page) {
			page_directory->allocate_pages((current_brk_page + 1) * PAGE_SIZE, amount - (current_brk % PAGE_SIZE), true);
		}
	} else if (amount < 0) {
		size_t new_brk_page = (current_brk + amount) / PAGE_SIZE;
		if (new_brk_page != current_brk_page) {
			page_directory->unmap_pages(new_brk_page, current_brk_page - new_brk_page);
		}
	}

	size_t prev_brk = current_brk;
	current_brk += amount;

	return prev_brk;
}

pid_t Process::sys_fork(Registers &regs)
{
	TaskManager::enabled() = false;
	auto *new_proc = new Process(this, regs);
	TaskManager::add_process(new_proc);
	TaskManager::enabled() = true;
	return new_proc->pid();
}

void *Process::kernel_stack()
{
	return (void*)((size_t)_kernel_stack_base + _kernel_stack_size);
}