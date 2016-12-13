#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <udis86.h>
#include <string.h>


int child_pid;
long oldchar[100];
long bp_addr[100];
int bp_ndx = -1;
struct user_regs_struct regs;
int wait_status;

int test(char* filename)
{
	child_pid = fork();
	if(child_pid==0){
		/*if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");
        return;
    }*/
		execl(filename, filename, 0);
	}
	else{
		//printf("fork return pid=%d\n", child_pid);
		ptrace(PTRACE_ATTACH, child_pid, NULL, NULL);
		while (1) {
			wait(&wait_status);
			sleep(3);
			printf("child got signal:%s\n", strsignal(WSTOPSIG(wait_status)));
			ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
			printf("eip at %lx\n", regs.eip);
			printf("[0x08048350] is %lx\n\n", ptrace(PTRACE_PEEKTEXT, child_pid, 0x08048350, NULL));
			ptrace(PTRACE_CONT, child_pid, NULL, NULL);
		}
	}
	return 0;
}

int hang_up(char* filename)
{
	child_pid = fork();
	if(child_pid==0){
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace");
        return;
    }
		execl(filename, filename, 0);
	}
	else{
		wait(NULL);
	}
	return child_pid;
}

int bp(long addr)
{
	bp_ndx++;
	//save oldchar and bp_addr
	oldchar[bp_ndx] = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
	bp_addr[bp_ndx] = addr;

	//replace oldchar with int3(0xcc)
	if(ptrace(PTRACE_POKETEXT, child_pid, addr, (oldchar[bp_ndx]&0xffffff00)|0xcc) == -1){
		printf("error: poketext failed\n");
		exit(1);
	}

	return bp_ndx;
}

int contn()
{
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

	printf("now eip is %lx\n", regs.eip);
	printf("0x0804844d value is %lx\n\n", ptrace(PTRACE_PEEKTEXT, child_pid, 0x0804844d, NULL));
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	wait(&wait_status);


	if(!WIFSTOPPED(wait_status)){
		printf("error: wait_status");
		exit(1);
	}
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	printf("child got signal:%s\n", strsignal(WSTOPSIG(wait_status)));
	printf("breakpoint at %lx\n", regs.eip);
	printf("0x0804844d value is %lx\n\n", ptrace(PTRACE_PEEKTEXT, child_pid, 0x0804844d, NULL));
	return 0;
}
