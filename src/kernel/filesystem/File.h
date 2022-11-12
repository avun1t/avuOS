#ifndef __AVUOS_FILE_H_
#define __AVUOS_FILE_H_

#include <common/shared_ptr.hpp>
#include <kernel/Result.hpp>
#include "FileDescriptor.h"
#include "DirectoryEntry.h"

class FileDescriptor;
class File {
public:
	virtual ~File();
	static ResultRet<DC::shared_ptr<FileDescriptor>> open(DC::shared_ptr<File> file, int options);
	virtual bool is_inode();
	virtual ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count);
	virtual ssize_t write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count);
	virtual ssize_t read_dir_entry(FileDescriptor &fd, size_t offset, DirectoryEntry *buffer);

protected:
	File();
};

#endif // __AVUOS_FILE_H_