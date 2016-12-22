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

int hang_up(char* filename);
int handle_is_bp();
int bp(long addr);
int contn();
int next_step();
void pkill();
