#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "disas.h"

void* SB;
int symarr_flag = 0;
void* sym_arr[1000];
char* sym_namearr[1000];
int sym_sizearr[1000];


int get_filesize(char* filename)
{
  struct stat statbuf;
  stat(filename, &statbuf);
  int size = statbuf.st_size;

  return size;
}

int malloc_file(char * filename)
{
  void* mem_pt;
  FILE* f;
  int size;

	f = fopen(filename, "r");
	if(f == NULL){
		perror("Open file failed");
		exit(1);
	}
  size = get_filesize(filename);
  mem_pt = malloc(size);
  if(fread(mem_pt, 1, size, f) != size){
		perror("Read file failed");
		exit(1);
	}
  long elfheader = 0x464c457f;
	if(memcmp(&elfheader, mem_pt, 4) != 0){
		perror("Not ELF!!");
		exit(1);
	}
  SB = mem_pt;

	return 1;
}


//get series
Elf32_Ehdr * get_ehdr()
{
  void* sb = SB;
  Elf32_Ehdr * ehdr = (Elf32_Ehdr *)sb;

	return ehdr;
}

Elf32_Shdr * get_shdr()
{
  void* sb = SB;
	Elf32_Ehdr * ehdr = get_ehdr();

	Elf32_Shdr * shdr = (Elf32_Shdr *)(sb+(int)ehdr->e_shoff);

	return shdr;
}

char* get_s_shstrtab()
{
  void* sb = SB;
	Elf32_Ehdr* ehdr = get_ehdr();
	Elf32_Shdr* shdr = get_shdr();

	for (int i=0; i<ehdr->e_shstrndx; i++){
		shdr++;
	}
	int addr = shdr->sh_addr+shdr->sh_offset;
	char* s_shstrtab = (char *)(sb+addr);

  return s_shstrtab;
}

void* get_by_sname(char* sname)
{
  void* sb = SB;
	Elf32_Shdr* shdr = get_shdr();
	Elf32_Ehdr* ehdr = get_ehdr();
	char* s_shstrtab = get_s_shstrtab();

	for (int j=0; j<ehdr->e_shnum; j++){
		if(strcmp(s_shstrtab+shdr->sh_name, sname)==0){
      return (sb + shdr->sh_offset);
    }
    shdr++;
	}

  return NULL;
}

Elf32_Shdr * get_shdr_byname(char* name)
{
	Elf32_Shdr* shdr = get_shdr();
	Elf32_Ehdr* ehdr = get_ehdr();
	char* s_shstrtab = get_s_shstrtab();

	for (int j=0; j<ehdr->e_shnum; j++){
		if(strcmp(s_shstrtab+shdr->sh_name, name)==0){
      return shdr;
    }
    shdr++;
	}

  return NULL;
}

int get_num_sym(char* name)
{
  void* sb = SB;
	Elf32_Shdr* shdr = get_shdr();
	Elf32_Ehdr* ehdr = get_ehdr();
	char* s_shstrtab = get_s_shstrtab();

	for (int j=0; j<ehdr->e_shnum; j++){
		if(strcmp(s_shstrtab+shdr->sh_name, name)==0){
      //printf("%d\n", shdr->sh_size);
      return shdr->sh_size/16;
    }
    shdr++;
	}

  return 0;
}

//char* get_s_strtab

