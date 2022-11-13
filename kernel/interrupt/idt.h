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

#ifndef __IDT_H_
#define __IDT_H_

struct IDTEntry {
	unsigned short offset_low;  // offset bits 0-15
	unsigned short selector;    // a code segment selector in the GDT
	unsigned char zero;         // always 0
	unsigned char attrs;        // type & attributes
	unsigned short offset_high; // offset bits 16-31
} __attribute__((packed));

struct IDTPointer {
	unsigned short size;
	unsigned int offset;
} __attribute__((packed));

extern "C" void idt_load();

void idt_set_gate(uint8_t num, uint32_t loc, uint16_t selector, uint8_t flags);
void register_idt();

#endif // __IDT_H_