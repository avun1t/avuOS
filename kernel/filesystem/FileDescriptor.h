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

#ifndef __AVUOS_FILEDESCRIPTOR_H_
#define __AVUOS_FILEDESCRIPTOR_H_

#include <common/shared_ptr.hpp>
#include "File.h"
#include "Inode.h"
#include "InodeMetadata.h"
#include <kernel/tasking/Lock.h>

class File;
class DirectoryEntry;
class Device;
class FileDescriptor {
public:
	FileDescriptor(DC::shared_ptr<File> file);
	FileDescriptor(Device *device);

	void set_options(int options);
	bool readable();
	bool writable();
	InodeMetadata metadata();

	int seek(int offset, int whence);
	ssize_t read(uint8_t *buffer, size_t count);
	ssize_t read_dir_entry(DirectoryEntry *buffer);
	ssize_t write(const uint8_t *buffer, size_t counut);
	size_t offset();

private:
	DC::shared_ptr<File> _fileptr;
	File *_file;
	DC::shared_ptr<Inode> _inode;

	bool _readable {false};
	bool _writable {false};
	bool _can_seek {true};

	int _seek {0};

	Lock lock;
};

#endif // __AVUOS_FILEDESCRIPTOR_H_