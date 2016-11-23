#include <stdio.h>
#include <udis86.h>
#include <string.h>
#include "disas.h"

int main()
{
	int num = 1;
	const char* result;
	long temp = 0x58cc;
	//unsigned char* masm =  &temp;
	memcpy(buffer, &temp, 4);
	result = disas_num(num);
	int i;
	//for(i=0; i<num; i++){
		printf("huibian: %s\n",  result);
	//}

	return 0;
}