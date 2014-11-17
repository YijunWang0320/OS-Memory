#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#define pmd_large(pmd)		(pmd_val(pmd) & 2)
static void walk_pgd(struct task_struct *p,unsigned long start);
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;
	int i;

	p = find_get_pid(pid);
	ts = get_pid_task(p, PIDTYPE_PID);
	if(ts->mm!=NULL) {
		 pgd_t *tmp_pgd = ts->mm->pgd;
		 struct mm_struct *mm = current->mm;
		 struct vm_area_struct *vma = find_vma(mm,fake_pgd);
		int i;
		// i = remap_pfn_range(vma,fake_pgd,tmp_pgd[0][0],PTRS_PER_PGD*sizeof(pgd_t),VM_READ);
		// if(i != 0)
		// 	printk("fail to remap, i = %d \n", i);
		// for(i=0;i<PTRS_PER_PGD;i++) {
		// 	pgd_t *pgd = ts->mm->pgd + i;
		// 	printk("pgd:%lu,*pgd:%lu\n",pgd,*pgd);
		// }
		walk_pgd(ts,addr);
	}
	spin_unlock(&ts->alloc_lock);
	return 10;		
}
static void walk_pte(pmd_t *pmd, unsigned long start)
{
	pte_t *pte = pte_offset_kernel(pmd, 0);
	//printk("pte off set: %lu", pte);
	unsigned long addr;
	unsigned i;
	//printk("in walk pte\n");

	for (i = 0; i < PTRS_PER_PTE; i++, pte++) {
		addr = start + i * PAGE_SIZE;
		//note_page(st, addr, 4, pte_val(*pte));
		printk("pte: %lu,%lu,%lu\n",pte_val(*pte),*pte,pte);
	}
}
static void walk_pmd(pud_t *pud, unsigned long start)
{
	pmd_t *pmd = pmd_offset(pud, 0);
	unsigned long addr;
	unsigned i;
	//printk("in walk pmd\n");

	for (i = 0; i < PTRS_PER_PMD; i++, pmd++) {
		addr = start + i * PMD_SIZE;
		if (pmd_none(*pmd) || pmd_large(*pmd) || !pmd_present(*pmd)) {
			//printk("pmd none:%d,%d,%d *pmd:%lu\n",pmd_none(*pmd),pmd_large(*pmd),pmd_present(*pmd),*pmd);
		//note_page(st, addr, 3, pmd_val(*pmd));
		} else {
			//printk("pte addr: %lu, pte addr: %lu \n", pmd_val(*pmd), *pmd);
			walk_pte(pmd, addr);
		}

	}
}

static void walk_pud(pgd_t *pgd, unsigned long start)
{
	pud_t *pud = pud_offset(pgd, 0);
	//printk("in walk pud\n");
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PUD; i++, pud++) {
		addr = start + i * PUD_SIZE;
		if (!pud_none(*pud)) {
			walk_pmd(pud, addr);
		} else {
			//note_page(st, addr, 2, pud_val(*pud));
			//printk("in pud none\n");
		}
	}
}

static void walk_pgd(struct task_struct *p,unsigned long start)
{
	pgd_t *pgd = p->mm->pgd;
	//printk("in walk pgd\n");
	unsigned long addr;
	unsigned i;
	for (i = 0; i < PTRS_PER_PGD; i++) {
		addr = start + i * PGDIR_SIZE;
		if (!pgd_none(pgd[i])) {
			walk_pud(pgd[i], addr);
			printk("size of pgd[i]: %lu, %lu", sizeof(pgd[i]), sizeof(struct pgd_t));
			//printk("*pgd:%lu,pgd[0][i]:%lu,pgd[1]:%lu,pgd:%lu\n",*pgd,pgd[i][0],pgd[1],pgd);
		} else {
			printk("in pdg none\n");
// 			note_page(&st, addr, 1, pgd_val(*pgd));
		}
	}
}
