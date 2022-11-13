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

#ifndef __PIT_H_
#define __PIT_H_

#define PIT_COUNTER0 0x40
#define PIT_COUNTER1 0x41
#define PIT_COUNTER2 0x42
#define PIT_CMD 0x43

void pit_handler();
void pit_init(uint32_t frequency);

#endif // __PIT_H_