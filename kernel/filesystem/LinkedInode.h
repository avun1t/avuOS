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

#ifndef __AVUOS_LINKEDINODE_H_
#define __AVUOS_LINKEDINODE_H_

#include <kernel/filesystem/Inode.h>
#include <common/unique_ptr.hpp>
#include <common/string.h>

class Inode;
class LinkedInode {
public:
	LinkedInode(DC::shared_ptr<Inode> inode, DC::string name, DC::shared_ptr<LinkedInode> parent);
	~LinkedInode();
	DC::shared_ptr<Inode> inode();
	DC::string name();
	DC::shared_ptr<LinkedInode> parent();
	DC::string get_full_path();

private:
	DC::shared_ptr<Inode> _inode;
	DC::string _name;
	DC::shared_ptr<LinkedInode> _parent;
};

#endif // __AVUOS_LINKEDINODE_H_