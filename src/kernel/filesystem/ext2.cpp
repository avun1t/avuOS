#include <common.h>
#include <memory/paging.h>
#include <memory/heap.h>
#include <ata.h>
#include <stdio.h>
#include <filesystem/ext2.h>
#include <filesystem/vfs.h>

extern uint8_t ata_buf[512], ata_buf2[512];

bool is_partition_ext2(int disk, int sect)
{
	read_sector(disk, sect+2, ata_buf); //Supercluster begins at partition sector + 2
	return ((ext2_superblock *)ata_buf)->signature == EXT2_SIGNATURE;
}

bool ext2_probe(filesystem_t *fs)
{
	if (strcmp(fs->type,"EXT2")) {
		ext2_partition *part = (ext2_partition *)fs->fs_data;
		return is_partition_ext2(part->disk, part->sector);
	}
	return 0;
}

void get_ext2_superblock(int disk, int sect, ext2_superblock *sp)
{
	read_sector(disk, sect+2, (uint8_t *)sp);

	if (sp->version_major < 1) { //If major version is less than 1, then use defaults for stuff
		sp->first_inode = 11;
		sp->inode_size = 128;
	}
}

void init_ext2_partition(int sect, device_t *device, ext2_superblock *sb, ext2_partition *part, filesystem_t *fs)
{
	part->sector = sect;
	part->disk = device->disk;
	part->block_size = 1024 << (sb->block_size);
	part->block_group_descriptor_table = sb->superblock_block+1;
	part->blocks_per_inode_table = (sb->inode_size*sb->inodes_per_group)/ext2_get_block_size(part);
	part->sectors_per_inode_table = (sb->inode_size*sb->inodes_per_group)/512;
	part->sectors_per_block = ext2_get_block_size(part)/512;
	part->num_block_groups = sb->total_blocks/sb->blocks_per_group + (sb->total_blocks % sb->blocks_per_group != 0);
	part->inodes_per_block = ext2_get_block_size(part)/sb->inode_size;
	part->superblock = sb;
	fs->type = "EXT2";
	fs->probe = ext2_probe;
	fs->read = (bool(*)(file_t *, uint8_t *, filesystem_t *))ext2_read_file;
	fs->read_part = (bool(*)(file_t *, uint8_t *, uint32_t, uint32_t, filesystem_t *))ext2_read_file;
	fs->fs_data = part;
	fs->get_file = (bool(*)(char *, file_t *, filesystem_t *))ext2_get_file;
	fs->list_dir = (bool(*)(file_t *, filesystem_t *))ext2_list_directory;
	fs->device = device;
}

uint32_t ext2_get_block_group_of_inode(uint32_t node, ext2_partition *part)
{
	return (node - 1) / ext2_get_superblock(part)->inodes_per_group;
}

uint32_t ext2_get_index_of_inode(uint32_t node, ext2_partition *part)
{
	return (node - 1) % ext2_get_superblock(part)->inodes_per_group;
}

//inode is the actual number of the inode, not the index or anything.
uint32_t ext2_get_block_of_inode(uint32_t node, ext2_partition *part)
{
	return (ext2_get_index_of_inode(node, part) * ext2_get_superblock(part)->inode_size) / ext2_get_block_size(part);
}

void ext2_read_inode(uint32_t inode, ext2_inode *buf, ext2_partition *part)
{
	uint32_t bg = ext2_get_block_group_of_inode(inode, part);
	uint8_t *read = ext2_alloc_block(part);
	ext2_block_group_descriptor *d = (ext2_block_group_descriptor *)ext2_read_block(2, read, part);
	for(int i = 0; i < bg; i++) d++; //note to self - d++ adds to the pointer by sizeof(ext2_block_group_descriptor)

	ext2_read_block(d->inode_table+ext2_get_block_of_inode(inode, part), (uint8_t *)buf, part);
	ext2_inode *in = (ext2_inode *)buf;
	uint32_t index = ext2_get_index_of_inode(inode, part) % part->inodes_per_block;
	for(int i = 0; i < index; i++) in++; //same here as above

	memcpy(buf, in, sizeof(ext2_inode));
	ext2_free_block(read, part);
}

