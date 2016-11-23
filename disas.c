#include <udis86.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

int print_asm(void* buffer, int buffer_size)
{
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
	ud_set_input_buffer(&ud_obj, buffer, buffer_size);

	void* addr = buffer;
	int offset = 0;
	int len;
	while (ud_disassemble(&ud_obj)) {
		len = ud_insn_len(&ud_obj);

		printf("%p<+%d>    ", addr, offset);	
		
		printf(" %-19s", ud_insn_hex(&ud_obj));
	
		printf(" %s\n", ud_insn_asm(&ud_obj));
		addr += len;
		offset +=  len;
	}
	return 0;
}