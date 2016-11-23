#include <udis86.h>
#include <string.h>

ud_t ud_obj;
unsigned char buffer[512];

void init_ud_t(){
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_vendor(&ud_obj, UD_VENDOR_INTEL);
	ud_set_input_buffer(&ud_obj, buffer, 512);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);
}

char* disas_num(int num)
{
	int i;
	const char * result;
	for(i=0; i<num; i++){
		if(ud_disassemble(&ud_obj)){
			result = ud_insn_asm(&ud_obj);
		}
		else return result;
	}
	return result;
}