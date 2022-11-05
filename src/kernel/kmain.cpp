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
#include <ata.h>
#include <filesystem/fat32.h>
#include <filesystem/ext2.h>
#include <keyboard.h>
#include <shell.h>
#include <pit.h>
#include <tasking/tasking.h>
#include <kmain.h>

int i;
filesystem_t fs = {};

int kmain(uint32_t mbootptr)
{
	load_gdt();
	interrupts_init();
	setup_paging();
	init_heap();
	parse_mboot(mbootptr + HIGHER_HALF);
	clear_screen();
	center_print("Now in 32-bit protected mode!", 0x07);
	//draw_mono_bitmap(logo, 320, 50, 0, 0, 0x2f);
	//ypos = 7;

	uint8_t sect[512];
	read_sector(boot_disk, 0, sect);

	if (sect[1] == 0xFF) {
		println_color("WARNING: I think you may be booting avuOS off of a USB drive or other unsupported device. Disk reading may not work", 0x0C);
	}

	uint32_t fp = get_first_partition(boot_disk);
	if (is_partition_ext2(boot_disk, fp)) {
		printf("Partition is ext2 ");
	} else {
		println("Partition is not ext2!");
		while(true);
	}

	ext2_superblock sb = {};
	get_ext2_superblock(boot_disk, fp, &sb);
	if (sb.version_major < 1) {
		printf("Unsupported ext2 version %d.%d. Must be at least 1.", sb.version_major, sb.version_minor);
		while(true);
	}

	printf("%d.%d\n", sb.version_major, sb.version_minor);
	
	ext2_partition ext2 = {};
	device_t dev = {boot_disk};
	init_ext2_partition(fp, &dev, &sb, &ext2, &fs);
	
	if (ext2_get_superblock(&ext2)->inode_size != 128) {
		printf("Unsupported inode size %d. avuOS only supports an inode size of 128 at this time.", ext2_get_superblock(&ext2)->inode_size);
	}

	init_tasking();
	//shell(&fs);
}

// called from kthread
void kmain_late()
{
	init_shell(&fs);
	add_process(create_process("shell", (uint32_t)shell));
	while (get_process(2));
	printf("\n\nShell exited.\n\n");
	while(1);
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