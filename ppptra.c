#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <udis86.h>
#include <string.h>
#include "disas.h"


int child_pid;
long oldchar[100];
long bp_addr[100];
int bp_ndx = -1;
struct user_regs_struct regs;
int wait_status;

int hang_up(char* filename)
{
	child_pid = fork();
	if(child_pid==0){
		if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
        perror("ptrace failed");
        exit(1);
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


int handle_is_bp()
{
	//this method will check eip is bp or not, if true, it will resume eip and opcode
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	for(int i=0; i<100; i++){
		if(regs.eip-1 == bp_addr[i]){
			printf("\033[34m@breakpoint %d\033[0m", i+1);

			regs.eip = bp_addr[i];
			ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
			ptrace(PTRACE_POKETEXT, child_pid, bp_addr[i], oldchar[i]);
			ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
			long buffer[4];
			buffer[0] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip, NULL);
			buffer[1] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+4, NULL);
			buffer[2] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+8, NULL);
			buffer[3] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+12, NULL);

			printf("oldchar%lx\n", oldchar[i]);
			printf("addr%lx\n", bp_addr[i]);
			printf("now%lx\n", ptrace(PTRACE_PEEKTEXT, child_pid, bp_addr[i], 0));
			//print_asm((void*)regs.eip, 16, (void*)regs.eip);
			print_asm((void*)buffer, 16, (void*)regs.eip);
			return 1;
		}
	}
	return 0;
}

int contn()
{
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	wait(&wait_status);
	if(!WIFSTOPPED(wait_status)){
		printf("error: wait_status");
		exit(1);
	}
	handle_is_bp();

	return 0;
}

int next_step()
{
	ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL);
	wait(NULL);
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	long buffer[6];
	buffer[0] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip, NULL);
	buffer[1] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+4, NULL);
	buffer[2] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+8, NULL);
	buffer[3] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+12, NULL);
	buffer[4] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+16, NULL);
	buffer[5] = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip+20, NULL);
	print_asm((void*)buffer, 24, (void*)regs.eip);
	return 0;
}
