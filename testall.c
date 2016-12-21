#include "readelf.h"
#include "ppptra.h"
#include <string.h>
#include <stdio.h>

static void invalid(void){
  fprintf(stderr, "Invalid argument! use -h for help~\n", );
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

int main(int argc, char *argv[]) {
  /* code */
  char* filename = argv[1];
  if(strcmp(filename,"-h")==0){
    usage();
    exit(0);
  }
  char c[50];
  char c2[50]="\n";
  char op[3][10];

  init_readelf(filename);
  hang_up(filename);
  pkill();
  //print_ehdr();
  //print_shdr();
  //print_s_symtab();
  //print_s_dynsym();
  //print_s_text();
  while (1) {
    printf(">");
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

    }else if(strcmp("finish", op[0])==0){
      //finish

    }else if(strcmp("set", op[0])==0){
      //set

    }else switch (op[0]) {
      case 'q':
        if(op[1]!=NULL){
          invalid();usage();exit(0);
        }
      case 'b':
        if(op==NULL){
          invalid();usage();exit(1);
        }
        long bpaddr = get_true_addr(op);
        bp(bpaddr);

      case 'c':

      case 's':

      case 'n':

      case 'x':

      case 'r':
    }

    printf("%s\n", c);
  }



    //printf("c is %c\n", c);
    switch(c){
      case 'b':bp(0x0804844d);break;
      case 'c':contn();break;
      case 'n':next_step();break;
      default:next_step();break;
    }
    //fflush(stdin);
  }
  //test(filename);
  return 0;
}
