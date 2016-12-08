#include "all.h"

int main(int argc, char *argv[]) {
  /* code */
  char* filename = argv[1];
  init_readelf(filename);
  print_ehdr();
  print_shdr();
  return 0;
}
