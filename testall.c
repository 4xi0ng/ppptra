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
    " r s                            Display the stack\n"
    " r r                            Display the value of all registers\n"
    " r [register_name]              Display the value of the register\n"
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
      if(op[2]!=NULL){
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

    }else switch (* op[0]) {
      case 'q':
        if(op[1]!=NULL){
          invalid();usage();exit(0);
        }
        exit(0);
      case 'b':
        if(op[2]!=NULL){
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
        contn();break;
      case 's':
        next_step();break;
      case 'n':
        break;
      case 'x':
        if(op[2]!=NULL){
          invalid();usage();exit(0);
        }
        long addr = get_true_bpaddr(op[1]);
        if(addr==0){
          printf("%s\n", "invalid address");
          break;
        }
        long data = ptrace(PTRACE_PEEKTEXT, child_pid, addr, NULL);
        printf("[%lx~%lx]: %lx\n", addr+4, addr, data);
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
          }

        }
      //r -S [.text]

      //r -S all


    }


  }

  return 0;
}
