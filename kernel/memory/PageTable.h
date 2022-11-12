#ifndef __AVUOS_PAGETABLE_H_
#define __AVUOS_PAGETABLE_H_

#include <common/cstddef.h>
#include "MemoryBitmap.hpp"
#include "paging.h"
namespace Paging {
	class PageTable {
	public:
		typedef union Entry {
			struct __attribute((packed)) Data {
			public:
				bool present : 1;
				bool read_write : 1;
				bool user : 1;
				bool write_through : 1;
				bool cache_disabled : 1;
				bool acessed: 1;
				bool dirty : 1;
				bool zero : 1;
				bool global : 1;
				uint8_t unused : 3;
				uint32_t page_addr : 20;

				void set_address(size_t address);
				size_t get_address();
			} data;
			uint32_t value;
		} Entry;

		PageTable();
		Entry* entries();
		Entry& operator[](int index);
	private:
		Entry _entries[1024] __attribute__((aligned(4096))) = {{.value=0}};
	};
}

#endif // __AVUOS_PAGETABLE_H_