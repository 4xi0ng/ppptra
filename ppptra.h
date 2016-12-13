#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <udis86.h>
#include <string.h>

extern int child_pid;
extern long oldchar[100];
extern long bp_addr[100];
extern int bp_ndx;
extern struct user_regs_struct regs;
extern int wait_status;

int test(char* filename);
int attach(char* filename);
int bp(long addr);
int contn();
