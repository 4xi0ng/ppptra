#include "readelf.h"
#include "ppptra.h"
#include "disas.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void invalid(void){
  fprintf(stderr, "Invalid argument! use -h for help~\n");
}

static void usage(void)
{
  fprintf(stderr,
    "ppptra usage:\n"
    " ./ppptra executable-file [args]\n"
    "how to debug:\n"
    " r -E                           Display the ELF file header\n"
    " r -S                           Display the sections' header\n"
    " r -S all                       Display all sections details\n"
    " r -S [sectionName]             Display the section details\n"
    " r -P                           Display the program headers\n"
    " r s                            Display the stack 10 lines\n"
    " r s [num]                      Display the stack num lines\n"
    " r r                            Display the value of all registers\n"
    " x @[address]                   Display the value of the address\n"
    " disas [funcName or @address]   Disassemble the code\n"
    " b [funcName or @address]       Set a breakpoint at here\n"
    " c                              Continue running\n"
    " s                              Step into\n"
    " n                              Single step\n"
    " finish                         Exit this function\n"
    " set [register_name] [value]    Modify the value of the register\n"
    " set @[address] [value]         Modify the value of the address\n");
}

long get_true_bpaddr(char * str)
{
  long addr;
  if(*str == '@'){
    addr = strtol(str+1, NULL, 16);
    return addr;
  }
  return (long)get_addr_bysym(str);
}

