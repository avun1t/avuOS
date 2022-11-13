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

#ifndef __SHELL_H_
#define __SHELL_H_

#include <kernel/filesystem/Ext2.h>
#include <kernel/filesystem/LinkedInode.h>

class Shell {
public:
	Shell();
	void shell();
	void command_eval(char *cmd, char *args);

private:
	char cmdbuf[256] = {0};
	char argbuf[256] = {0};
	char dir_buf[256] = {0};
	bool exitShell = false;
	DC::shared_ptr<LinkedInode> current_dir;
};

#endif // __SHELL_H_