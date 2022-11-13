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

#ifndef __AVUOS_DIRECTORYENTRY_H_
#define __AVUOS_DIRECTORYENTRY_H_

#include "InodeMetadata.h"

#define NAME_MAXLEN 256
#define TYPE_UNKNOWN 0
#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_CHARACTER_DEVICE 3
#define TYPE_BLOCK_DEVICE 4
#define TYPE_FIFO 5
#define TYPE_SOCKET 6
#define TYPE_SYMLINK 7

class DirectoryEntry {
public:
	InodeID id;
	size_t name_length;
	uint8_t type;
	char name[NAME_MAXLEN];
};

#endif // __AVUOS_DIRECTORYENTRY_H_