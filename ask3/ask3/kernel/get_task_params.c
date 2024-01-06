#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/d_params.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm-generic/errno-base.h>

asmlinkage long sys_get_task_params(struct d_params *d_arguments) {
     struct task_struct *task;
     printk("Giorgos Stivaktakis | csd4300  | sys_get_task_params\n");
     if (access_ok(VERIFY_WRITE, d_arguments, sizeof(struct d_params)) == 0) {
          printk("access_ok failed\n");
          return EINVAL;
     }
     
     task = get_current();
	if(!task->group_name || !task->member_id) {
		printk("group or member id is not set\n");
		return EINVAL;
	}
     copy_to_user(&(d_arguments->group_name), &(task->group_name), sizeof(char));
     copy_to_user(&(d_arguments->member_id), &(task->member_id), sizeof(int));
     
     printk("Succed to get task params\n");  
     return 0;
}
