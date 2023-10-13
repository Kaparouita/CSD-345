#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int execute_ls(char *args[ARGS_MAX]) {
    printf("hello");
    pid_t pid = fork();
    if (pid == 0) {
        execvp("ls", args);
        perror("ls");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
    }
    return 1; 
}

int execute_mkdir(char *args[ARGS_MAX]) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: mkdir <directory_name>\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid == 0) {
        execvp("mkdir", args);
        perror("mkdir");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
    }
    return 1;
}