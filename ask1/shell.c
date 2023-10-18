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
        fprintf(stderr, "Wrong format for pipe\n");
        exit(1);
    }
    cmd->io_file = cmd->args[i + 1];
}

int is_pipeline(char **command)
{
    for (int i = 0; command[i] != NULL; i++)
    {
        if (strcmp(command[i], "|") == 0)
        {
            return 1;
        }
    }
    return 0;
}

// cd hi | cat > hi.txt | ls
command *create_pipeline(command *cmd)
{
    int j, i = 0;
    command *prev_cmd = NULL;
    command *curr_cmd = cmd;
    command *head = NULL;

    while (cmd->args[i] != NULL)
    {
        if (strcmp(cmd->args[i], "|") == 0)
        {
            j = 0;
            curr_cmd->args[i] = NULL;
            curr_cmd->next = command_constructor();
            if (head == NULL)
            {
                head = curr_cmd;
                printf("Printing head\n");
                print_command(head);
            }
            prev_cmd = curr_cmd;
            curr_cmd = curr_cmd->next;
            i++;
        }
        else
        {
            printf("ARGS PRIN %s\n", cmd->args[i]);
            curr_cmd->args[j] = (char *)malloc(strlen(cmd->args[i]) + 1);
            strcpy(curr_cmd->args[j], cmd->args[i]);
            printf("ARGS %s\n", curr_cmd->args[j]);
            j++;
        }
        i++;
    }

    // Null-terminate the last command
    curr_cmd->args[i] = NULL;
    return head;
}

void print_next(command *cmd)
{
    while (cmd != NULL)
    {
        for (int i = 0; cmd->args[i] != NULL; i++)
        {
            printf("AAAAARGS %s ", cmd->args[i]);
        }
        printf("\n");
        cmd = cmd->next; // Move to the next command in the pipeline
    }
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
            k++;
            command_token = strtok(NULL, " \n");
        }
        if (is_pipeline(cmd->args))
        {
            print_command(cmd);
            printf("Pipeline detected\n");
            cmd->is_piped = 1;
            cmd = create_pipeline(cmd);
            print_next(cmd);
        }

        if (k > 0)
        {
            contains_redirects(cmd);
            printf("Executing command: %s\n", cmd->args[0]);
            // print_command(cmd);
            if (cmd->is_piped)
            {
                // Handle the pipeline (implement this separately)
                // execute_pipeline(&cmd);
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