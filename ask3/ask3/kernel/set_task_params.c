#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <asm-generic/errno-base.h>
#include <linux/types.h>
#include <asm/current.h>
#include <linux/sched.h>

asmlinkage long sys_set_task_params(char group_name, int member_id) {
    printk("Giorgos Stivaktakis | csd4300 | sys_set_task_params\n");
    struct task_struct *task;
    
    if (member_id <= 0 || group_name < 'A' || group_name > 'Z') {
        printk("Wrong params\n");
        return EINVAL;
    }

    task = get_current();
    task->group_name = group_name;
    task->member_id = member_id;
        
    printk("Succed to set task params\n");
    return 0;
}
