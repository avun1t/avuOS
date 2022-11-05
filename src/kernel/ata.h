#ifndef __ATA_H_
#define __ATA_H_

void prepare_disk(int disk, int address);
void read_sector(int disk, int address, uint8_t *sect);
void read_sectors(int disk, int address, int sectors, uint8_t *sect);
int get_first_partition(int disk);
int get_fs_type(int disk);

#endif // __ATA_H_