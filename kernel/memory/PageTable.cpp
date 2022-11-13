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

#include "PageTable.h"

namespace Paging {
	void PageTable::Entry::Data::set_address(size_t address)
	{
		page_addr = address >> 12u;
	}

	size_t PageTable::Entry::Data::get_address()
	{
		return page_addr << 12u;
	}

	PageTable::PageTable() = default;

	PageTable::Entry *PageTable::entries()
	{
		return _entries;
	}

	PageTable::Entry &PageTable::operator[](int index)
	{
		return _entries[index];
	}
}