#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

extern void* SB;

int get_filesize(char* filename);
int malloc_file(char * filename);
Elf32_Ehdr * get_ehdr();
Elf32_Shdr * get_shdr();
int print_ehdr();
char * check_sh_type(Elf32_Word sh_type);
int print_shdr();
void init_readelf(char* filename);
