#ifndef __FAT32_H_
#define __FAT32_H_

typedef struct fat32part {
	uint8_t disk;
	uint16_t part_sect;
	uint8_t sectors_per_cluster;
	uint32_t root_dir_sect;
	uint32_t root_dir_clust;
	uint32_t cluster_begin_sect;
	uint8_t num_fats;
	uint32_t sectors_per_fat;
	uint16_t reserved_sectors;
	uint32_t fat_sect;
	uint32_t current_dir_clust;
} fat32part;

typedef struct fat32file {
	uint32_t cluster;
	uint32_t size;
	uint32_t dir_cluster;
	uint8_t attrib;
} fat32file;

bool is_partition_fat32(int disk, int sect);
fat32part get_fat32_part(int disk, int part_sect);
uint32_t cluster_to_lba(uint32_t cluster);
uint32_t cluster_to_lba_other(fat32part p, uint32_t cluster);
void set_current_fat32_part(fat32part p);
void list_dir(uint32_t cluster, char *filter);
void list_current_dir(char *filter);
uint8_t change_dir(char *dir);
bool is_directory(fat32file file);
fat32file get_file(char *file);
bool exists(fat32file file);
static uint8_t change_one_dir(char *dir);
uint32_t get_cluster_of_entry(uint8_t *entry);
uint32_t get_cluster_chain_size(uint32_t cluster);
uint32_t get_fat_sector_for_cluster(uint32_t cluster);
uint32_t get_next_cluster(uint32_t cluster);
uint32_t get_cluster_of_file(char *file);
void print_file_contents(fat32file f);
void print_current_dir();
void execute_file(fat32file f);
fat32part get_current_fat32_part();

#endif // __FAT32_H_