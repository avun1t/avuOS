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

#ifndef __AVUOS_PROCESS_H_
#define __AVUOS_PROCESS_H_

#include <kernel/kstddef.h>
#include <common/string.h>
#include <kernel/memory/PageDirectory.h>
#include <kernel/tasking/TaskManager.h>
#include <kernel/tasking/elf.h>
#include <common/shared_ptr.hpp>

#define PROCESS_STACK_SIZE 4096

class Process {
public:
	~Process();

	static Process *create_kernel(const DC::string &name, void (*func)());
	static ResultRet<Process *> create_user(const DC::string &executable_loc);

	pid_t pid();
	DC::string name();

	void notify(uint32_t sig);
	void kill();
	void init();

	uint32_t state = 0;
	Paging::PageDirectory *page_directory;
	bool inited = false;
	Process *next = nullptr, *prev = nullptr;
	size_t page_directory_loc;
	Registers registers = {};
	bool kernel = false;

	DC::shared_ptr<FileDescriptor> stdin;
	DC::shared_ptr<FileDescriptor> stdout;

	// syscalls
	ssize_t sys_read(int fd, uint8_t *buf, size_t count);
	ssize_t sys_write(int fd, uint8_t *buf, size_t count);

private:
	Process(const DC::string &name, size_t entry_point, bool kernel = false);

	bool load_elf(const DC::shared_ptr<FileDescriptor> &fd, ELF::elf32_header *header);

	DC::string _name = "";
	pid_t _pid = 0;
};

#endif // __AVUOS_PROCESS_H_