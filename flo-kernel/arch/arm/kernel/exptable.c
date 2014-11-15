#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/mm_types.h>
#include <asm/page.h>
 44 #include <asm/pgtable.h>
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
    for (j = 0; j < PTRS_PER_PUD; ++j)
    {
        pud_t *pud = (pud_t *)pgd_page_vaddr(*pgd) + j;
        if (pud_none(*pud) || pud_bad(*pud))
            continue;
        for (k = 0; k < PTRS_PER_PMD; ++k)
        {
            pmd_t *pmd = (pmd_t *)pud_page_vaddr(*pud) + k;
            if (pmd_none(*pmd) || pmd_bad(*pmd))
                continue;

            for (l = 0; l < PTRS_PER_PTE; ++l)
            {
                pte_t *pte = (pte_t *)pmd_page_vaddr(*pmd) + l;
                if (!pte || pte_none(*pte))
                    continue;
                struct page *p = pte_page(*pte);
                unsigned long phys = page_to_phys(p);
                printk(KERN_NOTICE "addr %lx", phys);
            }
        }
    }
}
	}
	return 10;		
}
