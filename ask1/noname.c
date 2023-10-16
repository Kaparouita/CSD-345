#include "commands.h"
#include "mylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int initFunc(){
    current_dir = (char *)malloc(strlen("") + 1);  
    return 0;
}


char *getcf() {
    return current_dir;
}


int is_valid_command(const char *input, command *cmd) {
    if (strcmp(input, "ls") == 0) {
        *cmd = ls;
        return 1;
    } else if (strcmp(input, "date") == 0) {
        *cmd = date;
        return 1;
    } else if (strcmp(input, "cat") == 0) {
        *cmd = cat;
        return 1;
    } else if (strcmp(input, "rm") == 0) {
        *cmd = rm;
        return 1;
    } else if (strcmp(input, "mkdir") == 0) {
        *cmd = mkdir;
        return 1;
    } else if (strcmp(input, "quit") == 0) {
        *cmd = quit;
        return 1;
    } else if (strcmp(input, "chdir") == 0 || strcmp(input, "cd") == 0) {
        *cmd = cdir;
        return 1;
    }
     else {
        return 0;
    }
}