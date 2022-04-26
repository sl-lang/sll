import docs
import os
import util



def _generate(nm,dt):
	nm=nm.replace("\\","/")
	if (nm[0]=="/"):
		nm=nm[1:]
	nm="build/web/"+nm
	os.makedirs(nm[:nm.rindex("/")],exist_ok=True)
	if (nm[-5:]==".html"):
		dt=dt.replace(b"{{ROOT}}",(b"" if os.getenv("DOMAIN_ROOT",None) is not None else b"."))
	util.log(f"  Generated '{nm}'")
	with open(nm,"wb") as f:
		f.write(dt)



def _generate_pages(dt,pg_src):
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
	toc=""
	for k,v in sorted(m.items(),key=lambda e:dt["groups"][e[0]]["name"]):
		toc+=f"<div class=\"group\" id=\"{k}\"><a href=\"{{{{ROOT}}}}/{k}.html\"><h2 class=\"title\">{dt['groups'][k]['name']}</h2></a><div class=\"group-box\">"
		pg=f"<h1>{dt['groups'][k]['name']}</h1><h2>{dt['groups'][k]['desc']}</h2>"
		for sk,sv in sorted(v.items(),key=lambda e:("" if e[0]=="" else dt["subgroups"][e[0]]["name"])):
			if (len(sv)==0):
				continue
			toc+="<div class=\"subgroup\">"
			if (len(sk)!=0):
				toc+=f"<a href=\"{{{{ROOT}}}}/{k}.html#{sk}\"><h3 class=\"sg-title\">{dt['subgroups'][sk]['name']}</h3></a>"
				pg+=f"<a id=\"{sk}\" href=\"#{sk}\" style=\"text-decoration: none;color: #3010ff\"><h2>{dt['subgroups'][sk]['name']}</h2></a><h4>{dt['subgroups'][sk]['desc']}</h4>"
			toc+="<ul>"
			for e in sorted(sv,key=lambda se:se["name"]):
				toc+=f"<li><a href=\"{{{{ROOT}}}}/{e['group']}.html#{e['name']}\">{e['name']+('()' if 'func' in e['flag'] else '')}</a></li>"
				pg+=f"<div><a id=\"{e['name']}\" href=\"#{e['name']}\" style=\"text-decoration: none;color: #ff0000\"><pre>"
				if ("func" in e["flag"]):
					if ("macro" in e["flag"]):
						pg+="#define "+e["name"]+"("
						st=True
						for a in e["args"]:
							if (st):
								st=False
							else:
								pg+=","
							pg+="<span style=\"color: #ceb187\">"+a["type"]+"</span> "+a["name"]
						if ("var_arg" in e["flag"]):
							if (not st):
								pg+=","
							pg+="..."
						pg+=")"
						if (e["ret"] is not None):
							pg+=" -> "+e["ret"]["type"]
					else:
						if ("check_output" in e["flag"]):
							pg+="<span style=\"color: #cf89a2\">(check_output)</span> "
						if (e["ret"] is not None):
							pg+=e["ret"]["type"]
						else:
							pg+="void"
						pg+=" "+e["name"]+"("
						if (len(e["args"])==0):
							pg+="void"
						else:
							st=True
							for a in e["args"]:
								if (st):
									st=False
								else:
									pg+=","
								pg+=a["type"]+" "+a["name"]
						if ("var_arg" in e["flag"]):
							pg+=",..."
						pg+=");"
				else:
					if ("macro" in e["flag"]):
						pg+="#define "+e["name"]+" -> "+e["type"]["type"]
					else:
						pg+=e["type"]["type"]+" "+e["name"]+";"
				pg+=f"</pre></a><pre>Description: {e['desc']}"
				if (e["api_fmt"] is not None):
					pg+=f"\nAPI Signature: <span style=\"color: #1b84e3\">{e['api_fmt']}</span>"
				if (e["args"]):
					pg+="\nArguments:"
					for a in e["args"]:
						pg+=f"\n  {a['name']} -> {a['desc']}"
				if (e["ret"] is not None):
					pg+=f"\nReturn Value: {e['ret']['desc']}"
				pg+="</pre></div>"
			toc+="</ul></div>"
		toc+="</div></div>"
		_generate(f"/{k}.html",pg_src.replace(b"{{DATA}}",bytes(pg,"utf-8")).replace(b"{{NAME}}",bytes(dt["groups"][k]["name"],"utf-8")))
	return bytes(toc,"utf-8")



def generate():
	util.log("Generating Build Directory...")
	if (not os.path.exists("build/web")):
		os.mkdir("build/web")
	util.log("Reading CSS Files...")
	for k in os.listdir("src/web/css"):
		with open("src/web/css/"+k,"rb") as rf:
			_generate("/css/"+k,rf.read())
	util.log("Reading JS Files...")
	for k in os.listdir("src/web/js"):
		with open("src/web/js/"+k,"rb") as rf:
			_generate("/js/"+k,rf.read())
	util.log("Collecting Documentation Files...")
	d_fl=util.get_docs_files()
	util.log(f"  Found {len(d_fl)} Files\nGenerating Documentation...")
	d_dt=docs.create_docs(d_fl)
	util.log("Reading 'src/web/page.html'...")
	with open("src/web/page.html","rb") as rf:
		pg_src=rf.read()
	util.log(f"Generating Table of Content & Pages for {len(d_dt['data'])} Symbols...")
	toc=_generate_pages(d_dt,pg_src)
	util.log("Reading 'src/web/index.html'...")
	with open("src/web/index.html","rb") as rf:
		_generate("/index.html",rf.read().replace(b"{{DATA}}",toc))
	util.log("Reading 'src/web/404.html'...")
	with open("src/web/404.html","rb") as rf:
		_generate("/404.html",rf.read())
	util.log("Reading 'src/web/apt.sh'...")
	with open("src/web/apt.sh","rb") as rf:
		_generate("/apt",rf.read())
