#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* string_table,sll_string_t* string){
	for (sll_string_index_t i=0;i<string_table->length;i++){
		if (STRING_EQUAL(string_table->data+i,string)){
			sll_free_string(string);
			return i;
		}
	}
	string_table->length++;
	string_table->data=sll_reallocate(string_table->data,string_table->length*sizeof(sll_string_t));
	*(string_table->data+string_table->length-1)=*string;
	return string_table->length-1;
}
