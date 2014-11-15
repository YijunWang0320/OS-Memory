#include<linux/syscalls.h>
#include<linux/unistd.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;
	int i;
	int j;

	p = find_get_pid(pid);
	ts = get_pid_task(p, PIDTYPE_PID);
	if(ts->mm!=NULL) {
		pgd_t *pgd;
		for(i = 0;i<PTRS_PER_PGD;i++) {
			pgd = ts->mm->pgd+i;
			if(pgd_none(*pgd) || pgd_bad(*pgd))
				continue;
			for(j=0;j<PTRS_PER_PTE;j++) {
				pte_t *pte = (pte_t *)pgd_page_vaddr(*pgd)+j;
				if(!pte || pte_none(*pte))
					continue;
				struct page *p = pte_page(*pte);
				unsigned long phys=page_to_phys(p);
				printk("phys addr:%lu \n",phys);
			}		
		}
	}
	return 10;		
}