bool ext2_list_directory(file_t *file, filesystem_t *fs)
{
	ext2_partition *part = (ext2_partition *)fs->fs_data;
	ext2_inode *inode = (ext2_inode *)kmalloc(sizeof(inode));
	ext2_read_inode(file->fs_id,inode,part);

	if ((inode->type & 0xF000) != EXT2_DIRECTORY) {
		printf("Given file is not a directory! %d\n", inode->last_access_time);
		kfree(inode, sizeof(inode));
		return 0;
	} else {
		uint8_t *buf = ext2_alloc_block(part);

		for (int i = 0; i < 12; i++) {
			uint32_t block = inode->block_pointers[i];

			if (block == 0 || block > ext2_get_superblock(part)->total_blocks) break;
			ext2_read_block(block, buf, part);
			ext2_list_directory_entries((ext2_directory *)buf, part);
		}
		ext2_free_block(buf, part);
	}

	kfree(inode, sizeof(inode));
	return 1;
}

void ext2_list_directory_entries(ext2_directory *dir, ext2_partition *part)
{
	uint32_t add = 0;

	while (dir->inode != 0 && add < ext2_get_block_size(part)) {
		char *name = (char *)kmalloc(dir->name_length + 1);

		name[dir->name_length] = '\0';
		memcpy(name, &dir->type+1, dir->name_length);
		if(name[0] != '.' && name[0] != '\0') printf("%s\n", name);

		kfree(name, dir->name_length + 1);
		add+=dir->size;
		dir = (ext2_directory *)((uint32_t)dir + dir->size);
	}
}

//dir_inode will be set to 2 if find_name starts with '/'
uint32_t ext2_find_file(char *find_name, uint32_t dir_inode, ext2_inode *inode, ext2_partition *part)
{
	if (find_name[0] == '/'){ 
		find_name++;
		dir_inode = 2;
	}

	uint32_t namelen = strlen(find_name);
	if(namelen == 0) return dir_inode;
	uint8_t *buf = ext2_alloc_block(part);
	char *cfind_name = (char*)kmalloc(namelen);

	while (*find_name != 0) {
		uint32_t strindex = index_of('/',find_name);
		substrr(0,strindex,find_name,cfind_name);
		find_name+=strindex+(strindex == strlen(find_name) ? 0 : 1);
		ext2_read_inode(dir_inode,inode,part);
		bool found = 0;

		for (int i = 0; i < 12 && !found; i++) {
			uint32_t block = inode->block_pointers[i];

			if (block == 0 || block > ext2_get_superblock(part)->total_blocks) break;
			ext2_read_block(block, buf, part);
			ext2_directory *dir = (ext2_directory *)buf;
			uint32_t add = 0;

			while(dir->inode != 0 && add < ext2_get_block_size(part) && !found){
				char *name = (char*)kmalloc(dir->name_length + 1);
				name[dir->name_length] = '\0';
				memcpy(name, &dir->type+1, dir->name_length);

				if (strcmp(name, cfind_name)) {
					dir_inode = dir->inode;
					ext2_read_inode(dir_inode,inode,part);
					found = 1;
				}

				kfree(name, dir->name_length + 1);
				add+=dir->size;
				dir = (ext2_directory *)((uint32_t)dir + dir->size);
			}
		}
		if (!found) {
			dir_inode = 0;
			find_name+= strlen(find_name);
		}
	}

	kfree(cfind_name, namelen);
	ext2_free_block(buf,part);
	return dir_inode;
}

