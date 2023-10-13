#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commands.h"

char *currFolder;
int initFunc();
char *getcf();


int main() {
    initFunc();
    printf("Welcome to the shell!\n");
    while (1) {
        // Read a command from the user
        char input[ARGS_MAX];
        printf("<4300>-hy345sh@<giorgis>%s", getcf());
        fgets(input, sizeof(input), stdin);

        // Tokenize the input by semicolon
        char *commands[ARGS_MAX];
        int i = 0;
        char *token = strtok(input, ";");
        while (token != NULL) {
            commands[i] = token;
            i++;
            token = strtok(NULL, ";");
        }
        commands[i] = NULL;

        // Execute the command
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
                if (strcmp(args[0], "mkdir") == 0) {
                    execute_mkdir(args);
                } else if (strcmp(args[0], "ls") == 0) {
                    execute_ls(args);
                } // Add more commands here
                else {
                    fprintf(stderr, "Unknown command: %s\n", args[0]);
                }
            }
        }
    }
    return 0;
}


int initFunc(){
    currFolder = (char *)malloc(strlen("") + 1);  // +1 for the null terminator
    if (currFolder == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    return 0;
}

int moveDir(char *dir){
    if (chdir(dir) == -1) {
        perror("chdir() to dir1 failed");
        return 1;
    }
    return 0;
    /*currFolder = (char *)malloc(strlen("dir1>") + 1);  // +1 for the null terminator
    if (currFolder == NULL) {
        perror("Memory allocation failed");
        return 1;
    }
    strcpy(currFolder, "dir1");
    */
}

char *getcf() {
    return currFolder;
}
