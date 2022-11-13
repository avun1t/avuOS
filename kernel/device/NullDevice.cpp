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

#include "NullDevice.h"

NullDevice::NullDevice() : CharacterDevice(1, 3)
{}

ssize_t NullDevice::read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count)
{
	return 0;
}

ssize_t NullDevice::write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count)
{
	return 0;
}