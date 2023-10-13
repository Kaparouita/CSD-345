#ifndef COMMANDS_H
#define COMMANDS_H
#define ARGS_MAX 1024

// Function prototypes for basic commands
int execute_ls(char *args[ARGS_MAX]);
int execute_date(char *args[ARGS_MAX]);
int execute_cat(char *args[ARGS_MAX]);
int execute_rm(char *args[ARGS_MAX]);
int execute_mkdir(char *args[ARGS_MAX]);

#endif