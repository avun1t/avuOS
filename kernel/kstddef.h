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

#ifndef __KSTDDEF_H_
#define __KSTDDEF_H_

#include <common/cstddef.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

struct  __attribute__((packed)) Registers {
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int num, err_code;
	unsigned int eip, cs, eflags, useresp, ss;
};

void io_wait();
void outb(uint16_t port, uint8_t value);
void outw(uint16_t port, uint16_t value);
void outl(uint16_t port, uint32_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);
int index_of(char c, char *str);
int index_of_n(char c, int n, char *str);
int count_of(char c, char *str);
bool contains(char *str, char *cont);
void cli();
void sti();
void to_upper(char *str);
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);

#endif // __KSTDDEF_H_