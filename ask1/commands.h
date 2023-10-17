#ifndef COMMANDS_H
#define COMMANDS_H
#define ARGS_MAX 1024
#define PATH_MAX 512

typedef enum command{
    ls,date,cat,rm,mkdir,quit,cdir
}command;

typedef struct req{
    command    op;
    char*      args[ARGS_MAX];
    char*      result;
}req;

extern int loopflag;

int execute_commands(char input[ARGS_MAX]);
int execute_ls(char *args[ARGS_MAX]);
int execute_date(char *args[ARGS_MAX]);
int execute_cat(char *args[ARGS_MAX]);
int execute_rm(char *args[ARGS_MAX]);
int execute_mkdir(char *args[ARGS_MAX]);
int execute_quit(char *args[ARGS_MAX]);
int execute_chdir(char *args[ARGS_MAX]);


char *get_current_dir();


#endif