#ifndef __SHELL_H_
#define __SHELL_H_

#include <filesystem/ext2.h>

void init_shell(filesystem_t *fsp);
void shell();
static void command_eval(char *cmd, char *args);

#endif // __SHELL_H_