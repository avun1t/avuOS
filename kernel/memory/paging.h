#ifndef __PAGING_H_
#define __PAGING_H_

#include "PageTable.h"

#define PAGING_4KiB 0
#define PAGING_4MiB 1
#define PAGE_SIZE 4096
#define PAGE_SIZE_FLAG PAGING_4KiB
#define HIGHER_HALF 0xC0000000
#define KERNEL_START ((size_t)&_KERNEL_START)
#define KERNEL_END ((size_t)&_KERNEL_END)
#define KERNEL_SIZE (KERNEL_END - KERNEL_START)
#define KERNEL_SIZE_PAGES ((KERNEL_SIZE + (PAGE_SIZE - 1)) / PAGE_SIZE)
#define KERNEL_END_VIRTADDR (HIGHER_HALF + KERNEL_SIZE_PAGES * PAGE_SIZE)
#define PAGETABLES_VIRTADDR 0xFFC00000
#define KERNEL_HEAP_VIRTADDR (HIGHER_HALF + PAGE_SIZE*1024)

/**
 * The basic premise of how the memory allocation in avuOS is as follows:
 *
 * The kernel is always mapped to the topmost 1GiB of the address space. All the relevant page directory entries for
 * the kernel are stored in static variables in the PageDirectory class, and are copied over to each new page directory.
 *
 * There is a bitmap that dictates which 4KiB pages in physical memory are used (in paging.cpp), and each page directory
 * has a bitmap dictating which virtual 4KiB pages are used. Additionally, there is a static virtual memory bitmap for
 * the kernel.
 *
 * When a process needs to allocate a new page, the OS first looks to see if the page table which would handle that
 * virtual address even exists yet. If it doesn't, a 4KiB page to store it is allocated and mapped to
 * PAGETABLES_VIRTADDR in the kernel space (but since this mapping is per-process, it is not stored with the
 * static kernel entries). If all of the entries in a page table go unused, the page table is deallocated to free up
 * physical memory.
 *
 * This system means there is very little overhead when multitasking; when the context is switched, we simply load the
 * page directory for the current process and we go on our way. If for any reason the kernel's page directory entries
 * are modified, every process's page directory is updated to reflect the change (although this shouldn't happen very
 * often)
 */

namespace Paging {
	class PageTable;
	class PageDirectory;
	extern PageDirectory kernel_page_directory;

	extern "C" long _KERNEL_START;
	extern "C" long _KERNEL_END;
	extern "C" void load_page_dir(size_t* dir);

	/**
	 * Sets up paging.
	 */
	void setup_paging();

	/**
	 * Called when the CPU encounters a page fault in the kernel.
	 * @param r the Registers struct from the isr.
	 */
	void page_fault_handler(struct Registers *r);

	/**
	 * Get the physical memory bitmap.
	 * @return The physical memory bitmap.
	 */
	MemoryBitmap<0x100000>& pmem_bitmap();

	/**
	 * Used when setting up paging initially in order to map an entire page table starting at a virtual address.
	 * @param page_table The page table to set up
	 * @param virtual_address The virtual address to map the pagetable to.
	 * @param read_write The read_write flag on the page tables.
	 */
	void early_pagetable_setup(PageTable* page_table, size_t virtual_address, bool read_write);

	/**
	 * Get the amount of used physical memory in KiB.
	 * @return The amount of allocated physical memory in KiB (granularity of PAGE_SIZE)
	 */
	size_t get_used_mem();
};

int liballoc_lock();
int liballoc_unlock();
void* liballoc_alloc(int);
int liballoc_free(void*,int);

int liballoc_lock();
int liballoc_unlock();
void *liballoc_alloc(int);
int liballoc_free(void *, int);

#endif // __PAGING_H_