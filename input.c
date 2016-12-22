#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  char * str = "0804";
  long l = strtol(str,NULL, 16);
  printf("%lx\n", l);
}
