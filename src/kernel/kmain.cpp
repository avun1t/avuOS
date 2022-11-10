#include <kernel/kstddef.h>
#include <kernel/tasking/tss.h>
#include <kernel/memory/gdt.h>
#include <kernel/multiboot.h>
#include <kernel/kstdio.h>
#include <kernel/memory/kliballoc.h>
#include <kernel/memory/paging.h>
#include <kernel/interrupt/idt.h>
#include <kernel/interrupt/isr.h>
#include <kernel/interrupt/irq.h>
#include <kernel/interrupt/syscall.h>
#include <kernel/device/ata.h>
#include <kernel/filesystem/Ext2.h>
#include <kernel/keyboard.h>
#include <kernel/shell.h>
#include <kernel/pit.h>
#include <kernel/tasking/tasking.h>
#include <kernel/device/PIODevice.h>
#include <kernel/device/PartitionDevice.h>
#include <kernel/kmain.h>

int i;

int kmain(uint32_t mbootptr)
{
	load_gdt();
	interrupts_init();
	setup_paging();
	
	printf("init: First stage complete.\ninit: Initializing tasking...\n");

	init_tasking();

	return 0;
}

// called from kthread
void kmain_late()
{
	printf("init: Tasking initialized.\ninit: Initializing disk...\n");
	PIODevice disk = PIODevice(boot_disk);
	uint8_t sect[512];
	disk.read_block(0, sect);
	if (sect[1] == 0xFF) {
		println_color("init: WARNING: I think you may be booting avuOS off of a USB drive or other unsupported device. Disk reading functions may not work.",0x0C);
	}

	PartitionDevice part(&disk, pio_get_first_partition(boot_disk));
	if (Ext2Filesystem::probe(&part)) {
		printf("init: Partition is ext2");
	} else {
		println("init: Partition is not ext2! Hanging.");
		while (true);
	}

	Ext2Filesystem ext2fs(&part);
	if (ext2fs.superblock.version_major < 1) {
		printf("init: Unsupported ext2 version %d.%d. Must be at least 1. Hanging.", ext2fs.superblock.version_major, ext2fs.superblock.version_minor);
		while (true);
	}

	printf("%d.%d\n", ext2fs.superblock.version_major, ext2fs.superblock.version_minor);
	if (ext2fs.superblock.inode_size != 128) {
		printf("init: Unsupported inode size %d. avuOS only supports an inode size of 128 at this time. Hanging.", ext2fs.superblock.inode_size);
	}

	init_shell(&ext2fs);
	add_process(create_process("shell", (uint32_t)shell));
	while (get_process(2));
	printf("\n\nShell exited.\n\n");
	PANIC("Kernel process stopped!", "That should not happen.", true);
	__kill__();
}

void parse_mboot(uint32_t addr)
{
	struct multiboot_tag *tag;

	for (tag = (struct multiboot_tag *)(addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *)((multiboot_uint8_t *)tag + ((tag->size + 7) & ~7))) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				boot_disk = (uint8_t)(((struct multiboot_tag_bootdev *)tag)->biosdev & 0xFF);
				break;
		}
	}
}

void interrupts_init()
{
	register_idt();
	isr_init();
	idt_set_gate(0x80, (unsigned)syscall_handler, 0x08, 0x8E);
	idt_set_gate(0x81, (unsigned)preempt, 0x08, 0x8E); // for preempting without PIT
	irq_add_handler(1, keyboard_handler);
	//irq_add_handler(0, pit_handler);
	pit_init(200);
	irq_init();
	asm volatile("sti");
}