#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

int print_ehdr(char * filename)
{
	FILE * f;
	Elf32_Ehdr * ehdr;

	f = fopen(filename, "r");
	if(f == NULL){
		perror("Open file failed");
		exit(1);
	}

	ehdr = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));
	fread(ehdr, 1, 52, f);

	//is elf
	long elfheader = 0x464c457f;
	if(memcmp(&elfheader, ehdr->e_ident, 4) != 0){
		perror("Not ELF!!");
		exit(1);
	}
	printf("<--ELF HEADER-->\n");

	//Magic
	printf("    Magic: ");
	for (int i = 0; i < 16; ++i)
	{
		printf(" %02x", ehdr->e_ident[i]);
	}
	printf("\n");
	
	//Class
	printf("    Class: ");
	switch(ehdr->e_ident[4]){
		case ELFCLASSNONE: printf("NONE\n");break;
		case ELFCLASS32: printf("32\n");break;
		case ELFCLASS64: printf("64\n");break;
		case ELFCLASSNUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Data
	printf("    Data: ");
	switch(ehdr->e_ident[5]){
		case ELFDATANONE: printf("NONE\n");break;
		case ELFDATA2LSB: printf("little endian\n");break;
		case ELFDATA2MSB: printf("big endian\n");break;
		case ELFDATANUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Version
	printf("    Version: ");
	switch(ehdr->e_ident[6]){
		case EV_NONE: printf("NONE\n");break;
		case EV_CURRENT: printf("1 current version\n");break;
		case EV_NUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Type
	printf("    Type: ");
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
	printf("    Machine: ");
	switch(ehdr->e_machine){
		case EM_NONE: printf("NONE\n");break;
		case EM_M32: printf("AT&T WE 32100\n");break;
		case EM_386: printf("Intel 80386 \n");break;
		case EM_860: printf("Intel 80860\n");break;
		case EM_MIPS: printf("MIPS R3000 big-endian\n");break;
		case EM_MIPS_RS3_LE: printf("MIPS R3000 little-endian\n");break;
		case EM_ARM: printf("ARM\n");break;
		default: printf("\n");break;
	}

	//Version
	printf("    Version: ");
	switch(ehdr->e_version){
		case EV_NONE: printf("NONE\n");break;
		case EV_CURRENT: printf("1 current version\n");break;
		case EV_NUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	//Entry point address
	/*
	*/

	return 0;
}