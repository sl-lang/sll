import util



def generate_operator_parser(cfg_fp,o_fp):
	util.log(f"Generating Operator Parser from '{cfg_fp}'...\n  Reading Configuration file...")
	with open(cfg_fp,"r") as f:
		dt=f.read().split("\n")
	util.log("  Parsing Data...")
	data={}
	mx_len=0
	for k in dt:
		k=k.lstrip()
		if (len(k)==0 or k[0]==";"):
			continue
		k=k.split(" ")
		if (len(k)==1 or len(k[0])==0):
			continue
		ln=len(k[0])
		mx_len=max(mx_len,ln)
		if (ln not in data):
			data[ln]=[]
		sc=None
		fl=None
		for e in k[2:]:
			if (len(e)==0):
				continue
			if (e[0]=="$"):
				sc=e[1:]
			else:
				fl=e
		data[ln].append((k[0],k[1],sc,fl))
	util.log(f"  Generating Code to '{o_fp}'...")
	with open(o_fp,"w") as f:
		f.write(f"#ifndef __SLL_GENERATED_OPERATOR_PARSER_H__\n#define __SLL_GENERATED_OPERATOR_PARSER_H__ 1\n#include <sll/_internal/common.h>\n#include <sll/_internal/parse.h>\n#include <sll/file.h>\n#include <sll/node.h>\n#include <sll/types.h>\n\n\n\nstatic __SLL_FORCE_INLINE sll_read_char_t _operator_parser(sll_node_t* o,sll_read_char_t c,unsigned int* fl,sll_source_file_t* sf,sll_file_t* rf){{\n\tsll_char_t str[{mx_len+1}];\n\tsll_string_length_t sz=0;\n\tdo{{\n\t\tif (sz<{mx_len+1}){{\n\t\t\tstr[sz]=(sll_char_t)c;\n\t\t\tsz++;\n\t\t}}\n\t\tc=sll_file_read_char(rf,NULL);\n\t}} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!=SLL_END_OF_DATA));\n\to->t=SLL_NODE_TYPE_OPERATION_LIST;\n")
		st=False
		for k,v in data.items():
			f.write(f"\t{('else ' if st else '')}if (sz=={k}){{\n")
			st=True
			s_st=False
			for e,nm,sc,fl in v:
				f.write(f"\t\t{('else ' if s_st else '')}if ("+"&&".join([f"str[{i}]=='{c}'" for i,c in enumerate(e)])+f"){{\n\t\t\to->t=SLL_NODE_TYPE_{nm.upper()};\n")
				if (sc is not None):
					f.write(f"\t\t\to->dt.{sc}.sc=sf->_n_sc_id;\n")
				if (fl is not None):
					f.write(f"\t\t\t(*fl)|=EXTRA_COMPILATION_DATA_{fl.upper()};\n")
				f.write("\t\t}\n")
				s_st=True
			f.write("\t}\n")
		f.write("\treturn c;\n}\n\n\n\n#endif\n")
