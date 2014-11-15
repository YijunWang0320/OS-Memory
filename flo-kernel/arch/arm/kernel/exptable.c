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
    		
    		for(j = 0; j < PTRS_PER_PUD;++j)
    		{
    			pud_t *pud = pgd + j * PUD_SIZE;
    			if(pud_none(*pud) || pud_bad(*pud))
    				continue;
    			
    			for(k = 0; k < PTRS_PER_PMD; ++k)
    			{
    				pmd_t *pmd = pud + k * PMD_SIZE;
    				if(pmd_none(pmd) || pmd_bad(pmd))
    					continue;
    				if(l = 0; l < PTRS_PER_PTE; l++)
    				{
    					pte_t *pte = pmd + l * PAGE_SIZE;
    					printk("pte val: %lu", pte_val(*pte));
    					printk("pte index: %lu", pte_index(*pte));
    				}
    			}

    		}
	
		printk("=====================\n");
        	printk("pgd[0]: %lu\n",pgd[0]);
		printk("*pgd[0]: %lu\n",*pgd[0]);
		printk("pgd_index(*pgd[0]): %lu\n",pgd_index(*pgd[0]));
		printk("pte_index(*pgd[0]): %lu\n",pte_index(*pgd[0]));
		}
	}
	return 10;		
}
