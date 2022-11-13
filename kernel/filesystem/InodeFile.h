/*
	This file is part of avuOS.
	avuOS is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	avuOS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with avuOS.  If not, see <https://www.gnu.org/licenses/>.
	Copyright (c) avun1t 2022. All rights reserved.
*/

#ifndef __AVUOS_INODEFILE_H_
#define __AVUOS_INODEFILE_H_

#include "File.h"

class InodeFile : public File {
public:
	InodeFile(DC::shared_ptr<Inode>);

	bool is_inode() override;
	DC::shared_ptr<Inode> inode();
	ssize_t read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count) override;
	ssize_t read_dir_entry(FileDescriptor &fd, size_t offset, DirectoryEntry *buffer);

private:
	DC::shared_ptr<Inode> _inode;
};

#endif // __AVUOS_INODEFILE_H_