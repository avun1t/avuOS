#ifndef __IDE_H_
#define __IDE_H_

#include <pci/pci.h>

#define ATA_PRIMARY 0x0
#define ATA_SECONDARY 0x1

namespace IDE {
	class PATAChannel {
	public:
		PCI::Address address;
		uint8_t channel;
		PATAChannel(PCI::Address address, uint8_t channel);
		void init();
	};

	PATAChannel find_pata_channel(uint8_t channel);
}

#endif // __IDE_H_