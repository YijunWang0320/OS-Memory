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
    				if(pmd_none(*pmd) || pmd_bad(*pmd))
    					continue;
    				
    				for(l = 0; l < PTRS_PER_PTE; l++)
    				{
    					pte_t *pte = pmd + l * PAGE_SIZE;
    					printk("[index]\t[virt]\t[phys]\t[young bit]\t[file bit]\t[dirty bit]\t[read-only bit]\t[xn bit]\n");
    					printk("%lu\t%X\t%X\t%lu\t%lu\t%lu\t%lu\t%lu\t\n", pte_index(*pte), 0, *pte, pte_young(*pte),pte_file(*pte),pte_dirty(*pte), pte_write(*pte), pte_exec(*pte));
    				}
    			}

    		}
		}
	}
	return 10;		
}
