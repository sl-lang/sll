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
	compilation_data_t c_dt;
	init_compilation_data(argv[1],&is,&c_dt);
	error_t e=read_all_objects(&c_dt);
	if (IS_ERROR(e)){
		print_error(&is,GET_ERROR(e));
	}
	else{
		print_object(c_dt.h,&c_dt,stdout);
		error_t e=optimize_object(c_dt.h);
		if (IS_ERROR(e)){
			print_error(&is,GET_ERROR(e));
		}
		else{
			remove_object_debug_data(c_dt.h);
			remove_object_padding(c_dt.h);
			print_object(c_dt.h,&c_dt,stdout);
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
			write_object(&os,c_dt.h);
			fclose(of);
		}
	}
	fclose(f);
	return 0;
}
