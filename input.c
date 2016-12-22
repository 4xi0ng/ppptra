#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int arr[10];
void init(){
  arr[0]=5;
}

void print() {
  /* code */
  printf("%d\n", arr[0]);
}

int main(int argc, char const *argv[]) {
  init();
  print();
  return 0;
}
