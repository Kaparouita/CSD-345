#ifndef MYLIB_H
#define MYLIB_H

#include "commands.h"


extern char* current_dir;
int initFunc();
int is_valid_command(const char *input, command *cmd);

#endif