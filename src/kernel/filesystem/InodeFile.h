#ifndef __AVUOS_INODEFILE_H_
#define __AVUOS_INODEFILE_H_

#include "File.h"

class InodeFile : public File {
public:
	InodeFile(DC::shared_ptr<Inode>);

	bool is_inode() override;
	DC::shared_ptr<Inode> inode();
	size_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
	size_t read_dir_entry(FileDescriptor &fd, size_t offset, DirectoryEntry *buffer);

private:
	DC::shared_ptr<Inode> _inode;
};

#endif // __AVUOS_INODEFILE_H_