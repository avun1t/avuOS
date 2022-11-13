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

#include <kernel/kstddef.h>
#include <kernel/kstdio.h>
#include <kernel/memory/kliballoc.h>
#include <common/cstring.h>

void io_wait()
{
	asm volatile("jmp 1f\n\t"
					"1:jmp 2f\n\t"
					"2:");
}

void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %1, %0" :: "d"(port), "a"(value));
}

void outw(uint16_t port, uint16_t value)
{
	asm volatile("outw %1, %0" :: "dN" (port), "a" (value));
}

void outl(uint16_t port, uint32_t value)
{
	asm volatile("outl %1, %0" :: "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port)
{
   uint8_t ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint16_t inw(uint16_t port)
{
   uint16_t ret;
   asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

uint32_t inl(uint16_t port)
{
   uint32_t ret;
   asm volatile("inl %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

int index_of(char c, char *str)
{
	int i = 0;

	while (str[i] != '\0') {
		if (str[i] == c)
			return i;
		i++;
	}

	return strlen(str);
}

// like index_of, except ignores n instances of the character
int index_of_n(char c, int n, char *str)
{
	int i = 0;
	int count = 0;

	while (str[i] != '\0') {
		if (str[i] == c)
			if (count == n)
				return i;
			else
				count++;
		i++;
	}

	return strlen(str);
}

// returns number of instances of c in str
int count_of(char c, char *str)
{
	int count = 0;

	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == c)
			count++;
	}

	return count;
}

// returns true if str has cont in it.
bool contains(char *str, char *cont)
{
	int i = 0;
	int contlen = strlen(cont);
	bool flaga = false;
	bool flagb = false;

	while (str[i+contlen-1] != '\0') {
		flagb = true;
		for (int j = 0; j < strlen(cont); j++) {
			if (cont[j] != str[j+i])
				flagb = false;
		}

		if (flagb) {
			flaga = true;
		}

		i++;
	}

	return flaga;
}

void cli()
{
	asm volatile("cli");
}

void sti()
{
	asm volatile("sti");
}

void to_upper(char *str)
{
	while (*str != '\0') {
		if (*str >= 'a' && *str <= 'z') *str = *str - ('a' - 'A');
		*str++;
	}
}

void *operator new(size_t size)
{
	return kmalloc(size);
}

void *operator new[](size_t size)
{
	return kmalloc(size);
}

void operator delete(void *p)
{
	kfree(p);
}

void operator delete[](void *p)
{
	kfree(p);
}

extern "C" void __cxa_pure_virtual()
{
	// do nothing or print an error message
}