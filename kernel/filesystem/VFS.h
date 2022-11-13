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

#ifndef __AVUOS_VFS_H_
#define __AVUOS_VFS_H_

#include "LinkedInode.h"
#include "FileDescriptor.h"
#include <common/string.h>

class VFS {
public:
	class Mount {
	public:
		Mount();
		Mount(Filesystem *fs, LinkedInode *host_inode);
		InodeID host_inode();
		Filesystem *fs();
	
	private:
		Filesystem *_fs;
		LinkedInode *_host_inode;
	};

	static VFS &inst();

	ResultRet<DC::shared_ptr<LinkedInode>> resolve_path(DC::string path, const DC::shared_ptr<LinkedInode>& base, DC::shared_ptr<LinkedInode>* parent_storage);
	ResultRet<DC::shared_ptr<FileDescriptor>> open(DC::string path, int options, int mode, DC::shared_ptr<LinkedInode> base);
	DC::shared_ptr<LinkedInode> root_ref();

	VFS();
	~VFS();

	bool mount_root(Filesystem *fs);

private:
	DC::shared_ptr<Inode> _root_inode;
	DC::shared_ptr<LinkedInode> _root_ref;
	Mount mounts[16];
	static VFS *instance;
};

#endif // __AVUOS_VFS_H_