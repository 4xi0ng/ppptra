#include <udis86.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern ud_t ud_obj;

void* disas_num(void* buffer, int buffer_size);
int print_asm(void* buffer, int buffer_size);