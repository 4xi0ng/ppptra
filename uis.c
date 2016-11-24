#include "readelf.h"

int main(int argc, char *argv[])
{

	open_file(argv[1]);
	
	print_ehdr(read_ehdr());
	
	print_shdr(read_shdr(read_ehdr()), read_ehdr());
	return 0;
}