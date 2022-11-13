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

struct GDTEntryAccessByte {
	uint8_t value;
	struct bits {
		bool accessed : 1;
		bool read_write : 1;
		bool direction : 1;
		bool executable : 1;
		bool type : 1;
		uint8_t ring : 2;
		bool present : 1;
	} bits;
};

union GDTFlagsByte {
	uint8_t value;
	struct bits {
		uint8_t limit_high : 4;
		uint8_t zero : 2;
		bool size : 1;
		bool granularity : 1;
	} bits;
};

typedef struct GDTEntry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	GDTEntryAccessByte access;
	GDTFlagsByte flags_and_limit;
	uint8_t base_high;
} __attribute__((packed)) GDTEntry;

typedef struct GDTPointer {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) GDTPointer;

void gdt_set_gate(uint32_t num, uint32_t limit, uint32_t base, bool read_write, bool executable, bool type, uint8_t ring, bool present = true, bool accessed = false);
void setup_tss();
void load_gdt();
extern "C" void gdt_flush();

#endif // __GDT_H_