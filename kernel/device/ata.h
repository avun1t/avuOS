#ifndef __ATA_H_
#define __ATA_H_

void pio_read_sector(int disk, int sector, uint8_t *buffer);
void pio_prepare_disk(int disk, int sector);
void pio_read_sectors(int disk, int sector, int sectors, uint8_t *buffer);
int pio_get_first_partition(int disk);

#endif // __ATA_H_