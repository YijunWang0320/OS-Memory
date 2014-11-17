#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#define pmd_large(pmd)		(pmd_val(pmd) & 2)
#define PGD_ENTRY_SIZE 4
#define PTE_ENTRY_SIZE 4
static void walk_pgd(struct task_struct *p);
static unsigned long cur_pgd;
static unsigned long cur_addr;
static struct task_struct *ts;
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	int i;

	cur_pgd = fake_pgd;
	cur_addr = addr;
	p = find_get_pid(pid);
	if(pid == NULL) {
		return -EINVAL;
	}
	ts = get_pid_task(p, PIDTYPE_PID);
	spin_lock(&ts->alloc_lock);
	if(ts->mm!=NULL) {
		//remap_pfn_range(vma,fake_pgd,tmp_pgd[0][0],PTRS_PER_PGD*sizeof(pgd_t),VM_READ);
		walk_pgd(ts);
	}
	spin_unlock(&ts->alloc_lock);
	return 10;		
}
static void walk_pte(pmd_t *pmd)
{
	pte_t *pte = pte_offset_kernel(pmd, 0);
	unsigned i;
	// for (i = 0; i < PTRS_PER_PTE; i++, pte++) {
	// 	//addr = start + i * PAGE_SIZE;
	// 	//printk("pte: %lu,%lu,%lu\n",pte_val(*pte),*pte,pte);
	// }
	struct mm_struct *mm = ts->mm;
	struct vm_area_struct *vma = find_vma(mm,addr); 
	//*cur_addr = pte;
	remap_pfn_range(vma,cur_addr,pte,PTRS_PER_PTE*PTE_ENTRY_SIZE,VM_READ);
	*fake_pgd = cur_addr;
	cur_addr = cur_addr + PTRS_PER_PTE*PTE_ENTRY_SIZE;
	fake_pge = fake_pgd + PGD_ENTRY_SIZE;
}
static void walk_pmd(pud_t *pud)
{
	pmd_t *pmd = pmd_offset(pud, 0);
	unsigned i;
	//printk("in walk pmd\n");

	for (i = 0; i < PTRS_PER_PMD; i++, pmd++) {
		//addr = start + i * PMD_SIZE;
		if (pmd_none(*pmd) || pmd_large(*pmd) || !pmd_present(*pmd)) {
		//printk("pmd none:%d,%d,%d *pmd:%lu\n",pmd_none(*pmd),pmd_large(*pmd),pmd_present(*pmd),*pmd);
		//note_page(st, addr, 3, pmd_val(*pmd));
		} else {
			//printk("pte addr: %lu, pte addr: %lu \n", pmd_val(*pmd), *pmd);
			walk_pte(pmd);
		}

	}
}

static void walk_pud(pgd_t *pgd)
{
	pud_t *pud = pud_offset(pgd, 0);
	unsigned i;

	for (i = 0; i < PTRS_PER_PUD; i++, pud++) {
		//addr = start + i * PUD_SIZE;
		if (!pud_none(*pud)) {
			walk_pmd(pud);
		} else {
			//note_page(st, addr, 2, pud_val(*pud));
			//printk("in pud none\n");
		}
	}
}

static void walk_pgd(struct task_struct *p)
{
	pgd_t *pgd = p->mm->pgd;
	unsigned i;
	for (i = 0; i < PTRS_PER_PGD; i++) {
		//addr = start + i * PGDIR_SIZE;
		if (!pgd_none(pgd[i][0])) {
			if(pgd[i][0] != 0)
				walk_pud(pgd[i]);
			//printk("size of pgd[i]: %d\n", sizeof(pgd[i]));
		} else {
			printk("in pdg none\n");
// 			note_page(&st, addr, 1, pgd_val(*pgd));
		}
	}
}
