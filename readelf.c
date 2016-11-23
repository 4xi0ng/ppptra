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
	printf("Magic:");
	for (int i = 0; i < 16; ++i)
	{
		printf(" %02x", ehdr->e_ident[i]);
	}
	printf("\n");
	
	//Class
	printf("Class: ");
	switch(ehdr->e_ident[4]){
		case ELFCLASSNONE: printf("NONE\n");break;
		case ELFCLASS32: printf("32\n");break;
		case ELFCLASS64: printf("64\n");break;
		case ELFCLASSNUM: printf("NUM\n");break;
		default: printf("\n");break;
	}

	return 0;
}