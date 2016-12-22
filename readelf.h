#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void* SB;
int symarr_flag;
void* sym_arr[1000];
char* sym_namearr[1000];
int sym_sizearr[1000];

int print_ehdr();
int print_shdr();
int print_s_symtab();
int print_s_dynsym();
int print_s_text();
char* get_sym_byaddr(void* addr);
void* get_addr_bysym(char* fname);
int print_all_func(void* addr);

void init_readelf(char* filename);
