#include<linux/syscalls.h>
#include<linux/unistd.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;

	p = find_get_pid(pid_t);
	ts = get_pid_task(p, PIDTYPE_PID);
	long pgd = ts->mm->pgd;
	printk("content of pgd: %ld \n", pgd);
	return 10;		
}