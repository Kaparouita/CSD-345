#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "commands.h"
#include "mylib.h"

int (*command_functions[])(char *args[]) = {
    execute_quit,
    execute_chdir};

int execute_built_in_commands(command *cmd)
{
    if (strcmp(cmd->args[0], "cd") == 0 || strcmp(cmd->args[0], "chdir") == 0)
    {
        return (*command_functions[1])(cmd->args);
    }
    if (strcmp(cmd->args[0], "quit") == 0)
    {
        return (*command_functions[0])(cmd->args);
    }
    return (*command_functions[0])(cmd->args);
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
            //printf("Executing command: %s\n", cmd->args[0]);
            //print_command(cmd);
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


void execute_command_with_redirection(command *cmd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        // Input Redirection <
        if (cmd->symbol == I_redirect)
        {
            int input_fd = open(cmd->io_file, O_RDONLY);
            if (input_fd < 0)
            {
                perror("Input redirection");
                exit(1);
            }
            close(STDIN_FILENO);          // Close the current stdin
            dup2(input_fd, STDIN_FILENO); // Associate stdin with input_fd
            close(input_fd);
        }

        // Output Redirection >
        if (cmd->symbol == O_redirect)
        {
            int output_fd = open(cmd->io_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd < 0)
            {
                perror("Output redirection");
                exit(1);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Append Output Redirection >>
        if (cmd->symbol == DO_redirect)
        {
            int append_fd = open(cmd->io_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (append_fd < 0)
            {
                perror("Append output redirection");
                exit(1);
            }
            dup2(append_fd, STDOUT_FILENO);
            close(append_fd);
        }

        execvp(cmd->args[0], cmd->args);
        fprintf(stderr, "Command not found: %s\n", cmd->args[0]);
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
    }
}

void execute_pipeline(command *cmd)
{
    int pipefds[2];
    int prev_pipe = -1;
    pid_t child_pid;
    while (cmd != NULL)
    {
        if (cmd->is_piped)
            pipe(pipefds);

        child_pid = fork();
        if (child_pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (child_pid == 0)
        {
            if (prev_pipe != -1)
            {
                dup2(prev_pipe, STDIN_FILENO);
                close(prev_pipe);
            }

            if (cmd->is_piped)
            {
                dup2(pipefds[1], STDOUT_FILENO);
                close(pipefds[0]);
            }

            // execute
            execvp(cmd->args[0], cmd->args);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (cmd->is_piped)
            {
                close(pipefds[1]);
                prev_pipe = pipefds[0];
            }
        }
        cmd = cmd->next;
    }

    if (prev_pipe != -1)
        close(prev_pipe);

    while (wait(NULL) > 0)
        ;
}

int execute_simple_commands(char *args[ARGS_MAX])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        fprintf(stderr, "Command not found or wrong format for : %s\n", args[0]);
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
    }
    return 1;
}
