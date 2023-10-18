#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commands.h"
#include "mylib.h"

int loopflag = 1;
int contains_redirects(command *cmd);

int main()
{
    initFunc();
    printf("Welcome to the shell!\n");
    while (loopflag)
    {
        // Read a command from the user
        char input[ARGS_MAX];
        printf("<4300>-hy345sh@<giorgis><%s>", get_current_dir());
        fgets(input, sizeof(input), stdin);
        execute_commands(input);
    }
    return 0;
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

// cd hi | cat > hi.txt | ls
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

int execute_commands(char input[ARGS_MAX])
{
    char *commands[ARGS_MAX];
    int num_commands = split_command(input, commands);

    for (int j = 0; j < num_commands; j++)
    {
        command *cmd = command_constructor();
        int k = 0;

        char *command_token = strtok(commands[j], " \n");
        while (command_token != NULL)
        {
            cmd->args[k] = (char *)malloc(strlen(command_token) + 1);
            strcpy(cmd->args[k], command_token);
            cmd->args[k][strlen(command_token)] = '\0';
            k++;
            command_token = strtok(NULL, " \n");
        }
        commands[k] = NULL;
        if (is_pipeline(cmd->args))
        {
            cmd = create_pipeline(cmd);
        }
        if (k > 0)
        {
            contains_redirects(cmd);
            printf("Executing command: %s\n", cmd->args[0]);
            print_command(cmd);
            if (cmd->is_piped)
            {
                // Handle the pipeline (implement this separately)
                execute_pipeline(cmd);
            }
            else if (is_built_in_cmd(cmd->args[0]))
            {
                execute_built_in_commands(cmd);
            }
            else if (!cmd->is_piped && cmd->symbol == None)
            {
                execute_simple_commands(cmd->args);
            }
            else
            {
                execute_command_with_redirection(cmd);
            }
        }
    }
    return 1;
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