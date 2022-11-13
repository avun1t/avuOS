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

#ifndef __BLOCKDEVICE_H_
#define __BLOCKDEVICE_H_

#include <kernel/kstddef.h>
#include "Device.h"

class BlockDevice : public Device {
public:
    BlockDevice(unsigned major, unsigned minor);
    bool read_block(uint32_t block, uint8_t *buffer);
    bool write_block(uint32_t block, uint8_t *buffer);

    virtual bool read_blocks(uint32_t block, uint32_t count, uint8_t *buffer);
    virtual bool write_blocks(uint32_t block, uint32_t count, uint8_t *buffer);

    virtual size_t block_size();
	bool is_block_device() override;
};

#endif // __BLOCKDEVICE_H_