#ifndef __SHELL_H_
#define __SHELL_H_

#include <kernel/filesystem/Ext2.h>

void init_shell();
void shell();
static void command_eval(char *cmd, char *args);

#endif // __SHELL_H_