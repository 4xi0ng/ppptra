#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

extern void* SB;

int print_ehdr();
int print_shdr();
int print_s_symtab();

void init_readelf(char* filename);
