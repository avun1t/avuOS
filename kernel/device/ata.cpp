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

#include <kernel/kstddef.h>
#include <kernel/kstdio.h>
#include <kernel/device/ata.h>

/*
 * This system is to read disks formatted with an MBR only.
 * For reading specific filesystems, see the respective files in c/drivers/disk.
 */
uint8_t boot_disk = 0;
uint8_t ata_buf[512];
uint8_t ata_buf2[512];

void pio_prepare_disk(int disk, int address)
{
    outb(0x1F1, 0x00);
    outb(0x1F2, 0x01);
    outb(0x1F3, (uint8_t)address);
    outb(0x1F4, (uint8_t)(address >> 8));
    outb(0x1F5, (uint8_t)(address >> 16));
    outb(0x1F6, 0xE0 | (disk << 4) | ((address >> 24) & 0x0F));
    outb(0x1F7, 0x20);
    while (!(inb(0x1F7) & 0x08)) {}
}

void pio_read_sector(int disk, int sector, uint8_t *buffer)
{
    pio_prepare_disk(disk, sector);

    for (int i = 0; i < 256; i++) {
        uint16_t tmp = inw(0x1F0);
        buffer[i*2] = (uint8_t)tmp;
        buffer[i*2+1] = (uint8_t)(tmp >> 8);
    }
}

void pio_read_sectors(int disk, int sector, int sectors, uint8_t *buffer)
{
    for (int i = 0; i < sectors; i++) {
        pio_read_sector(disk, sector+i, buffer+i*512);
    }
}

int pio_get_first_partition(int disk)
{
    pio_prepare_disk(disk, 0);
    uint16_t pos = 0;

    for (int i = 0; i < 256; i++) {
        uint16_t tmpword = inw(0x1F0);
        if (i == 227) {
            pos = tmpword;
        }
    }

    return pos;
}