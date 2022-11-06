#ifndef __BLOCKDEVICE_H_
#define __BLOCKDEVICE_H_

#include <common.h>
#include "Device.h"

class BlockDevice : public Device {
public:
    BlockDevice();
    bool read_block(uint32_t block, uint8_t *buffer);
    bool write_block(uint32_t block, uint8_t *buffer);

    virtual bool read_blocks(uint32_t block, uint32_t count, uint8_t *buffer);
    virtual bool write_blocks(uint32_t block, uint32_t count, uint8_t *buffer);

    bool read(uint32_t start, uint32_t length, uint8_t *buffer);
    bool write(uint32_t start, uint32_t length, uint8_t *buffer);

    virtual size_t block_size();
};

#endif // __BLOCKDEVICE_H_