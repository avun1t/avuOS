#ifndef __AVUOS_RANDOMDEVICE_H_
#define __AVUOS_RANDOMDEVICE_H_

#include "CharacterDevice.h"

class RandomDevice: public CharacterDevice {
public:
	RandomDevice();
	ssize_t read(FileDescriptor& fd, size_t offset, uint8_t* buffer, size_t count) override;
	ssize_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count) override;
};


#endif // __AVUOS_RANDOMDEVICE_H_