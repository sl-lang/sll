import docs
import os
import re
import util



TYPE_POINTER_REGEX=re.compile(r"(\*+)")
TYPE_MAP={"__SLL_S1":"signed char","__SLL_U1":"_Bool","__SLL_S8":"signed char","__SLL_U8":"unsigned char","__SLL_S16":"signed short int","__SLL_U16":"unsigned short int","__SLL_S32":"signed int","__SLL_U32":"unsigned int","__SLL_S64":"signed long long int","__SLL_U64":"unsigned long long int","__SLL_F32":"float","__SLL_F64":"double"}
ROOT=("" if os.getenv("DOMAIN_ROOT",None) is not None else ".")



def _generate(nm,dt):
	nm=nm.replace("\\","/")
	if (nm[0]=="/"):
		nm=nm[1:]
	nm="build/web/"+nm
	os.makedirs(nm[:nm.rindex("/")],exist_ok=True)
	if (nm[-5:]==".html"):
		dt=dt.replace("{{ROOT}}",ROOT)
	util.log(f"  Generated '{nm}'")
	with open(nm,"w") as f:
		f.write(dt)



def _generate_id(g,sg,nm,t):
	o=g
	if (sg is not None):
		o+="-"+sg
		if (nm is not None):
			o+="-"+nm+"-"
			if ("func" in t):
				o+="f"
			if ("macro" in t):
				o+="m"
	return o.lower()



def _add_code_type(t):
	for k,v in TYPE_MAP.items():
		t=t.replace(k,v)
	if (t[:2]=="__" and t[-2:]=="__"):
		return f"<span class=\"code-type-other\">{t}</span>"
	return TYPE_POINTER_REGEX.sub(r"""<span class="code-type-pointer">\1</span>""",f"<span class=\"code-type\">{t}</span>").replace("const","<span class=\"code-keyword\">const</span>")



def _generate_docs(dt):
	m={}
	for k in dt["groups"]:
		m[k]={"":[]}
	for k in dt["data"]:
		if (k["subgroup"] is None):
			m[k["group"]][""].append(k)
		elif (k["subgroup"] not in m[k["group"]]):
			m[k["group"]][k["subgroup"]]=[k]
		else:
			m[k["group"]][k["subgroup"]].append(k)
	data="<div>"
	for k,v in sorted(m.items(),key=lambda e:dt["groups"][e[0]]["name"]):
		elem_id=_generate_id(k,None,None,None)
		data+=f"<div><div class=\"group-title\"><a id=\"{elem_id}\" href=\"#{elem_id}\">{dt['groups'][k]['name']}</a></div><h2>{dt['groups'][k]['desc']}</h2>"
		for sk,sv in sorted(v.items(),key=lambda e:("" if e[0]=="" else dt["subgroups"][e[0]]["name"])):
			if (len(sv)==0):
				continue
			if (len(sk)!=0):
				elem_id=_generate_id(k,sk,None,None)
				data+=f"<div><div class=\"subgroup-title\"><a id=\"{elem_id}\" href=\"#{elem_id}\">{dt['subgroups'][sk]['name']}</a></div><h3>{dt['subgroups'][sk]['desc']}</h3>"
			for e in sorted(sv,key=lambda se:se["name"]):
				elem_id=_generate_id(k,sk,e["name"],e["flag"])
				data+=f"<div><a id=\"{elem_id}\" href=\"#{elem_id}\"><pre class=\"code\">"
				if ("func" in e["flag"]):
					if ("macro" in e["flag"]):
						data+="<span class=\"code-keyword\">#define</span> <span class=\"code-name\">"+e["name"]+"</span>("
						st=True
						for a in e["args"]:
							if (st):
								st=False
							else:
								data+=","
							data+=_add_code_type(a["type"])+" <span class=\"code-arg\">"+a["name"]+"</span>"
						if ("var_arg" in e["flag"]):
							if (not st):
								data+=","
							data+="<span class=\"code-keyword\">...</span>"
						data+=")"
						if (e["ret"] is not None):
							data+=" <span class=\"code-comment\">-&gt;</span> "+_add_code_type(e["ret"]["type"])
					else:
						if (e["api_fmt"] is not None):
							data+="<span class=\"code-annotation\">(api_call)</span> "
						if ("check_output" in e["flag"]):
							data+="<span class=\"code-annotation\">(check_output)</span> "
						data+=_add_code_type(e["ret"]["type"] if e["ret"] is not None else "void")
						data+=" <span class=\"code-name\">"+e["name"]+"</span>("
						if (len(e["args"])==0):
							data+=_add_code_type("void")
						else:
							st=True
							for a in e["args"]:
								if (st):
									st=False
								else:
									data+=","
								data+=_add_code_type(a["type"])+" <span class=\"code-arg\">"+a["name"]+"</span>"
						if ("var_arg" in e["flag"]):
							data+=",<span class=\"code-keyword\">...</span>"
						data+=");"
				else:
					if ("macro" in e["flag"]):
						data+="<span class=\"code-keyword\">#define</span> <span class=\"code-name\">"+e["name"]+"</span> <span class=\"code-comment\">-&gt;</span> "+_add_code_type(e["type"]["type"])
					else:
						data+=_add_code_type(e["type"]["type"])+" <span class=\"code-name\">"+e["name"]+"</span>;"
				data+=f"</pre></a><pre>Description: {e['desc']}"
				if (e["api_fmt"] is not None):
					data+=f"\nAPI Signature: <span style=\"color: #1b84e3\">{e['api_fmt']}</span>"
				if (e["args"]):
					data+="\nArguments:"
					for a in e["args"]:
						data+=f"\n  {a['name']} -> {a['desc']}"
				if (e["ret"] is not None):
					data+=f"\nReturn Value: {e['ret']['desc']}"
				data+="</pre></div>"
			if (len(sk)!=0):
				data+="</div>"
		data+="</div>"
	return data+"</div>"



def generate():
	util.log("Generating Build Directory...")
	if (not os.path.exists("build/web")):
		os.mkdir("build/web")
	util.log("Reading CSS Files...")
	for k in os.listdir("src/web/css"):
		with open("src/web/css/"+k,"r") as rf:
			_generate("/css/"+k,rf.read())
	util.log("Reading JS Files...")
	for k in os.listdir("src/web/js"):
		with open("src/web/js/"+k,"r") as rf:
			_generate("/js/"+k,rf.read())
	util.log("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt=docs.create_docs(d_fl)
	util.log(f"Generating Table of Content & Pages for {len(d_dt['data'])} Symbols...")
	data=_generate_docs(d_dt)
	util.log("Reading 'src/web/docs.html'...")
	with open("src/web/docs.html","r") as rf:
		_generate("/index.html",rf.read().replace("{{DATA}}",data))
	util.log("Reading 'src/web/404.html'...")
	with open("src/web/404.html","r") as rf:
		_generate("/404.html",rf.read())
	util.log("Reading 'src/web/apt.sh'...")
	with open("src/web/apt.sh","r") as rf:
		_generate("/apt",rf.read())
