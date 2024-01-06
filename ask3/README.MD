# Assignment 3              |   HY-345
## Stivaktakis Giorgos      |   csd4300  

### Description
In this program we add  2 methods to the linux kernel,get_task_params and set_task_params.
In order to do that we also added 2 fields to the task_struct struct, the group_name and the member_id.
The get_task_params method returns the group_name and the member_id of the task_struct that is given as an argument.
The set_task_params method sets the group_name and the member_id of the task_struct that is given as an argument.


### Steps to implement the program

1. Get the linux kernel source code
2. Add the 2 fields to the task_struct struct in the include/linux/sched.h file
3. Define a system call number for the 2 methods
4. Add the 2 methods to the system call table
5. Add the 2 methods to the syscalls.h file
6. Implement the 2 methods in the kernel/new_methods.c file
7. Add the 2 methods to the Makefile in the kernel directory
8. Compile the kernel
9. Run with qemu
