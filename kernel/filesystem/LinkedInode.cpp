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

#include <kernel/kstdio.h>
#include <common/cstring.h>
#include "LinkedInode.h"

LinkedInode::LinkedInode(DC::shared_ptr<Inode> inode, DC::string name, DC::shared_ptr<LinkedInode> parent) : _inode(inode), _parent(parent), _name(name)
{}

LinkedInode::~LinkedInode() = default;

DC::shared_ptr<Inode> LinkedInode::inode()
{
	return _inode;
}

DC::string LinkedInode::name()
{
	return _name;
}

DC::shared_ptr<LinkedInode> LinkedInode::parent()
{
	return _parent;
}

DC::string LinkedInode::get_full_path()
{
	if (parent().get() == nullptr) {
		return "/";
	}

	DC::string ret = "";
	LinkedInode* chain[128];
	int i = 0;

	for (auto* ref = this; ref; ref = ref->parent().get()) {
		chain[i] = ref;
		i++;
	}

	i-=2;
	for (; i >= 0; i--) {
		ret += "/";
		ret += chain[i]->name();
	}
	
	return ret;
}