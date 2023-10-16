#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commands.h"
#include "mylib.h"


int (*command_functions[])(char *args[]) = {
    execute_ls,
    execute_date,
    execute_cat,
    execute_rm,
    execute_mkdir,
    execute_quit,
    execute_chdir
};


int execute_commands(char input[ARGS_MAX]) {
    char *commands[ARGS_MAX];
    command *cmd = malloc(sizeof(command));
    int i = 0;
    char *token = strtok(input, ";");
    while (token != NULL) {
        commands[i] = token;
        i++;
        token = strtok(NULL, ";");
    }
    commands[i] = NULL;
    for (int j = 0; j < i; j++) {
        char *args[ARGS_MAX];
        int k = 0;
        char *command_token = strtok(commands[j], " \n");
        while (command_token != NULL) {
            args[k] = command_token;
            k++;
            command_token = strtok(NULL, " \n");
        }
        args[k] = NULL;

        if (args[0] != NULL) {
            printf("Executing command: %s\n", args[0]);
            if (is_valid_command(args[0],cmd)) {
                command_functions[*cmd](args);
            }
            else {
                fprintf(stderr, "Unknown command: %s\n", args[0]);
                return 0;
            }
        }
    }
    return 1;
}
