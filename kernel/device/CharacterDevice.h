#ifndef __AVUOS_CHARACTERDEVICE_H_
#define __AVUOS_CHARACTERDEVICE_H_

#include "Device.h"

class CharacterDevice : public Device {
public:
	CharacterDevice(unsigned major, unsigned minor);
	bool is_character_device() override;
};

#endif // __AVUOS_CHARACTERDEVICE_H_