int print_s_got()
{
  printf("\n<SECTION .got>\n");
  if(get_shdr_byname(".got")!=NULL){
    Elf32_Shdr* shdr = get_shdr_byname(".got");
    void* addr = (void*)shdr->sh_addr;
    int num = shdr->sh_size/4;

    for(int i=0;i<num;i++){
      long data = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
      printf("GOT[%d] 0x%08lx:    0x%08lx\n", i, (long)addr, data);
      addr += 4;
    }
  }
  if(get_shdr_byname(".got.plt")!=NULL){
    Elf32_Shdr* shdr1 = get_shdr_byname(".got.plt");
    void* addr1 = (void*)shdr1->sh_addr;
    int num1 = shdr1->sh_size/4;

    for(int i=0;i<num1;i++){
      long data1 = ptrace(PTRACE_PEEKTEXT, child_pid, addr1, NULL);
      printf("GOT.PLT[%d] 0x%08lx:    0x%08lx\n", i, (long)addr1, data1);
      addr1 += 4;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  /* code */
  char* filename = argv[1];
  if(strcmp(filename,"-h")==0){
    usage();
    exit(0);
  }
  char c[50];
  char c2[50]="\n";
  char* op[3];

  init_readelf(filename);
  hang_up(filename);
  pkill();
  //print_ehdr();
  //print_shdr();
  //print_s_symtab();
  //print_s_dynsym();
  //print_s_text();
  while (1) {
    printf("\n\033[34m>\033[0m");
    fgets(c, 50, stdin);
    if(strcmp(c, "\n")==0){
      strcpy(c, c2);
    }
    else{
      int l = strlen(c);
      c[l-1] = '\0';
      strcpy(c2, c);
    }

    op[0] = strtok(c, " ");
    op[1] = strtok(NULL, " ");
    op[2] = strtok(NULL, " ");

    if(strcmp("disas", op[0])==0){
      //disas
      if(op[2]!=NULL || op[1]==NULL){
        invalid();usage();exit(0);
      }
      long bpaddr = get_true_bpaddr(op[1]);
      if(bpaddr==0){
        printf("%s\n", "invalid address");
      }else{
        print_all_func((void *)bpaddr);
      }
    }else if(strcmp("finish", op[0])==0){
      //finish

    }else if(strcmp("set", op[0])==0){
      //set
      if(op[2]==NULL || op[1]==NULL){
        invalid();usage();exit(0);
      }
      if(*op[1] == '@'){
        long addr = strtoul(op[1]+1, NULL, 16);
        long val = strtoul(op[2], NULL, 16);
        if(ptrace(PTRACE_POKETEXT, child_pid, addr, val)==-1){
          printf("set value FAILED\n");
        }else{
          printf("%lx_%lx\n", addr, val);
          printf("set value SUCCESS\n");
        }
      }else{
        ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
        if(strcmp(op[1],"eip")==0){
          regs.eip = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"esp")==0){
          regs.esp = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"eax")==0){
          regs.eax = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"ebx")==0){
          regs.ebx = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"ecx")==0){
          regs.ecx = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"edx")==0){
          regs.edx = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"esi")==0){
          regs.esi = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"edi")==0){
          regs.edi = strtol(op[2], NULL, 16);
        }else if(strcmp(op[1],"ebp")==0){
          regs.ebp = strtol(op[2], NULL, 16);
        }else{
          printf("set FAILED\n");
        }
        ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);
      }

    }else switch (* op[0]) {
      case 'q':
        if(op[1]!=NULL){
          invalid();usage();exit(0);
        }
        exit(0);
      case 'b':
        if(op[2]!=NULL || op[1]==NULL){
          invalid();usage();exit(0);
        }
        long bpaddr = get_true_bpaddr(op[1]);
        if(bpaddr==0){
          printf("%s\n", "invalid breakpoint");
          break;
        }
        bp(bpaddr);
        break;
      case 'c':
        if(op[1]!=NULL){
          invalid();usage();exit(0);
        }
        contn();break;
      case 's':
        if(op[1]!=NULL){
          invalid();usage();exit(0);
        }
        next_step();break;
      case 'n':
        break;
      case 'x':
        if(op[2]!=NULL || op[1]==NULL){
          invalid();usage();exit(0);
        }
        long addr = get_true_bpaddr(op[1]);
        if(addr==0){
          printf("%s\n", "invalid address");
          break;
        }
        long data = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
        printf("[%lx~%lx]: %08lx\n", addr+4, addr, data);
        break;
      case 'r':
      //r -E
        if(strcmp(op[1], "-E")==0 && op[2]==NULL){
          print_ehdr();
          break;
        }
      //r -S
        else if(strcmp(op[1], "-S")==0){
          if(op[2]==NULL){
            print_shdr();
            break;
          }else if(strcmp(op[2],".text")==0){
            print_s_text();
            break;
          }else if(strcmp(op[2],".symtab")==0){
            print_s_symtab();
            break;
          }else if(strcmp(op[2],".dynsym")==0){
            print_s_dynsym();
            break;
          }else if(strcmp(op[2],".plt")==0){
            print_s_plt();
            break;
          }else if(strcmp(op[2],".got")==0){
            print_s_got();
            break;
          }else if(strcmp(op[2],".interp")==0){
            print_s_interp();
            break;
          }else if(strcmp(op[2],".rel")==0){
            print_s_rel();
            break;
          }else if(strcmp(op[2],".dynamic")==0){
            print_s_dynamic();
            break;
          }else if(strcmp(op[2],"all")==0){
            print_s_text();
            print_s_symtab();
            print_s_dynsym();
            print_s_plt();
            print_s_got();
            print_s_interp();
            print_s_rel();
            print_s_dynamic();
            break;
          }
        }
      //r -P
        else if(strcmp(op[1], "-P")==0 && op[2]==NULL){

        }
      //r r
        else if(strcmp(op[1], "r")==0 && op[2]==NULL){
          ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
          printf("eip : %08lx\n", regs.eip);
          printf("eax : %08lx\n", regs.eax);
          printf("ebx : %08lx\n", regs.ebx);
          printf("ecx : %08lx\n", regs.ecx);
          printf("edx : %08lx\n", regs.edx);
          printf("esi : %08lx\n", regs.esi);
          printf("edi : %08lx\n", regs.edi);
          printf("esp : %08lx\n", regs.esp);
          printf("ebp : %08lx\n", regs.ebp);
        }
        else if(strcmp(op[1], "s")==0 && op[2]==NULL){
          ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
          for(int i=0;i<10;i++){
            printf("0x%08lx = %08lx\n", regs.esp+i*4, ptrace(PTRACE_PEEKTEXT, child_pid, regs.esp+i*4, NULL));
          }
        }
        else if(strcmp(op[1], "s")==0 && op[2]!=NULL){
          int num = strtol(op[2], NULL, 10);
          ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
          for(int i=0;i<num;i++){
            printf("0x%08lx = %08lx\n", regs.esp+i*4, ptrace(PTRACE_PEEKTEXT, child_pid, regs.esp+i*4, NULL));
          }
        }

        break;
      //no such a command
      default:printf("no such a command\n");
    }
  }

  return 0;
}
