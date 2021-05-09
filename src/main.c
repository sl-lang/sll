#include <language.h>
#include <stdio.h>



int main(int argc,const char** argv){
	if (argc<2){
		printf("Not Enough Arguments!\n");
		return 1;
	}
	FILE* f=NULL;
#ifdef _MSC_VER
	if (fopen_s(&f,argv[1],"rb")){// lgtm [cpp/path-injection]
#else
	if (!(f=fopen(argv[1],"rb"))){// lgtm [cpp/path-injection]
#endif
		printf("Unable to Open File '%s'!\n",argv[1]);
		return 1;
	}
	input_data_source_t in;
	create_input_data_source(f,&in);
	object_t* o=read_object(&in);
	if (IS_ERROR(o)){
		print_error(&in,GET_ERROR(o));
	}
	else{
		print_object(o,stdout);
	}
	fclose(f);
	return 0;
}
