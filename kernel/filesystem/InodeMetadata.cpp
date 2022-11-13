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

#include "InodeMetadata.h"

bool InodeMetadata::is_directory() const
{
	return (mode & 0xF000u) == MODE_DIRECTORY;
}

bool InodeMetadata::is_simple_file() const
{
	return (mode & 0xF000u) == MODE_FILE;
}

bool InodeMetadata::exists() const
{
	return inode_id != 0;
}

bool InodeMetadata::is_block_device() const
{
	return (mode & 0xF000u) == MODE_BLOCK_DEVICE;
}

bool InodeMetadata::is_character_device() const
{
	return (mode & 0xF000u) == MODE_CHAR_DEVICE;
}

bool InodeMetadata::is_device() const
{
	return (mode & 0xF000u) == MODE_CHAR_DEVICE || (mode & 0xF000u) == MODE_BLOCK_DEVICE;
}