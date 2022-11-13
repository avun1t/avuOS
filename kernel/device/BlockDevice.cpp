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
#include <common/defines.h>
#include "BlockDevice.h"

BlockDevice::BlockDevice(unsigned major, unsigned minor) : Device(major, minor)
{}

bool BlockDevice::read_block(uint32_t block, uint8_t *buffer)
{
	return this->read_blocks(block, 1, buffer);
}

bool BlockDevice::write_block(uint32_t block, uint8_t *buffer)
{
	return this->write_blocks(block, 1, buffer);
}

bool BlockDevice::read_blocks(uint32_t block, uint32_t count, uint8_t *buffer)
{
	return false;
}

bool BlockDevice::write_blocks(uint32_t block, uint32_t count, uint8_t *buffer)
{
	return false;
}

size_t BlockDevice::block_size()
{
	return 0;
}

bool BlockDevice::is_block_device()
{
	return true;
}