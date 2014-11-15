#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;
	int i,j,k,l;

	p = find_get_pid(pid);
	ts = get_pid_task(p, PIDTYPE_PID);
	if(ts->mm!=NULL) {
		for (i = 0; i < PTRS_PER_PGD; ++i)
		{
    		pgd_t *pgd = ts->mm->pgd + i;
    		if (pgd_none(*pgd) || pgd_bad(*pgd))
        		continue; 
		printk("=====================\n");
        	printk("pgd[0]: %lu\n",pgd[0]);
		printk("*pgd[0]: %lu\n",*pgd[0]);
		printk("pgd_index(*pgd[0]): %lu\n",pgd_index(*pgd[0]));
		printk("pte_index(*pgd[0]): %lu\n",pte_index(*pgd[0]));
		}
	}
	return 10;		
}
