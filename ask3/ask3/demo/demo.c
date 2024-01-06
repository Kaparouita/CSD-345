#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#define set_task_params(group_name,member_id) syscall(341,group_name,member_id)
#define get_task_params(d_args) syscall(342,d_args)
struct d_params {
    char group_name; // A-Z
	int member_id;  // bigger than 0
};


int main(int argc, char *argv[]) {
     struct d_params *d_args = malloc(sizeof(struct d_params));
     struct d_params *d_wrong_args;
     char char_args = 'A';
     int error;
     //WORKING
     printf("[CORRECT] Setting params to A,1 \n");
     error = set_task_params('A',1);
     if (error == 0){
          printf("Set params success\n");
     }

     //CORRECT GET PARAMS
     printf("[CORRECT] Getting params expecting A,1\n");
     if(error = get_task_params(d_args) == 0) {
          printf("Get params -> Group_name: %c, Member_id: %d\n", \
          d_args->group_name, \
          d_args->member_id);
     }

     //WRONG GET PARAMS
     printf("[ERROR] Getting params expecting error (d_args is not a struct d_params)\n");
     if (error = get_task_params(char_args) != 0) {
          printf("Get params error: %d\n", error);
     }

     //WRONG GROUP NAME
     printf("[ERROR] Setting params to a , 2 expecting error (char must be in range A-Z)\n");
     if ((error = set_task_params('a',2)) != 0) {
          printf("Set params error: %d\n", error);
     }

     //WRONG MEMBER ID
     printf("[ERROR] Setting params to A, -1 expecting error (member_id must be bigger than 0)\n");
     if ((error = set_task_params('A',-1)) != 0) {
          printf("Set params error: %d\n", error);
     }

     //WRONG GROUP NAME NOT CHAR
     printf("[ERROR] Setting params to 1, 1 expecting error (group_name must be a char)\n");
     if ((error = set_task_params(1,1)) != 0) {
          printf("Set params error: %d\n", error);
     }

     wait(NULL);
     return 0;
}

