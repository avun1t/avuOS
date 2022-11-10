#ifndef __VFS_H_
#define __VFS_H_

#include <kernel/device/BlockDevice.h>
#include <kernel/filesystem/Inode.h>

class Inode;
typedef uint32_t InodeID;

class Filesystem {
public:
	virtual const char *name() const;
	static bool probe(BlockDevice *dev);
	virtual Inode *get_inode(InodeID id);
	Filesystem(BlockDevice *device);
	InodeID root_inode();
	BlockDevice *device();
protected:
	BlockDevice *_device;
	uint8_t id;
	InodeID root_inode_id;
};

#endif // __VFS_H_