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

#ifndef __PARTITIONDEVICE_H_
#define __PARTITIONDEVICE_H_

#include <kernel/device/BlockDevice.h>

class PartitionDevice : public BlockDevice {
public:
	PartitionDevice(unsigned major, unsigned minor, const DC::shared_ptr<BlockDevice> &parent, size_t offset_blocks);
	bool read_blocks(uint32_t block, uint32_t count, uint8_t *buffer) override;
	bool write_blocks(uint32_t block, uint32_t count, uint8_t *buffer) override;
	ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
	size_t block_size() override;
	size_t part_offset();
	DC::shared_ptr<File> parent();

private:
	uint32_t _offset;
	DC::shared_ptr<BlockDevice> _parent;
};

#endif // __PARTITIONDEVICE_H_