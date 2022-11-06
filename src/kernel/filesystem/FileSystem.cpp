#include <filesystem/FileSystem.h>

filesystem::Filesystem(BlockDevice *device)
{
    this->device = device;
}

const char *Filesystem::name() const
{
    return nullptr;
}

bool Filesystem::probe(BlockDevice *dev)
{
    return false;
}

void Filesystem::get_inode(InodeID id, Inode *inode)
{}