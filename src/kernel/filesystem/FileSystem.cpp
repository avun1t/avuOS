#include <kernel/filesystem/FileSystem.h>
#include "InodeRef.h"

Filesystem::Filesystem(BlockDevice &device) : _device(device)
{
    this->_device = device;
}

const char *Filesystem::name() const
{
    return nullptr;
}

bool Filesystem::probe(BlockDevice &dev)
{
    return false;
}

DC::shared_ptr<Inode> Filesystem::get_inode(InodeID id)
{
	return DC::shared_ptr<Inode>(get_inode_rawptr(id));
}

Inode *Filesystem::get_inode_rawptr(InodeID id)
{
	return nullptr;
}

InodeID Filesystem::root_inode()
{
	return root_inode_id;
}

BlockDevice &Filesystem::device()
{
	return _device;
}