#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <udis86.h>
#include <string.h>

struct user_regs_struct regs;
int wait_status;
char c;
unsigned char buffer[16];
unsigned char str_asm[256];
ud_t ud_obj;

long trap(pid_t child_pid, long addr, long int3)
{
	long oldchar;
	oldchar = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
	if(ptrace(PTRACE_POKETEXT, child_pid, addr, (oldchar&0xffffff00)|int3) == -1){
		printf("error: poketext failed\n");
		exit(1);
	}
	printf("oldchar = %lx\n", oldchar);
	printf("newchar = %lx\n", ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip, NULL));
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	wait(&wait_status);
	if(!WIFSTOPPED(wait_status)){
		printf("error: wait_status");
		exit(1);
	}
	printf("child got signal:%s\n", strsignal(WSTOPSIG(wait_status)));
	print_regs(child_pid);
	return oldchar; 
}

int recovery_regs(pid_t child_pid, long addr, long oldchar)
{
	ptrace(PTRACE_POKETEXT, child_pid, addr, oldchar);
	regs.eip = addr;
	ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
	print_regs(child_pid);
}

int goon(pid_t child_pid, long addr, long oldchar)
{
	ptrace(PTRACE_POKETEXT, child_pid, addr, oldchar);
	regs.eip = addr;
	ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
	print_regs(child_pid);
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	return 0;
}

int gostep(pid_t child_pid)
{
	ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL);
	wait(&wait_status);
	if(WIFSTOPPED(wait_status)){
		printf("***step***\n");	
		print_regs(child_pid);
	}
	return 0;
}

void init_ud_t(){
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_input_buffer(&ud_obj, buffer, 16);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
}

char* disa()
{
	ud_disassemble(&ud_obj);
	strcpy(str_asm, ud_insn_asm(&ud_obj));
	return str_asm;
}

int show_next_asm(pid_t child_pid, long oldaddr, long oldchar)
{
	int i ;
	long temp;
	memcpy(buffer, &oldchar, 4);
	for(i=1; i<4; i++){
		temp = ptrace(PTRACE_PEEKTEXT, child_pid, oldaddr+i*4, NULL);
		memcpy(buffer, &temp, 4);
	}
	disa();
	printf("str_asm: %s\n", str_asm);
	return 0;
}

int print_regs(pid_t child_pid)
{	
	long eip_v;
	long esp_v;
	ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
	eip_v = ptrace(PTRACE_PEEKTEXT, child_pid, regs.eip, NULL);
	esp_v = ptrace(PTRACE_PEEKTEXT, child_pid, regs.esp, NULL);
	printf("----show_regs\n");
	printf("EIP=%lx  [%lx]=%lx\n", regs.eip, regs.eip, eip_v);
	printf("ESP=%lx  [%lx]=%lx\n", regs.esp, regs.esp, esp_v);
	printf("----show_regs end\n");
	return 0;
}

int main(int argc, char *argv[])
{
	pid_t child_pid;
	pid_t mypid;
	long int3 = 0xcc;
	long backup;

	
	init_ud_t();
	if(argc!=2){
		printf("Usage: %s <execultable file>\n", argv[0]);
		exit(1);
	}
	mypid = getpid();
	printf("mypid=%d\n", mypid);
	child_pid = fork();
	if(child_pid==0){
		execve(argv[1], NULL, NULL);
	}
	else{
		//printf("fork return pid=%d\n", child_pid);
		ptrace(PTRACE_ATTACH, child_pid, NULL, NULL);
		wait(NULL);
		
		print_regs(child_pid);
		long addr = regs.eip;
		backup = trap(child_pid, addr, int3);
		printf("--trap over--\n");
		//sleep(5);
		recovery_regs(child_pid, addr, backup);
		
	
		while(1){
			printf(">");
			scanf("%c", &c);
			getchar();
			printf("c is %c\n", c);
			switch(c){
				case 'n':gostep(child_pid);show_next_asm(child_pid, addr, backup);break;
				default:printf("error: unkonw command\n");exit(1);
			}
			//fflush(stdin);
		}
		gostep(child_pid);

		//sleep(20);
		ptrace(PTRACE_DETACH, child_pid, NULL, NULL);
		
		printf("child_pid=%d\n", child_pid);
	}
	return 0;
}
