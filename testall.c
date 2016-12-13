#include "all.h"
#include "ppptra.h"

int main(int argc, char *argv[]) {
  /* code */
  char* filename = argv[1];
  //init_readelf(filename);
  //print_ehdr();
  //print_shdr();
  //print_s_symtab();
  //print_s_dynsym();
  //print_s_text();
 hang_up(filename);
 bp(0x0804844d);
 contn();
  //test(filename);
  return 0;
}
