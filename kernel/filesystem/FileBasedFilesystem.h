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

#ifndef __AVUOS_FILEBASEDFILESYSTEM_H_
#define __AVUOS_FILEBASEDFILESYSTEM_H_

#include "FileSystem.h"

class FileBasedFilesystem : public Filesystem {
public:
	FileBasedFilesystem(DC::shared_ptr<FileDescriptor> file);
	size_t logical_block_size();
	bool read_logical_blocks(size_t block, size_t count, uint8_t *buffer);
	bool read_blocks(size_t block, size_t count, uint8_t *buffer);

protected:
	size_t _logical_block_size {512};
};

#endif // __AVUOS_FILEBASEDFILESYSTEM_H_