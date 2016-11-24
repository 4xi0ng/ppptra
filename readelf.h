#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE * f;

void* read2mem(int start_addr, int offset, int size);

int open_file(char * filename);

void* read_ehdr();

int print_ehdr(Elf32_Ehdr * cst_ehdr);

void* read_shdr(Elf32_Ehdr* cst_ehdr);

int print_shdr(Elf32_Shdr * cst_shdr, Elf32_Ehdr* cst_ehdr);