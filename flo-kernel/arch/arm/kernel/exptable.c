#include<linux/syscalls.h>
#include<linux/unistd.h>
SYSCALL_DEFINE3(expose_page_table,pid_t,pid,unsigned long, fake_pgd,
unsigned long, addr) {
	return 10;		
}
