#ifndef __AVUOS_FILEBASEDFILESYSTEM_H_
#define __AVUOS_FILEBASEDFILESYSTEM_H_

#include "FileSystem.h"

class FileBasedFilesystem : public Filesystem {
public:
	FileBasedFilesystem(DC::shared_ptr<FileDescriptor> file);
	size_t logical_block_size();
	bool read_logical_blocks(size_t block, size_t count, uint8_t *buffer);
	bool read_blocks(size_t block, size_t count, uint8_t *buffer);

protected:
	size_t _logical_block_size {512};
};

#endif // __AVUOS_FILEBASEDFILESYSTEM_H_