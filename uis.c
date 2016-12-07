#include "readelf.h"

int main(int argc, char *argv[])
{
	char * inp;
	open_file(argv[1]);
	Elf32_Ehdr * cst_ehdr = read_ehdr();
	Elf32_Shdr * cst_shdr = read_shdr(cst_ehdr);
	while(1){
		printf(">");
		gets(inp);
		if(strcmp(inp, "show elf")==0){
			print_ehdr(cst_ehdr);
		}else if(strcmp(inp, "show sections")==0){
			print_shdr(cst_shdr, cst_ehdr);
		}else if(strcmp(inp, "q")==0){
			printf("Bye~\n");
			return 0;
		}else if(strcmp(inp, "disas ")==0){
			print_shdr(cst_shdr, cst_ehdr);
		}

	}
	
	
	
	
	return 0;
}