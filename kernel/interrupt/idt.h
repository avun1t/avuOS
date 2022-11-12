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