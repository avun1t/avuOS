#ifndef __INODEREF_H_
#define __INODEREF_H_

#include <kernel/filesystem/Inode.h>

class InodeRef {
public:
	InodeRef(DC::shared_ptr<Inode> inode, string name, DC::shared_ptr<InodeRef> parent);
	~InodeRef();
	DC::shared_ptr<Inode> inode();
	string name();
	DC::shared_ptr<InodeRef> parent();
	void get_full_path(char *buffer);

private:
	DC::shared_ptr<Inode> _inode;
	string _name;
	DC::shared_ptr<InodeRef> _parent;
};

#endif // __INODEREF_H_