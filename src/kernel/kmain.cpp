#include <common.h>
#include <tasking/tss.h>
#include <memory/gdt.h>
#include <multiboot.h>
#include <stdio.h>
#include <memory/heap.h>
#include <memory/paging.h>
#include <interrupt/idt.h>
#include <interrupt/isr.h>
#include <interrupt/irq.h>
#include <interrupt/syscall.h>
#include <device/ata.h>
#include <filesystem/Ext2.h>
#include <keyboard.h>
#include <shell.h>
#include <pit.h>
#include <tasking/tasking.h>
#include <device/PIODevice.h>
#include <device/PartitionDevice.h>
#include <kmain.h>

int i;

int kmain(uint32_t mbootptr)
{
	load_gdt();
	interrupts_init();
	setup_paging();
	init_heap();
	parse_mboot(mbootptr + HIGHER_HALF);
	clear_screen();
	center_print("Now in 32-bit protected mode!", 0x07);

	init_tasking();

	return 0;
}

// called from kthread
void kmain_late()
{
	PIODevice disk = PIODevice(boot_disk);
	uint8_t sect[512];
	disk.read_block(0, sect);
	if (sect[1] == 0xFF) {
		println_color("WARNING: I think you may be booting avuOS off of a USB drive or other unsupported device. Disk reading functions may not work.",0x0C);
	}

	PartitionDevice part(&disk, pio_get_first_partition(boot_disk));
	if (Ext2Filesystem::probe(&part)) {
		printf("Partition is ext2 ");
	} else {
		println("Partition is not ext2!");
		while (true);
	}

	Ext2Filesystem ext2fs(&part);
	if (ext2fs.superblock.version_major < 1) {
		printf("Unsupported ext2 version %d.%d. Must be at least 1.", ext2fs.superblock.version_major, ext2fs.superblock.version_minor);
		while (true);
	}

	printf("%d.%d\n", ext2fs.superblock.version_major, ext2fs.superblock.version_minor);
	if (ext2fs.superblock.inode_size != 128) {
		printf("Unsupported inode size %d. avuOS only supports an inode size of 128 at this time.", ext2fs.superblock.inode_size);
	}

	//auto* in = (Ext2Inode*)(ext2fs.get_inode(2));
	//Ext2Inode* boot = (Ext2Inode*)(in->find("boot"));
	//Ext2Inode* in2 = new(kmalloc(sizeof(Ext2Inode))) Ext2Inode;
	//printf("%d\n", in2->is_link());

	printf("The OS will not work for now. Paging and alloc are being rewritten.");
	while (1);

	init_shell(&ext2fs);
	add_process(create_process("shell", (uint32_t)shell));
	while (get_process(2));
	printf("\n\nShell exited.\n\n");
	while (1);
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