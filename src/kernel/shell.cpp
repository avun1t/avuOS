#include <kernel/kstddef.h>
#include <kernel/keyboard.h>
#include <kernel/kstdio.h>
#include <kernel/shell.h>
#include <kernel/filesystem/Ext2.h>
#include <kernel/memory/kliballoc.h>
#include <kernel/tasking/tasking.h>
#include <kernel/tasking/elf.h>
#include <kernel/pci/pci.h>
#include <kernel/memory/paging.h>
#include <kernel/device/ide.h>
#include <kernel/filesystem/VFS.h>
#include <common/cstring.h>
#include <common/stdlib.h>

extern bool shell_mode;
extern char kbdbuf[256];
extern bool tasking_enabled;

Shell::Shell() : current_dir(VFS::inst().resolve_path("/", DC::shared_ptr<LinkedInode>(nullptr)))
{}

void dummy()
{
	while(1);
}

void Shell::shell()
{
	while (!exitShell) {
		printf("kernel:%s$ ", current_dir->get_full_path().c_str());

		shell_mode = true;
		get_input();

		shell_mode = false;
		set_color(0x07);

		substr(index_of(' ', kbdbuf), kbdbuf, cmdbuf);

		if (index_of(' ', kbdbuf)+1 <= strlen(kbdbuf)) {
			substrr(index_of(' ', kbdbuf)+1, strlen(kbdbuf), kbdbuf, argbuf);
		} else {
			argbuf[0] = 0;
		}

		command_eval(cmdbuf, argbuf);
		set_color(0x0f);
	}

	__kill__();
}

/*
bool find_and_execute(char *cmd, bool wait)
{
	file_t *file = (file_t *)kmalloc(sizeof(file_t));
	if (shellfs->get_file(cmd, file, shellfs) && !file->isDirectory) {
		if (file->sectors*512 > 0x1000) {
			printf("Executable too large.\n");
		} else {
			shellfs->read(file, prog, shellfs);
			
			process_t *proc = create_process(cmd, (uint32_t)progx);
			uint32_t pid = add_process(proc);
			
			while(wait && proc->state == PROCESS_ALIVE);
		}

		kfree(file, sizeof(file_t));
		return true;
	}

	return false;
}
*/

