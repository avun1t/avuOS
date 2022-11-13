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

#ifndef __AVUOS_TTYDEVICE_H_
#define __AVUOS_TTYDEVICE_H_

#include <common/circular_queue.hpp>
#include "CharacterDevice.h"
#include "KeyboardDevice.h"

#define NUM_TTYS 8

class TTYDevice : public CharacterDevice, public KeyboardHandler {
public:
	static DC::shared_ptr<TTYDevice> current_tty();

	TTYDevice(size_t id, const DC::string &name, unsigned major, unsigned minor);

	ssize_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count) override;
	ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
	void set_active();
	bool active();

	bool buffered = true;

protected:
	static void register_tty(size_t id, TTYDevice *device);
	static void set_current_tty(size_t tty);

private:
	static size_t _current_tty;
	static DC::shared_ptr<TTYDevice> _ttys[NUM_TTYS];

	void handle_key(KeyEvent) override;

	DC::circular_queue<uint8_t> _input_buffer;
	DC::circular_queue<uint8_t> _buffered_input_buffer;
	DC::string _name;
	size_t _id;
	bool _active = false;
};

#endif // __AVUOS_TTYDEVICE_H_