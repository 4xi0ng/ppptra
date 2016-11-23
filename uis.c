#include "readelf.h"

int main(int argc, char *argv[])
{
	print_ehdr(argv[1]);
	return 0;
}