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

int disas_num(unsigned char* opc, int num)
{
	int i;
	memcpy(buffer, opc, 512);
	for(i=0; i<num; i++){
		ud_disassemble(&ud_obj);
	}
}