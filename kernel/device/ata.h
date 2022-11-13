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

#ifndef __ATA_H_
#define __ATA_H_

void pio_read_sector(int disk, int sector, uint8_t *buffer);
void pio_prepare_disk(int disk, int sector);
void pio_read_sectors(int disk, int sector, int sectors, uint8_t *buffer);
int pio_get_first_partition(int disk);

#endif // __ATA_H_