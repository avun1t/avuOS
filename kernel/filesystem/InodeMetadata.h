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

#ifndef __AVUOS_INODEMETADATA_H_
#define __AVUOS_INODEMETADATA_H_

#include <common/cstddef.h>

#define MODE_FIFO 0x1000
#define MODE_CHAR_DEVICE 0x2000
#define MODE_DIRECTORY 0x4000
#define MODE_BLOCK_DEVICE 0x6000
#define MODE_FILE 0x8000
#define MODE_SYMLINK 0xA000
#define MODE_SOCKET 0xC000

typedef uint32_t InodeID;
class InodeMetadata {
public:
	size_t mode = 0;
	size_t size = 0;
	InodeID inode_id;

	unsigned dev_major;
	unsigned dev_minor;

	bool is_directory() const;
	bool is_block_device() const;
	bool is_character_device() const;
	bool is_device() const;
	bool is_simple_file() const;
	bool exists() const;
};

#endif // __AVUOS_INODEMETADATA_H_