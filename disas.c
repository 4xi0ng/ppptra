#include <udis86.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "readelf.h"

ud_t ud_obj;

void custom_init_ud_t(void* buffer, int buffer_size)
{
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_input_buffer(&ud_obj, buffer, buffer_size);

	/*
	you need do something like this:

	while (ud_disassemble(&ud_obj)) {
		ud_insn_len(&ud_obj);

		ud_insn_hex(&ud_obj);

		ud_insn_asm(&ud_obj);
	}
	*/
}

int print_asm(void* buffer, int buffer_size, void* mem_addr)
{
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_pc(&ud_obj, (int)mem_addr);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_input_buffer(&ud_obj, buffer, buffer_size);

	//mem_addr = buffer;
	int offset = 0;
	int len;
	while (ud_disassemble(&ud_obj)) {
		char* func = get_sym_byaddr(mem_addr);
		if(func != NULL){
			printf("\033[32m%s\033[0m\n", func);
		}
		len = ud_insn_len(&ud_obj);

		char * r = ud_insn_asm(&ud_obj);
		if(strcmp(r, "invalid")==0){
			return 0;
		}
		printf("%p<+%d>    ", mem_addr, offset);

		printf(" %-19s", ud_insn_hex(&ud_obj));

		printf(" %s\n", r);
		mem_addr += len;
		offset +=  len;
	}
	return 0;
}
