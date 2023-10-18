#include "commands.h"
#include "mylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <libgen.h>

int initFunc()
{
    current_dir = (char *)malloc(PATH_MAX);
    if (getcwd(current_dir, PATH_MAX) != NULL)
    {
        current_dir = basename(current_dir);
    }
    return 0;
}

int is_valid_command(const char *input, cmd_name *cmd)
{
    if (strcmp(input, "quit") == 0)
    {
        *cmd = quit;
        return 1;
    }
    else if (strcmp(input, "chdir") == 0 || strcmp(input, "cd") == 0)
    {
        *cmd = cdir;
        return 1;
    }
    else
    {
        return 0;
    }
}