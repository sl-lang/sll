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
	input_data_stream_t is;
	create_input_data_stream(f,&is);
	object_t* o=read_object(&is);
	if (IS_ERROR(o)){
		print_error(&is,GET_ERROR(o));
	}
	else{
		print_object(o,stdout);
		FILE* of=NULL;
		char o_fp[512];
		const char* fp=argv[1];
		uint16_t i=0;
		while (*fp&&*fp!='.'){
			*(o_fp+i)=*fp;
			fp++;
			i++;
		}
		o_fp[i]='.';
		o_fp[i+1]='l';
		o_fp[i+2]='l';
		o_fp[i+3]='l';
		o_fp[i+4]='c';
		o_fp[i+5]=0;
#ifdef _MSC_VER
		if (fopen_s(&of,o_fp,"wb")){// lgtm [cpp/path-injection]
#else
		if (!(of=fopen(o_fp,"wb"))){// lgtm [cpp/path-injection]
#endif
			printf("Unable to Open File '%s'!\n",o_fp);
			return 1;
		}
		output_data_stream_t os;
		create_output_data_stream(of,&os);
		write_object(&os,o);
		fclose(of);
	}
	fclose(f);
	return 0;
}
