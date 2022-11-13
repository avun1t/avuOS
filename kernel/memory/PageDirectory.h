#ifndef __AVUOS_PAGEDIRECTORY_H_
#define __AVUOS_PAGEDIRECTORY_H_

#include <common/cstddef.h>
#include <common/vector.hpp>
#include "paging.h"

namespace Paging {
	class PageDirectory {
	public:
		typedef union Entry {
			class __attribute((packed)) Data {
			public:
				bool present : 1;
				bool read_write : 1;
				bool user : 1;
				bool write_through : 1;
				bool cache_disable : 1;
				bool accessed : 1;
				bool zero : 1;
				uint8_t size : 1;
				bool ignored : 1;
				uint8_t unused : 3;
				size_t page_table_addr : 20;

				void set_address(size_t address);
				size_t get_address();
			} data;
			uint32_t value;
		} Entry;

		/**************************************
		 * Static kernel page directory stuff *
		 **************************************/

		static Entry kernel_entries[256];
		static MemoryBitmap<0x40000> kernel_vmem_bitmap;
		static PageTable kernel_page_tables[256] __attribute__((aligned(4096)));
		static size_t kernel_page_tables_physaddr[1024];

		/**
		 * Initialize the kernel page directory entries & related variables.
		 */
		static void init_kmem();

		/**
		 * Maps one page at virtaddr (must be in kernel space) to physaddr
		 * @param physaddr The physical address to map from.
		 * @param virtaddr The virtual address to map to. Must be in kernel space (>=3GiB)
		 * @param read_write Whether or not the page should be read/write.
		 */
		static void k_map_page(size_t physaddr, size_t virtaddr, bool read_write);

		/**
		 * A wrapper around k_map_page to wrap multiple pages.
		 */
		static void k_map_pages(size_t physaddr, size_t virtaddr, bool read_write, size_t num_pages);

		/**
		 * Unmaps one page at virtaddr in kernel space.
		 * @param virtaddr The virtual address to unmap. Must be in kernel space (>=3GiB)
		 */
		static void k_unmap_page(size_t virtaddr);

		/**
		 * A wrapper around k_unmap_page that unmaps multiple pages.
		 */
		static void k_unmap_pages(size_t virtaddr, size_t num_pages);

		/**
		 * Allocates a number of contigous pages in kernel vmem and returns a pointer to the first one.
		 * @param pages The number of pages to allocate.
		 * @return A pointer to the first page allocated.
		 */
		 static void *k_alloc_pages(size_t pages);

		/**
		 * Frees num_pages pages allocated with k_alloc_pages.
		 * @param ptr The pointer returned by k_alloc_pages to the start of the pages to be freed.
		 * @param num_pages The number of pages to be freed.
		 */
		 static void k_free_pages(void *ptr, size_t num_pages);

		/************************************
		 * Per-process page directory stuff *
		 ************************************/

		PageDirectory();
		~PageDirectory();
		Entry* entries();

		/**
		 * Get the physical address of the table of entries.
		 * @return The physical address of the table of entries.
		 */
		size_t entries_physaddr();

		/**
		 * @return The entry at index.
		 */
		Entry& operator[](int index);

		/**
		 * Maps one page from physaddr to virtaddr in program space.
		 * @param physaddr The physical address to map from.
		 * @param virtaddr The virtual address to map to. Must be in program space (<3GiB)
		 * @param read_write Whether or not the page should be read/write.
		 */
		void map_page(size_t physaddr, size_t virtaddr, bool read_write);

		/**
		 * A wrapper around map_page to map multiple pages.
		 */
		void map_pages(size_t physaddr, size_t virtaddr, bool read_write, size_t num_pages);

		/**
		 * Unmaps one page at virtaddr in program space.
		 * @param virtaddr The virtual address of the page to unmap. Must be in program space (<3GiB)
		 */
		void unmap_page(size_t virtaddr);

		/**
		 * A wrapper around unmap_page to unmap multiple pages.
		 */
		void unmap_pages(size_t virtaddr, size_t num_pages);

		/**
		 * Allocates the needed amount of pages to fit memsize amount of data starting at vaddr.
		 * @param vaddr The virtual address to start mapping at.
		 * @param memsize The amount of memory needed (NOT pages)
		 * @return Whether or not the allocation was successful.
		 */
		bool allocate_pages(size_t vaddr, size_t memsize);

		/**
		 * Gets the physical address for virtaddr.
		 * @param virtaddr The virtual address.
		 * @return The physical address for virtaddr.
		 */
		size_t get_physaddr(size_t virtaddr);

		/**
		 * Allocates space for a new page table at tables_index in the page directory.
		 * @param tables_index The index in the page directory that this newly allocated page table is for.
		 * @return A pointer to the newly allocated page table.
		 */
		PageTable* alloc_page_table(size_t tables_index);

		/**
		 * Deallocates the space used for a page table at tables_index in the page directory.
		 * @param tables_index The index in the page directory of the page table being dealloc'd.
		 */
		void dealloc_page_table(size_t tables_index);

		/**
		 * Updates the entries for kernel space from the kernel page directory entries.
		 */
		void update_kernel_entries();
		
		/**
		 * Sets the entry pointer to entries. Should only be used for kernel page table.
		 */
		void set_entries(Entry *entries);

	private:
		// The page directory entries for this page directory.
		Entry *_entries = nullptr;
		// The bitmap of used vmem for this page directory.
		MemoryBitmap<0xC0000> _vmem_bitmap;
		// The bitmap of used pmem for this page directory.
		MemoryBitmap<0x100000> _personal_pmem_bitmap;
		// An array of pointers to the page tables that the directory points to.
		PageTable* _page_tables[768] = {nullptr};
		// An array of u16s that stores the number of pages mapped in each page table, used to deallocate a page table once no longer needed
		uint16_t _page_tables_num_mapped[1024] = {0};
		// The page table that points to the memory allocated to store the page tables for this page directory
		PageTable *_page_tables_table;
		// The physical addresses for each page table, used to translate vaddr -> paddr
		size_t _page_tables_physaddr[1024] = {0};
	};
}

#endif // __AVUOS_PAGEDIRECTORY_H_