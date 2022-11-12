#ifndef __AVUOS_TTYDEVICE_H_
#define __AVUOS_TTYDEVICE_H_

#include "CharacterDevice.h"

class TTYDevice : public CharacterDevice {
public:
	TTYDevice(unsigned major, unsigned minor);
	size_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count) override;
	size_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
};

#endif // __AVUOS_TTYDEVICE_H_