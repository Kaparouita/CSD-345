#ifndef MYLIB_H
#define MYLIB_H

#include "commands.h"

extern char *current_dir;
int initFunc();
int is_valid_command(const char *input, cmd_name *cmd);
int is_pipeline(char **command);
int contains_redirects(command *cmd);
void update_args(command *cmd, int i);
int split_command(char *input, char *commands[ARGS_MAX]);

#endif