//print series
int print_ehdr()
{
	Elf32_Ehdr * ehdr = get_ehdr();
	printf("\n<--ELF HEADER-->\n");

	//Magic
	printf("%-20s", "    Magic: ");
	for (int i = 0; i < 16; ++i)
	{
		printf("%02x ", ehdr->e_ident[i]);
	}
	printf("\n");

	//Class
	printf("%-20s", "    Class: ");
	switch(ehdr->e_ident[4]){
		case ELFCLASSNONE: printf("NONE\n");break;
		case ELFCLASS32: printf("32\n");break;
		case ELFCLASS64: printf("64\n");break;
		case ELFCLASSNUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Data
	printf("%-20s", "    Data: ");
	switch(ehdr->e_ident[5]){
		case ELFDATANONE: printf("NONE\n");break;
		case ELFDATA2LSB: printf("little endian\n");break;
		case ELFDATA2MSB: printf("big endian\n");break;
		case ELFDATANUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Version
	printf("%-20s", "    Version: ");
	switch(ehdr->e_ident[6]){
		case EV_NONE: printf("NONE\n");break;
		case EV_CURRENT: printf("1 current version\n");break;
		case EV_NUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Type
	printf("%-20s", "    Type: ");
	switch(ehdr->e_type){
		case ET_NONE: printf("NONE\n");break;
		case ET_REL: printf("Relocatable file\n");break;
		case ET_EXEC: printf("Executable file\n");break;
		case ET_DYN: printf("Shared object file\n");break;
		case ET_CORE: printf("Core file\n");break;
		case ET_NUM: printf("Number of defined types\n");break;
		case ET_LOOS: printf("OS-specific range start\n");break;
		case ET_HIOS: printf("OS-specific raneg end\n");break;
		case ET_LOPROC: printf("Processor-specific range start\n");break;
		case ET_HIPROC: printf("Processor-specific range end\n");break;
		default: printf("\n");break;
	}

	//Machine
	printf("%-20s", "    Machine: ");
	switch(ehdr->e_machine){
		case EM_NONE: printf("NONE\n");break;
		case EM_M32: printf("AT&T WE 32100\n");break;
		case EM_386: printf("Intel 80386 \n");break;
		case EM_860: printf("Intel 80860\n");break;
		case EM_MIPS: printf("MIPS R3000 big-endian\n");break;
		case EM_MIPS_RS3_LE: printf("MIPS R3000 little-endian\n");break;
		case EM_ARM: printf("ARM\n");break;
		case EM_X86_64: printf("AMD x86-64 architecture\n");break;
		case EM_AARCH64: printf("ARM AARCH64\n");break;
		default: printf("\n");break;
	}

	//Version
	printf("%-20s", "    Version: ");
	switch(ehdr->e_version){
		case EV_NONE: printf("NONE\n");break;
		case EV_CURRENT: printf("1 current version\n");break;
		case EV_NUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Entry point address
	printf("%-20s0x%x\n", "    Entry: ", ehdr->e_entry);

	/* Program header table file offset */
	printf("%-20s0x%x\n", "    PH offset: ", ehdr->e_phoff);

	/* Section header table file offset */
	printf("%-20s0x%x\n", "    SH offset: ", ehdr->e_shoff);

	/* Processor-specific flags */
	printf("%-20s0x%x\n", "    Flags: ", ehdr->e_flags);

	//ELF header size
	printf("%-20s0x%x\n", "    EH size: ", ehdr->e_ehsize);

	//Program header table entry size
	printf("%-20s0x%x\n", "    PH entsize: ", ehdr->e_phentsize);

	/* Program header table entry count */
	printf("%-20s0x%x\n", "    PH entnum: ", ehdr->e_phnum);

	/* Section header table entry size */
	printf("%-20s0x%x\n", "    SH entsize: ", ehdr->e_shentsize);

	/* Section header table entry count */
	printf("%-20s0x%x\n", "    SH entnum: ", ehdr->e_shnum);

	/* Section header string table index */
	printf("%-20s0x%x\n", "    SH shstrndx: ", ehdr->e_shstrndx);

	return 0;
}

char * check_sh_type(Elf32_Word sh_type)
{
	switch(sh_type){
		case SHT_NULL: return "NULL";
		case SHT_PROGBITS: return "PROGBITS";
		case SHT_NOTE: return "NOTE";
		case SHT_GNU_HASH: return "GNU_HASH";
		case SHT_DYNSYM: return "DYNSYM";
		case SHT_STRTAB: return "STRTAB";
		case SHT_GNU_versym: return "VERSYM";
		case SHT_GNU_verneed: return "VERNEED";
		case SHT_REL: return "REL";
		case SHT_INIT_ARRAY: return "INIT_ARRAY";
		case SHT_FINI_ARRAY: return "FINI_ARRAY";
		case SHT_DYNAMIC: return "DYNAMIC";
		case SHT_NOBITS: return "NOBITS";
		case SHT_SYMTAB: return "SYMTAB";
		default: return "unknow~";
	}
}

int print_shdr()
{
	Elf32_Ehdr* ehdr = get_ehdr();
	Elf32_Shdr* shdr = get_shdr();
	char* s_shstrtab = get_s_shstrtab();

	printf("\n<--SECTION HEADER-->\n");
	printf("    [Nr]%-23s%-12s%-9s%-7s%-7s%-3s%-3s%-3s%-3s%-3s\n", "Name", "Type","Addr","Off", "Size","ES","Fg","Lk","If","Al");

	for (int j=0; j<ehdr->e_shnum; j++){
		printf("    [%02d]%-23s", j, s_shstrtab+shdr->sh_name);
		printf("%-12s", check_sh_type(shdr->sh_type));
		printf("%08x ", shdr->sh_addr);
		printf("%06x ", shdr->sh_offset);
		printf("%06x ", shdr->sh_size);
		printf("%02x ", shdr->sh_entsize);
		printf("%02x ", shdr->sh_flags);
		printf("%02x ", shdr->sh_link);
		printf("%02x ", shdr->sh_info);
		printf("%02x", shdr->sh_addralign);
		printf("\n");
		shdr++;
	}
}

char* check_st_type(unsigned char type)
{
  switch (type) {
    case STT_NOTYPE:return "NOTYPE";
    case STT_OBJECT:return "OBJECT";
    case STT_FUNC:return "FUNC";
    case STT_SECTION:return "SECTION";
    case STT_FILE:return "FILE";
    case STT_COMMON:return "COMMON";
    case STT_TLS:return "TLS";
    case STT_NUM:return "NUM";
    case STT_LOOS:return "LOOS";
    //case STT_GNU_IFUNC:return "IFUNC";
    case STT_HIOS:return "HIOS";
    case STT_LOPROC:return "LOPROC";
    case STT_HIPROC:return "HIPROC";
  }
}

char* check_st_bind(unsigned char bind)
{
  switch (bind) {
    case STB_LOCAL:return "Local";
    case STB_GLOBAL:return "Global";
    case STB_WEAK:return "Weak";
    case STB_NUM:return "Number";
    case STB_LOOS:return "LOOS";
    //case STB_GNU_UNIQUE:return "Unique";
    case STB_HIOS:return "HIOS";
    case STB_LOPROC:return "LOPROC";
    case STB_HIPROC:return "HIPROC";
  }
}

char* check_st_vis(unsigned char vis)
{
  switch (vis) {
    case STV_DEFAULT:return "DEFAULT";
    case STV_INTERNAL:return "INTERNAL";
    case STV_HIDDEN:return "HIDDEN";
    case STV_PROTECTED:return "PROTECTED";
  }
}

int print_s_symtab()
{
  void* tmp = get_by_sname(".symtab");
  //printf("%p\n", tmp);
  int num = get_num_sym(".symtab");
  //printf("%d\n", num);
  char* s_strtab = (char*)get_by_sname(".strtab");

  if(tmp == NULL){
    return 0;
  }
  Elf32_Sym* sym = (Elf32_Sym *)tmp;
  printf("%s", "\n<SECTION .symtab>\n");
  printf("    [Num]  %-10s%-6s%-10s%-8s%-9s%-6s%-3s\n", "Value", "Size","Type","Bind", "Vis","Ndx","Name");
  for (int i = 0; i < num; i++) {
    printf("    %-5d  ", i);
    printf("%08x  ", sym->st_value);
    printf("%04x  ", sym->st_size);
    printf("%-10s", check_st_type(ELF32_ST_TYPE(sym->st_info)));
    printf("%-8s", check_st_bind(ELF32_ST_BIND(sym->st_info)));
    printf("%-9s", check_st_vis(sym->st_other));
    printf("%04x  ", sym->st_shndx);
    printf("%s\n", s_strtab+sym->st_name);
    sym++;
  }

  return 0;
}

int print_s_dynsym()
{
  void* tmp = get_by_sname(".dynsym");
  //printf("%p\n", tmp);
  int num = get_num_sym(".dynsym");
  //printf("%d\n", num);
  char* s_strtab = (char*)get_by_sname(".dynstr");

  if(tmp == NULL){
    return 0;
  }
  Elf32_Sym* sym = (Elf32_Sym *)tmp;
  printf("%s", "\n<SECTION .dynsym>\n");
  printf("    [Num]  %-10s%-6s%-10s%-8s%-9s%-6s%-3s\n", "Value", "Size","Type","Bind", "Vis","Ndx","Name");
  for (int i = 0; i < num; i++) {
    printf("    %-5d  ", i);
    printf("%08x  ", sym->st_value);
    printf("%04x  ", sym->st_size);
    printf("%-10s", check_st_type(ELF32_ST_TYPE(sym->st_info)));
    printf("%-8s", check_st_bind(ELF32_ST_BIND(sym->st_info)));
    printf("%-9s", check_st_vis(sym->st_other));
    printf("%04x  ", sym->st_shndx);
    printf("%s\n", s_strtab+sym->st_name);
    sym++;
  }

  return 0;
}

char* get_sym_byaddr(void* addr)
{
  int symtab_num = get_num_sym(".symtab");
  int dynsym_num = get_num_sym(".dynsym");
  int num = symtab_num + dynsym_num;

  if(symarr_flag == 0){

    if(symtab_num != 0){
      Elf32_Sym* sym = (Elf32_Sym *)get_by_sname(".symtab");
      char* s_strtab = (char*)get_by_sname(".strtab");
      for (int i = 0; i < symtab_num; i++) {
        if(ELF32_ST_TYPE(sym->st_info) == STT_FUNC){
          sym_arr[i] = (void*)sym->st_value;
          sym_namearr[i] = s_strtab+sym->st_name;
          sym_sizearr[i] = sym->st_size;
        }
        //printf("okok%s\n", sym_namearr[i]);
        sym++;
      }
    }

    if(dynsym_num != 0){
      Elf32_Sym* dyn = (Elf32_Sym *)get_by_sname(".dynsym");
      char* s_dynstr = (char*)get_by_sname(".dynstr");
      for (int j = 0; j < dynsym_num; j++) {
        if(ELF32_ST_TYPE(dyn->st_info) == STT_FUNC){
          sym_arr[symtab_num+j] = (void*)dyn->st_value;
          sym_namearr[symtab_num+j] = s_dynstr+dyn->st_name;
          sym_sizearr[symtab_num+j] = dyn->st_size;
        }
        dyn++;
      }
    }
    symarr_flag = 1;
  }
  for (int a = 0; a < num; a++) {
    if(sym_arr[a] == addr){
      return sym_namearr[a];
    }
  }
  return NULL;

}

void* get_addr_bysym(char* fname)
{
  int symtab_num = get_num_sym(".symtab");
  int dynsym_num = get_num_sym(".dynsym");
  Elf32_Ehdr * ehdr = get_ehdr();
  int num = symtab_num + dynsym_num;

  if(symarr_flag == 0){
    get_sym_byaddr((void*)ehdr->e_entry);
  }
  for (int a = 0; a < num; a++) {
    if(sym_namearr[a]!=NULL){
      if(strcmp(sym_namearr[a], fname)==0){
        return sym_arr[a];
      }
    }
  }
  return NULL;
}

int print_all_func(void* addr)
{
  void* sb =SB;
  int symtab_num = get_num_sym(".symtab");
  int dynsym_num = get_num_sym(".dynsym");
  int num = symtab_num + dynsym_num;
  Elf32_Ehdr * ehdr = get_ehdr();
  Elf32_Shdr* shdr = get_shdr_byname(".text");
  void* buffer = sb + shdr->sh_offset + ((int)addr - ehdr->e_entry);

  if(symarr_flag == 0){
    get_sym_byaddr("test");
  }
  int size = 32;
  long side = shdr->sh_addr + shdr->sh_size;
  for (int a = 0; a < num; a++) {
    if( (long)sym_arr[a]>addr && (long)sym_arr[a]<side){
      side = sym_arr[a];
    }
  }
  size = side - (long)addr;
  print_asm(buffer, size, addr);

  return 0;
}

int print_s_text()
{
  Elf32_Shdr* shdr = get_shdr_byname(".text");
  int size = shdr->sh_size;
  long mem_addr = shdr->sh_addr;

  int symtab_num = get_num_sym(".symtab");
  int dynsym_num = get_num_sym(".dynsym");
  Elf32_Ehdr * ehdr = get_ehdr();
  int num = symtab_num + dynsym_num;

  if(symarr_flag == 0){
    get_sym_byaddr((void*)ehdr->e_entry);
  }
  printf("\n%s\n", "<SECTION .text>");
  long val = mem_addr;
  while (val>=mem_addr && val<=mem_addr+size) {
    for (int a = 0; a < num; a++) {
      if(val==sym_arr[a]){
        printf("%08lx    %s\n", val, sym_namearr[a]);
        break;
      }
    }
    val = val+1;
  }

  return 0;
}

int print_s_plt()
{
  void* sb =SB;
  Elf32_Shdr* shdr = get_shdr_byname(".plt");
  void* buffer = sb + shdr->sh_offset;
  void* mem_addr = (void*)shdr->sh_addr;
  int size = shdr->sh_size;
  int num = size/16;
  printf("\n<SECTION .plt>\n");
  for(int i=0;i<num;i++){
    printf("PLT[%d]\n", i);
    print_asm(buffer, 16, mem_addr);
    buffer += 16;
    mem_addr += 16;
  }
  return 0;
}

int  print_s_interp()
{
  void* sb =SB;
  Elf32_Shdr* shdr = get_shdr_byname(".interp");
  void* buffer = sb + shdr->sh_offset;
  printf("\n<SECTION .interp>\n%s\n", buffer);
  return 0;
}

int print_s_rel()
{
  Elf32_Sym* sym = get_by_sname(".dynsym");
  char* s_strtab = (char*)get_by_sname(".dynstr");
  printf("\n<SECTION .rel>\n");
  if(get_shdr_byname(".rel.dyn")!=NULL){
    void* sb =SB;
    Elf32_Shdr* shdr = get_shdr_byname(".rel.dyn");
    Elf32_Rel* rel = sb + shdr->sh_offset;
    int num = shdr->sh_size/8;
    printf("[.rel.dyn] count %d:\n", num);
    printf("OFFSET    TYPE             SYMBOL\n");
    for(int i=0;i<num;i++){
      printf("%08lx  ",rel->r_offset);
      switch (ELF32_R_TYPE(rel->r_info)) {
        case 6:printf("R_386_GLOB_DAT   ");break;
        case 7:printf("R_386_JUMP_SLOT  ");break;
        default:printf("%d", ELF32_R_TYPE(rel->r_info));
      }
      int ndx = ELF32_R_SYM(rel->r_info);
      Elf32_Sym* mysym = sym;
      while(ndx>0){
        mysym++;
        ndx--;
      }
      printf("%s\n", s_strtab+mysym->st_name);
      rel++;
    }
  }

  if(get_shdr_byname(".rel.plt")!=NULL){
    void* sb =SB;
    Elf32_Shdr* shdr = get_shdr_byname(".rel.plt");
    Elf32_Rel* rel = sb + shdr->sh_offset;
    int num = shdr->sh_size/8;
    printf("[.rel.plt] count %d:\n", num);
    printf("OFFSET    TYPE             SYMBOL\n");
    for(int i=0;i<num;i++){
      printf("%08lx  ",rel->r_offset);
      switch (ELF32_R_TYPE(rel->r_info)) {
        case 6:printf("R_386_GLOB_DAT   ");break;
        case 7:printf("R_386_JUMP_SLOT  ");break;
        default:printf("%d", ELF32_R_TYPE(rel->r_info));
      }
      int ndx = ELF32_R_SYM(rel->r_info);
      Elf32_Sym* mysym = sym;
      while(ndx>0){
        mysym++;
        ndx--;
      }
      printf("%s\n", s_strtab+mysym->st_name);
      rel++;
    }
  }

  return 0;
}

int print_s_dynamic()
{

}
//init
void init_readelf(char* filename)
{
  malloc_file(filename);
}
