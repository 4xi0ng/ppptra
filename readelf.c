#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * f;

void* read2mem(int start_addr, int offset, int size)
{
	void* mem_pt;

	if(f == NULL){
		perror("Open file failed");
		exit(1);
	}
	if(fseek(f, offset, start_addr) != 0){
		perror("Fseek failed");
		exit(1);
	}
	mem_pt = malloc(size);
	if(fread(mem_pt, 1, size, f) != size){
		perror("Read file failed");
		exit(1);
	}
	return mem_pt;
}

int open_file(char * filename)
{
	f = fopen(filename, "r");
	if(f == NULL){
		perror("Open file failed");
		exit(1);
	}
	return 0;
}

void* read_ehdr()
{
	Elf32_Ehdr * ehdr = (Elf32_Ehdr *)read2mem(0, 0, 52);

	//is elf
	long elfheader = 0x464c457f;
	if(memcmp(&elfheader, ehdr->e_ident, 4) != 0){
		perror("Not ELF!!");
		exit(1);
	}

	return ehdr;
}

int print_ehdr(Elf32_Ehdr * cst_ehdr)
{
	Elf32_Ehdr * ehdr = cst_ehdr;
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

void* read_shdr(Elf32_Ehdr* cst_ehdr)
{
	Elf32_Ehdr * ehdr = cst_ehdr;
	int SHentsize = ehdr->e_shentsize;
	int SHentnum = ehdr->e_shnum;

	Elf32_Shdr * shdr = (Elf32_Shdr *)read2mem(0, (int)ehdr->e_shoff, SHentnum * SHentsize);

	return shdr;
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

int print_shdr(Elf32_Shdr * cst_shdr, Elf32_Ehdr* cst_ehdr)
{
	Elf32_Ehdr* ehdr = cst_ehdr;
	Elf32_Shdr* shdr = cst_shdr;

	for (int i=0; i<ehdr->e_shstrndx; i++){
		shdr++;
	}
	int addr = shdr->sh_addr+shdr->sh_offset;
	char* str = (char *)read2mem(0, addr, shdr->sh_size);
	printf("\n<--SECTION HEADER-->\n");
	printf("    [Nr]%-20s%-12s%-9s%-7s%-7s%-3s%-3s%-3s%-3s%-3s\n", "Name", "Type","Addr","Off", "Size","ES","Fg","Lk","If","Al");

	//reset shdr
	shdr = cst_shdr;
	for (int j=0; j<ehdr->e_shnum; j++){
		printf("    [%02d]%-20s", j, str+shdr->sh_name);
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

