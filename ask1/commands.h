#ifndef COMMANDS_H
#define COMMANDS_H
#define ARGS_MAX 1024
#define PATH_MAX 512

typedef enum cmd_name
{
    quit,
    cdir
} cmd_name;

typedef enum symbol
{
    O_redirect,  // >
    I_redirect,  // <
    DO_redirect, // >>
    None,
} symbol;

typedef struct command
{
    char **args;          // arguments for the command
    symbol symbol;        // redirection symbol
    char *io_file;        // file for redirection
    int is_piped;         // is the command piped?
    struct command *next; // next command in pipeline
} command;

command *command_constructor();
void print_command(command *cmd);

extern int loopflag;

int execute_commands(char input[ARGS_MAX]);
int execute_quit(char *args[ARGS_MAX]);
int execute_chdir(char *args[ARGS_MAX]);
int execute_simple_commands(char *args[ARGS_MAX]);

int is_built_in_cmd(char *cmd);

int execute_built_in_commands(command *cmd);
void execute_command_with_redirection(command *cmd);
void execute_pipeline(command *cmd);

char *get_current_dir();
#endif