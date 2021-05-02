#include <language.h>
#include <stdio.h>



int main(int argc,const char** argv){
	if (argc<2){
		printf("Error! %u\n",__LINE__);
		return 1;
	}
	FILE* f;
	if (fopen_s(&f,argv[1],"rb")){
		printf("Error! %u\n",__LINE__);
		return 1;
	}
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
