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

#ifndef __INODE_H_
#define __INODE_H_

#include <kernel/kstddef.h>
#include <common/shared_ptr.hpp>
#include <common/string.h>
#include "InodeMetadata.h"
#include "DirectoryEntry.h"

class Filesystem;
typedef uint32_t InodeID;

class Inode {
public:
	InodeID id;
	Filesystem &fs;

	Inode(Filesystem &fs, InodeID id);

	virtual DC::shared_ptr<Inode> find(DC::string name);
	virtual Inode *find_rawptr(DC::string name);
	virtual ssize_t read(size_t start, size_t length, uint8_t *buffer) = 0;
	virtual ssize_t read_dir_entry(size_t start, DirectoryEntry *buffer) = 0;

	InodeMetadata metadata();

protected:
	InodeMetadata _metadata;
};

#endif // __INODE_H_