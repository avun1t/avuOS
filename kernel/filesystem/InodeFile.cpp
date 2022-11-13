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

#include <kernel/kstdio.h>
#include "InodeFile.h"

InodeFile::InodeFile(DC::shared_ptr<Inode> inode) : _inode(inode)
{}

bool InodeFile::is_inode()
{
	return true;
}

DC::shared_ptr<Inode> InodeFile::inode()
{
	return _inode;
}

ssize_t InodeFile::read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count)
{
	return _inode->read(offset, count, buffer);
}

ssize_t InodeFile::read_dir_entry(FileDescriptor &fd, size_t offset, DirectoryEntry *buffer)
{
	return _inode->read_dir_entry(offset, buffer);
}