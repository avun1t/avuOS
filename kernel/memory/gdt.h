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

#ifndef __GDT_H_
#define __GDT_H_

#define GDT_ENTRIES 6

typedef struct GDTEntry {
	uint16_t limit;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) GDTEntry;

typedef struct GDTPointer {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) GDTPointer;

void gdt_set_gate(uint32_t num, uint16_t limit, uint32_t base, uint8_t access, uint8_t gran);
void load_gdt();
extern "C" void gdt_flush();

#endif // __GDT_H_