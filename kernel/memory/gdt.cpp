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
#include <kernel/memory/gdt.h>
#include <kernel/tasking/TSS.h>
#include <kernel/tasking/TaskManager.h>

GDTEntry gdt[GDT_ENTRIES];
GDTPointer gp;

extern "C" void *stack;

void gdt_set_gate(uint32_t num, uint16_t limit, uint32_t base, uint8_t access, uint8_t gran)
{
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	gdt[num].limit = limit;
	gdt[num].granularity = gran;

	gdt[num].access = access;
}

void load_gdt()
{
	gp.limit = (sizeof(GDTEntry) * GDT_ENTRIES) - 1;
	gp.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0); // null
	gdt_set_gate(1, 0xFFFF, 0, 0b10011010, 0b11001111); // kernel code
	gdt_set_gate(2, 0xFFFF, 0, 0b10010010, 0b11001111); // kernel data
	gdt_set_gate(3, 0xFFFF, 0, 0b11111010, 0b11001111); // user code
	gdt_set_gate(4, 0xFFFF, 0, 0b11110010, 0b11001111); // user data
	gdt_set_gate(5, sizeof(TSS), (size_t)&TaskManager::tss - HIGHER_HALF, 0b10010010, 0x40); // TSS

	TaskManager::tss.ss0 = 0x10;
	TaskManager::tss.esp0 = (size_t)stack;

	gdt_flush();
	//asm volatile("ltr %0" ::"r"((uint16_t)0x28));
}