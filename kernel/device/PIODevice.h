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

#ifndef __PIODEVICE_H_
#define __PIODEVICE_H_

#include <kernel/kstddef.h>
#include "BlockDevice.h"

class PIODevice : public BlockDevice {
public:
    PIODevice(unsigned major, unsigned minor, uint8_t id);

    uint8_t disk_id;

    bool read_blocks(uint32_t block, uint32_t count, uint8_t *buffer);
    bool write_blocks(uint32_t block, uint32_t count, uint8_t *buffer);

    size_t block_size();
	ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
};

#endif // __PIODEVICE_H_