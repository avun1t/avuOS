#ifndef __AVUOS_ZERODEVICE_H_
#define __AVUOS_ZERODEVICE_H_

#include "CharacterDevice.h"

class ZeroDevice: public CharacterDevice {
public:
	ZeroDevice();
	ssize_t read(FileDescriptor& fd, size_t offset, uint8_t* buffer, size_t count) override;
	ssize_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count) override;
};


#endif // __AVUOS_ZERODEVICE_H_