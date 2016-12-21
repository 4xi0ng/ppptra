#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  char c[20];
  char c2[20];

  printf("%p\n", c);
  printf("%p\n", c2);

  while (1) {
    printf(">");
    //scanf("%s", c);
    fgets(c, 20, stdin);
    //getchar();
    if(strcmp(c, "\n")==0){
      strcpy(c, c2);
      printf("pres enter\n");
      //c = c2;
    }
    else{
      int l = strlen(c);
      //printf("%d\n", l);
      c[l-1] = '\0';
      strcpy(c2, c);
      //c2 = c;
    }
    /* code */
    printf("%s\n", c);
  }
  return 0;
}
