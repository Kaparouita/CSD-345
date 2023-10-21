#include "commands.h"
#include "mylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <libgen.h>

int initFunc()
{
    current_dir = (char *)malloc(PATH_MAX);
    if (getcwd(current_dir, PATH_MAX) != NULL)
    {
        current_dir = basename(current_dir);
    }
    return 0;
}

int is_valid_command(const char *input, cmd_name *cmd)
{
    if (strcmp(input, "quit") == 0)
    {
        *cmd = quit;
        return 1;
    }
    else if (strcmp(input, "chdir") == 0 || strcmp(input, "cd") == 0)
    {
        *cmd = cdir;
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_pipeline(char **command)
{
    char **cmd = command;
    while (*cmd != NULL)
    {
        if (strcmp(*cmd, "|") == 0)
        {
            return 1;
        }
        cmd++;
    }
    return 0;
}

int contains_redirects(command *cmd)
{
    for (int i = 0; cmd->args[i] != NULL; i++)
    {
        if (strcmp(cmd->args[i], ">") == 0)
        {
            cmd->symbol = O_redirect;

            update_args(cmd, i);
            return 1;
        }
        else if (strcmp(cmd->args[i], "<") == 0)
        {
            cmd->symbol = I_redirect;
            update_args(cmd, i);
            return 1;
        }
        else if (strcmp(cmd->args[i], ">>") == 0)
        {
            cmd->symbol = DO_redirect;
            update_args(cmd, i);
            return 1;
        }
    }
    return 0;
}

void update_args(command *cmd, int i)
{
    if (cmd->args[i + 1] == NULL)
    {
        fprintf(stderr, "Wrong format for redirection\n");
        exit(1);
    }
    cmd->args[i] = NULL;
    cmd->io_file = cmd->args[i + 1];
}

int split_command(char *input, char *commands[ARGS_MAX])
{
    int i = 0;
    char *token = strtok(input, ";");
    while (token != NULL)
    {
        commands[i] = token;
        i++;
        token = strtok(NULL, ";");
    }
    commands[i] = NULL;
    return i;
}



