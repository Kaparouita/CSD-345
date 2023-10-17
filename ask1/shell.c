#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commands.h"
#include "mylib.h"

int loopflag = 1;



int main() {
    initFunc();
    printf("Welcome to the shell!\n");
    while (loopflag) {
        // Read a command from the user
        char input[ARGS_MAX];
        printf("<4300>-hy345sh@<giorgis><%s>", get_current_dir());
        fgets(input, sizeof(input), stdin);
        execute_commands(input);
       
    }
    return 0;
}


