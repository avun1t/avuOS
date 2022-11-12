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
#include <kernel/filesystem/VFS.h>
#include <common/vector.hpp>
#include <kernel/device/TTYDevice.h>

int i;

int kmain(uint32_t mbootptr)
{
	clear_screen();
	printf("init: Starting avuOS...\n");
	parse_mboot(mbootptr + HIGHER_HALF);

	load_gdt();
	interrupts_init();
	setup_paging();
	Device::init();
	
	printf("init: First stage complete.\ninit: Initializing tasking...\n");

	init_tasking();

	return 0;
}

void shell_process()
{
	Shell shell;
	shell.shell();
}

// called from kthread
void kmain_late()
{
	printf("init: Tasking initialized.\ninit: Initializing disk...\n");
	auto disk = DC::make_shared<PIODevice>(3, 0, boot_disk);
	uint8_t sect[512];
	disk->read_block(0, sect);
	if (sect[1] == 0xFF) {
		println_color("init: WARNING: I think you may be booting avuOS off of a USB drive or other unsupported device. Disk reading functions may not work.",0x0C);
	}

	auto part = DC::make_shared<PartitionDevice>(3, 1, disk, pio_get_first_partition(boot_disk));
	auto part_descriptor = DC::make_shared<FileDescriptor>(part);
	if (Ext2Filesystem::probe(*part_descriptor.get())) {
		printf("init: Partition is ext2 ");
	} else {
		println("init: Partition is not ext2! Hanging.");
		while (true);
	}

	auto *ext2fs = new Ext2Filesystem(part_descriptor);
	ext2fs->init();
	if (ext2fs->superblock.version_major < 1) {
		printf("init: Unsupported ext2 version %d.%d. Must be at least 1. Hanging.", ext2fs->superblock.version_major, ext2fs->superblock.version_minor);
		while (true);
	}

	printf("%d.%d\n", ext2fs->superblock.version_major, ext2fs->superblock.version_minor);
	if (ext2fs->superblock.inode_size != 128) {
		printf("init: Unsupported inode size %d. avuOS only supports an inode size of 128 at this time. Hanging.", ext2fs->superblock.inode_size);
	}

	VFS *vfs = new VFS();
	if (!vfs->mount_root(ext2fs)) {
		printf("init: Failed to mount root. Hanging.");
		while (true);
	}

	add_process(create_process("shell", (uint32_t)shell_process));
	while (get_process(2));
	printf("\n\nShell exited.\n\n");
	while (1);
	PANIC("Kernel process stopped!", "That should not happen.", true);
	__kill__();
}

void parse_mboot(uint32_t addr)
{
	struct multiboot_info *header = (multiboot_info*)addr;
	
	if (header->flags & MULTIBOOT_INFO_BOOTDEV) {
		boot_disk = (header->boot_device & 0xF0000000u) >> 28u;
		printf("init: BIOS boot disk: 0x%x\n", boot_disk);
	} else {
		printf("init: No multiboot boot device info. Cannot boot.\n");
		while(1);
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