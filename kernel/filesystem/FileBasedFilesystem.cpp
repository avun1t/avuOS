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

#include <common/defines.h>
#include "FileBasedFilesystem.h"

FileBasedFilesystem::FileBasedFilesystem(DC::shared_ptr<FileDescriptor> file) : Filesystem(file)
{}

bool FileBasedFilesystem::read_logical_blocks(size_t block, size_t count, uint8_t *buffer)
{
	if (_file->seek(block * logical_block_size(), SEEK_SET) < 0) return false;
	return _file->read(buffer, count * logical_block_size()) >= 0;
}

size_t FileBasedFilesystem::logical_block_size()
{
	return _logical_block_size;
}

bool FileBasedFilesystem::read_blocks(size_t block, size_t count, uint8_t *buffer)
{
	if (_file->seek(block * block_size(), SEEK_SET) < 0) return false;
	return _file->read(buffer, count * block_size()) >= 0;
}