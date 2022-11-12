#ifndef __INODE_H_
#define __INODE_H_

#include <kernel/kstddef.h>
#include <common/shared_ptr.hpp>
#include <common/string.h>
#include "InodeMetadata.h"
#include "DirectoryEntry.h"

class Filesystem;
typedef uint32_t InodeID;

class Inode {
public:
	InodeID id;
	Filesystem &fs;

	Inode(Filesystem &fs, InodeID id);

	virtual DC::shared_ptr<Inode> find(DC::string name);
	virtual Inode *find_rawptr(DC::string name);
	virtual ssize_t read(size_t start, size_t length, uint8_t *buffer) = 0;
	virtual ssize_t read_dir_entry(size_t start, DirectoryEntry *buffer) = 0;

	InodeMetadata metadata();

protected:
	InodeMetadata _metadata;
};

#endif // __INODE_H_