bool ext2_get_file(char *fn, file_t *file, filesystem_t *fs)
{
	ext2_inode *inode = (ext2_inode *)kmalloc(sizeof(ext2_inode));
	uint32_t id = ext2_find_file(fn, 2, inode, (ext2_partition *)(fs->fs_data));
	file->size = id ? inode->size_lower : 0;
	file->sectors = id ? inode->sectors_in_use : 0;
	file->fs_id = id;
	file->isDirectory = id ? (inode->type & 0xF000) == EXT2_DIRECTORY : 0;

	kfree(inode, sizeof(ext2_inode));
	return id ? true : false; // not casting to uint8_t because if id has zero in lower 8 bits then it will return false
}

// buf must be a multiple of the blocksize
bool ext2_read_file(file_t *file, uint8_t *buf, filesystem_t *fs)
{
	if (!file->fs_id) return 0;

	ext2_partition *part = (ext2_partition *)(fs->fs_data);
	ext2_inode *inode = (ext2_inode*)kmalloc(sizeof(ext2_inode));
	ext2_read_inode(file->fs_id, inode, part);

	for (int i = 0; i < 12; i++) {
		uint32_t block = inode->block_pointers[i];
		if (block == 0 || block > ext2_get_superblock(part)->total_blocks){break;}
		ext2_read_block(block, buf, part);
	}

	if (inode->s_pointer)
		ext2_read_slink(inode->s_pointer, buf+12*ext2_get_block_size(part), part);
	if (inode->d_pointer)
		ext2_read_dlink(inode->d_pointer, buf+(ext2_get_block_size(part)/sizeof(uint32_t))*ext2_get_block_size(part)+12*ext2_get_block_size(part), part);
	if (inode->t_pointer)
		printf("WARNING! File uses t_pointer. Will not work.\n");
	
	kfree(inode, sizeof(ext2_inode));
	return 1;
}

bool ext2_read_part_of_file(file_t *file, uint8_t *buf, uint32_t offset, uint32_t amount, filesystem_t *fs)
{
	//TODO
	return false;
}

void ext2_read_slink(uint32_t block, uint8_t *buf, ext2_partition *part)
{
	uint8_t *bbuf = ext2_alloc_block(part);
	ext2_read_block(block, bbuf, part);
	uint32_t *blocks = (uint32_t *)bbuf;
	uint32_t numblocks = ext2_get_block_size(part)/sizeof(uint32_t);

	for (int i = 0; i < numblocks; i++){
		if (blocks[i] == 0) break;
		ext2_read_block(blocks[i], buf+i*ext2_get_block_size(part), part);
	}

	ext2_free_block(bbuf,part);
}

void ext2_read_dlink(uint32_t block, uint8_t *buf, ext2_partition *part)
{
	uint8_t *bbuf = ext2_alloc_block(part);

	ext2_read_block(block, bbuf, part);

	uint32_t *blocks = (uint32_t *)bbuf;
	uint32_t numblocks = ext2_get_block_size(part)/sizeof(uint32_t);
	uint32_t singsize = numblocks*ext2_get_block_size(part);

	for (int i = 0; i < numblocks; i++) {
		if (blocks[i] == 0) break;
		ext2_read_block(blocks[i], buf+i*singsize, part);
	}

	ext2_free_block(bbuf, part);
}

ext2_superblock *ext2_get_superblock(ext2_partition *part)
{
	return part->superblock;
}

uint32_t ext2_get_block_size(ext2_partition *part)
{
	return part->block_size;
}

uint8_t *ext2_read_block(uint32_t block, uint8_t *buf, ext2_partition *part)
{
	read_sectors(part->disk, ext2_block_to_sector(block, part), part->sectors_per_block, buf);
	return buf;
}

uint8_t *ext2_alloc_block(ext2_partition *part)
{
	return (uint8_t *)kmalloc(ext2_get_block_size(part));
}

void ext2_free_block(uint8_t *block, ext2_partition *part)
{
	kfree(block, ext2_get_block_size(part));
}

uint32_t ext2_block_to_sector(uint32_t block, ext2_partition *part)
{
	return part->sector+(ext2_get_block_size(part)/512)*block;
}