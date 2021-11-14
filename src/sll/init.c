#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/init.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



__SLL_EXTERNAL void sll_init(void){
	sll_platform_setup_console();
	_file_init_std_streams();
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_file_t* rf,sll_compilation_data_t* o){
	o->rf=rf;
	o->tm=sll_platform_get_current_time();
	o->h=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->idt.s[i].dt=NULL;
		o->idt.s[i].l=0;
	}
	o->idt.il=NULL;
	o->idt.ill=0;
	o->et.dt=NULL;
	o->et.l=0;
	o->ft.dt=NULL;
	o->ft.l=0;
	o->st.dt=NULL;
	o->st.l=0;
	_init_object_stack(o);
	o->_n_sc_id=1;
	IGNORE_RESULT(sll_create_string(&(o->st),fp,sll_string_length_unaligned(fp)));
}