void Shell::command_eval(char *cmd, char *args)
{
	if (strcmp(cmd,"help")) {
		println("ls: List the files in the current directory. Use -h for help.");
		println("cd: Change the current directory.");
		println("pwd: Print the working directory.");
		println("about: Shows some information about the system.");
		println("help: Shows this message.");
		println("cat: Prints a file's contents.");
		println("about: Prints some information.");
		//println("partinfo: Prints information about the current partition.");
		println("pagefault: Triggers a page fault, in case you wanted to.");
		println("tasks: Prints all running tasks.");
		println("bg: Run a program in the background.");
		println("kill: Kill a program.");
		println("dummy: Create a dummy process.");
		println("elfinfo: Print info about an ELF executable.");
		println("lspci: Lists PCI devices.");
		println("exit: Pretty self explanatory.");
	} else if (strcmp(cmd,"ls")) {
		printf("Not implemented");
	} else if (strcmp(cmd,"cd")) {
		auto ref = VFS::inst().resolve_path(args, current_dir);
		if (ref) {
			if (ref->inode()->metadata().is_directory()) {
				current_dir = ref;
			} else {
				printf("Could not cd to '%': Not a directory\n", args);
			}
		} else {
			printf("Could not find '%s'\n", args);
		}
	} else if (strcmp(cmd,"pwd")) {
		printf("%s\n", current_dir->get_full_path().c_str());
	} else if (strcmp(cmd,"about")) {
		println("avuOS 0.1");
	} else if (strcmp(cmd,"cat")) {
		/*file_t file = {};
		strcpy(dirbuf, dirbuf2);
		strcat(dirbuf2,args);
		strcat(dirbuf2,"/");

		if (shellfs->get_file(dirbuf2, &file, shellfs)) {
			uint8_t *buf = (uint8_t *)kmalloc(file.sectors*512);
			shellfs->read(&file, buf, shellfs);
			
			for(int i = 0; i < file.size; i++)
				putch(buf[i]);
			
			kfree(buf, file.sectors*512);
		} else {
			printf("Cannot find %s.\n",args);
		}*/
	} else if (strcmp(cmd,"pagefault")) {
		if (strcmp(args,"-r")) {
			char i = ((char*)0xDEADC0DE)[0];
		} else if(strcmp(args,"-w")) {
			((char*)0xDEADC0DE)[0]='F';
		} else {
			println("Usage: pagefault [-r,-w]");
			println("-r: Triggers a page fault by reading.");
			println("-w: Triggers a page fault by writing.");
		}
	} else if (strcmp(cmd,"exit")) {
		exitShell = true;
	} else if (strcmp(cmd,"tasks")) {
		print_tasks();
	} else if (strcmp(cmd,"bg")) {
		/*if (strcmp(args,"") || !find_and_execute(args, false)) {
			printf("Cannot find \"%s\".\n", args);
		}*/
	} else if (strcmp(cmd,"kill")) {
		uint32_t pid = atoi(args);
		process_t *proc = get_process(pid);

		if (proc != NULL && proc->pid != 1) {
			kill(proc);
			printf("Sent SIGTERM (%d) to %s (PID %d).\n", SIGTERM, proc->name, proc->pid);
		} else if (proc->pid == 1) {
			printf("Cannot kill kernel!\n");
		} else {
			printf("No process with PID %d.\n", pid);
		}
	} else if (strcmp(cmd, "dummy")) {
		add_process(create_process("dummy", (uint32_t)dummy));
	} else if (strcmp(cmd, "elfinfo")) {
		/*file_t file = {};

		strcpy(dirbuf, dirbuf2);
		strcat(dirbuf2,args);
		strcat(dirbuf2,"/");

		if (shellfs->get_file(dirbuf2, &file, shellfs)) {
			uint8_t *headerBuf = (uint8_t *)kmalloc(512);
			elf32_header *header = (elf32_header*)headerBuf;

			shellfs->read(&file, headerBuf, shellfs);

			if (header->magic != ELF_MAGIC) {
				printf("Not an ELF file. %x\n", header->magic);
				return;
			}

			printf("Bits: %s\n", header->bits == ELF32 ? "32" : "64");
			printf("Endianness: %s\n", header->endianness == ELF_LITTLE_ENDIAN ? "little" : "big");
			printf("Instruction set: %s\n", header->instruction_set == ELF_X86 ? "x86" : "other");
			printf("Program Header Entry Size: %d\n", header->program_header_table_entry_size);
			printf("Num Program Header Entries: %d\n", header->program_header_table_entries);
			kfree(headerBuf, 512);
		} else {
			printf("Cannot find %s.\n",args);
		}*/
	} else if (strcmp(cmd, "lspci")) {
		PCI::enumerate_devices([](PCI::Address address, PCI::ID id, void *dataPtr) {
			uint8_t clss = PCI::read_byte(address, PCI_CLASS);
			uint8_t subclss = PCI::read_byte(address, PCI_SUBCLASS);
			printf("%x:%x.%x Vendor: 0x%x Device: 0x%x\n	Class: 0x%x Subclass: 0x%x\n", address.bus, address.slot, address.function, id.vendor, id.device, clss, subclss);
		}, nullptr);
	} else if (strcmp(cmd, "findpata")) {
		IDE::PATAChannel channel = IDE::find_pata_channel(ATA_PRIMARY);
		printf("%x:%x.%x Int %d\n", channel.address.bus, channel.address.slot, channel.address.function, PCI::read_byte(channel.address, PCI_INTERRUPT_LINE));
	} else {
		/*if (!find_and_execute(cmd, true)) {
			printf("\"%s\" is not a recognized command, file, or program.\n", cmd);
		}*/
	}	
}