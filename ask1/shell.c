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
    printf("Welcome to the my shell csd4300!\n");
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



// cd hi | cat > hi.txt | ls


