#ifndef __AVUOS_NULLDEVICE_H_
#define __AVUOS_NULLDEVICE_H_

#include "CharacterDevice.h"

class NullDevice : public CharacterDevice {
public:
	NullDevice();
	size_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override
	size_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count) override;
};

#endif // __AVUOS_NULLDEVICE_H_