#ifndef __VFS_H_
#define __VFS_H_

#include <device/BlockDevice.h>
#include <filesystem/Inode.h>

class Inode;
typedef uint32_t InodeID;

class Filesystem {
public:
	BlockDevice *device;
	uint8_t id;
	InodeID rootInode;
	virtual const char *name() const;
	static bool probe(BlockDevice *dev);
	virtual void get_inode(InodeID id, Inode *inode);
	Filesystem(BlockDevice *device);
};

#endif // __VFS_H_