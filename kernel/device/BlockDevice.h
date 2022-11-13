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