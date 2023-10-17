#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <libgen.h> 


char * current_dir;

int execute_chdir(char *args[ARGS_MAX]) {
    
    if (chdir(args[1]) != 0) {
        perror("chdir");
        return 1;
    }
    else {
        current_dir = (char *)malloc(PATH_MAX);
        if (getcwd(current_dir, PATH_MAX) != NULL) {
            current_dir= basename(current_dir);
        }
    }
    return 1;
}

char *get_current_dir() {
    return current_dir;
}

int execute_ls(char *args[ARGS_MAX]) {
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

int execute_date(char *args[ARGS_MAX]) {
    execvp("date", args);
    perror("date");  
    return 1;       
}

int execute_cat(char *args[ARGS_MAX]) {
    execvp("cat", args);
    perror("cat");  
    return 1;      
}
int execute_rm(char *args[ARGS_MAX]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp("rm", args);
        perror("rm");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("fork");
    }
    return 1;
}

int execute_quit(char *args[ARGS_MAX]) {
    loopflag = 0;
    return 1;
}


int execute_mkdir(char *args[ARGS_MAX]) {
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
