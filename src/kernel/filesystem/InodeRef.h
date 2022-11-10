#ifndef __INODEREF_H_
#define __INODEREF_H_

#include <kernel/filesystem/Inode.h>

class InodeRef {
public:
	InodeRef(Inode &inode, string name, InodeRef *parent);
	Inode &inode();
	string name();
	InodeRef *parent();

private:
	Inode &_inode;
	string _name;
	InodeRef *_parent;
};

#endif // __INODEREF_H_