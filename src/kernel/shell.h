#ifndef __SHELL_H_
#define __SHELL_H_

#include <filesystem/Ext2.h>

void init_shell(Filesystem *fsp);
void shell();
static void command_eval(char *cmd, char *args);

#endif // __SHELL_H_