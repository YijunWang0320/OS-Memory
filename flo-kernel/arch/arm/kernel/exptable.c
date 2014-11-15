#include<linux/syscalls.h>
#include<linux/unistd.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;

	p = find_get_pid(pid);
	ts = get_pid_task(p, PIDTYPE_PID);
	if(ts->mm!=NULL) {
		unsigned long pgd = ts->mm->pgd[0];
		unsigned long pgd2 = ts->mm->pgd[1];
		printk("content of pgd[0]: %lu, pgd[1]= %lu, task:%s \n", pgd, pgd2,ts->comm);
	}
	return 10;		
}
