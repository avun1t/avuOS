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

#ifndef __AVUOS_FILE_H_
#define __AVUOS_FILE_H_

#include <common/shared_ptr.hpp>
#include <kernel/Result.hpp>
#include "FileDescriptor.h"
#include "DirectoryEntry.h"

class FileDescriptor;
class File {
public:
	virtual ~File();
	virtual bool is_inode();
	virtual ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count);
	virtual ssize_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count);
	virtual ssize_t read_dir_entry(FileDescriptor &fd, size_t offset, DirectoryEntry *buffer);

protected:
	File();
};

#endif // __AVUOS_FILE_H_