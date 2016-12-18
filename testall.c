#include "readelf.h"
#include "ppptra.h"

int main(int argc, char *argv[]) {
  /* code */
  char* filename = argv[1];
  init_readelf(filename);
  //print_ehdr();
  //print_shdr();
  //print_s_symtab();
  //print_s_dynsym();
  //print_s_text();
  char c;
  hang_up(filename);
  while(1){
    printf(">");
    scanf("%c", &c);
    getchar();
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
