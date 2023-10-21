#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <libgen.h>

char *current_dir;

int execute_chdir(char *args[ARGS_MAX])
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "cd: missing argument\n");
    }
    else if (chdir(args[1]) != 0)
    {
        perror("chdir");
        return 1;
    }
    else
    {
        current_dir = (char *)malloc(PATH_MAX);
        if (getcwd(current_dir, PATH_MAX) != NULL)
        {
            current_dir = basename(current_dir);
        }
    }
    return 1;
}

char *get_current_dir()
{
    return current_dir;
}

int execute_quit(char *args[ARGS_MAX])
{
    exit(0);
}

int is_built_in_cmd(char *cmd)
{
    if (strcmp(cmd, "cd") == 0 || strcmp(cmd, "chdir") == 0)
    {
        return 1;
    }
    if (strcmp(cmd, "quit") == 0)
    {
        return 1;
    }
    return 0;
}

command *command_constructor()
{
    command *cmd = (command *)malloc(sizeof(command));
    if (cmd == NULL)
    {
        perror("Failed to allocate memory for command");
        exit(1);
    }

    cmd->next = NULL;
    cmd->args = (char **)malloc(ARGS_MAX * sizeof(char *));
    cmd->is_piped = 0;
    cmd->symbol = None;

    return cmd;
}

command *create_pipeline(command *cmd)
{
    int j, i = 0;
    command *curr_cmd = command_constructor();
    command *head = NULL;

    while (cmd->args[i] != NULL)
    {
        if (strcmp(cmd->args[i], "|") == 0)
        {
            curr_cmd->is_piped = 1;
            curr_cmd->args[j] = NULL;
            curr_cmd->next = command_constructor();
            if (head == NULL)
                head = curr_cmd;
            curr_cmd = curr_cmd->next;
            j = 0;
        }
        else
        {
            curr_cmd->args[j] = (char *)malloc(strlen(cmd->args[i]) + 1);
            strcpy(curr_cmd->args[j], cmd->args[i]);
            j++;
        }
        i++;
    }
    // Null-terminate the last command
    curr_cmd->args[i] = NULL;
    return head;
}


void print_command(command *cmd)
{
    printf("Command: %s\n", cmd->args[0]);
    printf("Arguments:\n");
    for (int i = 1; cmd->args[i] != NULL; i++)
    {
        printf("- %s\n", cmd->args[i]);
    }

    printf("Redirection Symbol: ");
    switch (cmd->symbol)
    {
    case O_redirect:
        printf("Output Redirection (>)\n");
        break;
    case I_redirect:
        printf("Input Redirection (<)\n");
        break;
    case DO_redirect:
        printf("Append Output Redirection (>>)\n");
        break;
    default:
        printf("None\n");
        break;
    }

    printf("Piped: %s\n", cmd->is_piped ? "Yes" : "No");
}
