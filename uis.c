#include <stdio.h>
#include <udis86.h>
#include <string.h>

int main()
{
	ud_t ud_obj;
	//unsigned char buffer[4]={0}; 
	//long opc = 0x000058cc;
	//printf("opc : %lx\n", opc);

	ud_init(&ud_obj);
	ud_set_input_file(&ud_obj, fopen("test", "r"));
	ud_set_mode(&ud_obj, 32);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);

	//memcpy(buffer, &opc, 4);
	//printf("buffer: %x%x\n", buffer[0], buffer[1]);

	//ud_disassemble(&ud_obj);
	int i;
	for(i=0; i<20; i++){ 
	//while(ud_disassemble(&ud_obj)){
		ud_disassemble(&ud_obj);
		printf("\t%s\n", ud_insn_asm(&ud_obj));
	//}
	}

	return 0;
}