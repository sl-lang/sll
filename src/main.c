#define ALLOW_EMPTY_EXPRESSIONS 1
#include <language.h>
#include <stdio.h>



int main(int argc,const char** argv){
	if (argc<2){
		printf("Not Enough Arguments!\n");
		return 1;
	}
	FILE* f=fopen(argv[1],"rb");
	if (!f){
		printf("Unable to Open File '%s'!\n",argv[1]);
		return 1;
	}
	set_feature(FEATURE_EMPTY_EXPRESSION,1);
	set_feature(FEATURE_OPERATION_LIST,1);
	object_t* o=read_object(f);
	if (IS_ERROR(o)){
		print_error(GET_ERROR(o));
	}
	else{
		print_object(o,stdout);
	}
	fclose(f);
	return 0;
}
