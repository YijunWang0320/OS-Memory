#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <asm/pgtable.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	struct pid *p;
	struct task_struct *ts;
	int i;

	p = find_get_pid(pid);
	ts = get_pid_task(p, PIDTYPE_PID);
	if(ts->mm!=NULL) {
		//pgd_t *tmp_pgd = ts->mm->pgd;
		//struct mm_struct *mm = current->mm;
		//struct vm_area_struct *vma = find_vma(mm,fake_pgd);
		//remap_pfn_range(vma,fake_pgd,tmp_pgd,PTRS_PER_PGD*8,VM_READ);
		//walk_pgd(ts->mm->pgd);
		for(i=0;i<PTRS_PER_PGD;i++) {
			pdg_t *pgd = ts->mm->pgd;
			printk("pgd:%lu,*pgd:%lu\n",pgd,*pgd);
		}

	}
	return 10;		
}
// static void walk_pte(pmd_t *pmd, unsigned long start)
// {
// 	pte_t *pte = pte_offset_kernel(pmd, 0);
// 	unsigned long addr;
// 	unsigned i;

// 	for (i = 0; i < PTRS_PER_PTE; i++, pte++) {
// 		addr = start + i * PAGE_SIZE;
// 		//note_page(st, addr, 4, pte_val(*pte));
// 		printk("pte: %lu\n",*pte);
// 	}
// }

// static void walk_pmd(pud_t *pud, unsigned long start)
// {
// 	pmd_t *pmd = pmd_offset(pud, 0);
// 	unsigned long addr;
// 	unsigned i;

// 	for (i = 0; i < PTRS_PER_PMD; i++, pmd++) {
// 		addr = start + i * PMD_SIZE;
// 		if (pmd_none(*pmd) || pmd_large(*pmd) || !pmd_present(*pmd)) {

// 		//note_page(st, addr, 3, pmd_val(*pmd));
// 		} else
// 			walk_pte(pmd, addr);
// 	}
// }

// static void walk_pud(pgd_t *pgd, unsigned long start)
// {
// 	pud_t *pud = pud_offset(pgd, 0);
// 	unsigned long addr;
// 	unsigned i;

// 	for (i = 0; i < PTRS_PER_PUD; i++, pud++) {
// 		addr = start + i * PUD_SIZE;
// 		if (!pud_none(*pud)) {
// 			walk_pmd(pud, addr);
// 		} else {
// 			//note_page(st, addr, 2, pud_val(*pud));
// 			//printk("in pud there is none\n");
// 		}
// 	}
// }

// static void walk_pgd(struct task_struct *p)
// {
// 	pgd_t *pgd = p->mm->pgd;
// 	unsigned long addr;
// 	unsigned i;

// 	memset(&st, 0, sizeof(st));
// 	st.seq = m;
// 	st.marker = address_markers;

// 	for (i = 0;
// 	     i < PTRS_PER_PGD; i++, pgd++) {
// 		addr = i * PGDIR_SIZE;
// 		if (!pgd_none(*pgd)) {
// 			walk_pud(pgd, addr);
// 		} else {
// 			//printk("in pdg none\n");
// // 			note_page(&st, addr, 1, pgd_val(*pgd));
// 		}
// 	}
// }
