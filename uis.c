#include "readelf.h"

int main(int argc, char *argv[])
{
	char * inp;
	open_file(argv[1]);
	while(1){
		printf(">");
		gets(inp);
		if(strcmp(inp, "show elf")==0){
			print_ehdr(read_ehdr());
		}else if(strcmp(inp, "show sections")==0){
			print_shdr(read_shdr(read_ehdr()), read_ehdr());
		}else if(strcmp(inp, "q")==0){
			printf("Bye~\n");
			return 0;
		}else if(strcmp(inp, "disas ")==0){
			print_shdr(read_shdr(read_ehdr()), read_ehdr());
		}

	}
	
	
	
	
	return 0;
}