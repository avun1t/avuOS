#ifndef __PARTITIONDEVICE_H_
#define __PARTITIONDEVICE_H_

#include <device/BlockDevice.h>

class PartitionDevice : public BlockDevice {
public:
	uint32_t offset;
	BlockDevice *parent;

	PartitionDevice(BlockDevice *parent, uint32_t offset);
	bool read_blocks(uint32_t block, uint32_t count, uint8_t *buffer) override;
	bool write_blocks(uint32_t block, uint32_t count, uint8_t *buffer) override;
	size_t block_size() override;
};

#endif // __PARTITIONDEVICE_H_