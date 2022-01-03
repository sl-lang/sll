import util



OPTIMIZER_STACK_SIZE=5



def _parse_cond(cnd):
	cnd=cnd.lower().split(".")
	return (f"SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[{int(cnd[0])}])" if len(cnd)==1 else f"st[{int(cnd[0])}]->dt.{cnd[1].strip()}")



def _parse_value(v):
	o=""
	st=True
	for k in v.lstrip().lower().split("|"):
		k=k.strip()
		if (st):
			st=False
		else:
			o+="|"
		if (k[0] in "+"):
			o+=k[1:]
		elif (k[0] in "-"):
			o+=k
		elif (k[0] in "0123456789"):
			o+=_parse_cond(k)
		elif (k=="nop"):
			o+="ASSEMBLY_INSTRUCTION_TYPE_NOP"
		else:
			o+="SLL_ASSEMBLY_INSTRUCTION_"
			if (k[:5]!="flag_"):
				o+="TYPE_"
			o+=k.upper()
	return o



def _generate_cond(f,dt,lvl,sl):
	if (type(dt)==str):
		for k in dt.split(","):
			f.write("\t"*lvl)
			k=k.split("=")
			k[0]=k[0].strip()
			if (k[0][0]=="$"):
				n=int(k[0][1:])
				sl.add(n)
				f.write(f"goto _shift_{n};\n")
			else:
				e=k[0].split(".")
				if (len(e)==1):
					f.write(f"st[{int(e[0])}]->t=")
				else:
					f.write(f"st[{int(e[0])}]->dt.{e[1].strip()}=")
				f.write(_parse_value(k[1])+";\n")
		return
	pfx=""
	for i in range(0,len(dt[0])):
		f.write("\t"*lvl+pfx+"if (")
		pfx="else "
		cnd=dt[0][i].split("=")
		obj=_parse_cond(cnd[0])
		if (cnd[1][0]=="!"):
			cmp_="!="
			join="&&"
			cnd[1]=cnd[1][1:]
			pfx=""
		else:
			cmp_="=="
			join="||"
		st=True
		for k in cnd[1].split("|"):
			if (not st):
				f.write(join)
			f.write(obj+cmp_+_parse_value(k))
			st=False
		f.write("){\n")
		_generate_cond(f,dt[1][i],lvl+1,sl)
		f.write("\t"*lvl+"}\n")



def generate_assembly_optimizer(cfg_fp,o_fp):
	util.log(f"Generating Assembly Optimizer from '{cfg_fp}'...\n  Reading Configuration file...")
	with open(cfg_fp,"r") as f:
		dt=f.read().split("\n")
	util.log("  Parsing Data...")
	data=([],[])
	for k in dt:
		k=k.lstrip()
		if (len(k)==0 or k[0]==";"):
			continue
		k=k.split("->")
		if (len(k)!=2):
			continue
		cond=k[0].split(",")
		res=k[1]
		c=data
		for i in range(0,len(cond)-1):
			e=cond[i].replace(" ","")
			if (e not in c[0]):
				c[0].append(e)
				c[1].append(([],[]))
			c=c[1][c[0].index(e)]
		e=cond[-1].replace(" ","")
		if (e in c[0]):
			raise RuntimeError(f"Duplicated condition: {','.join(cond)}")
		c[0].append(e)
		c[1].append(res)
	util.log(f"  Generating Code to '{o_fp}'...")
	with open(o_fp,"w") as f:
		f.write("#include <sll/_sll_internal.h>\n#include <sll/assembly.h>\n#include <sll/types.h>\n\n\n\nstatic SLL_FORCE_INLINE void _optimize_assembly(sll_assembly_instruction_t** st,sll_assembly_instruction_t* nop){\n")
		sl=set()
		_generate_cond(f,data,1,sl)
		for k in sl:
			f.write(f"\treturn;\n_shift_{k}:\n")
			i=0
			while (i<OPTIMIZER_STACK_SIZE-1):
				if (i+k>=OPTIMIZER_STACK_SIZE):
					f.write("\t"+f"st[{i}]=nop;\n")
				else:
					f.write("\t"+f"st[{i}]=st[{i+k}];\n")
				i+=1
		f.write("}\n")
