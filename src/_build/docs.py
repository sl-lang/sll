import re



DOCS_COMMENT_REGEX=re.compile(r"\/\*\*(.*?)\*\/",re.S)
VALID_FLAGS=["api","check_output","func","group","macro","subgroup","type","union","var","var_arg"]



def create_docs(fl):
	o=[]
	g_dt={}
	sg_dt={}
	for nm in fl:
		with open(nm,"r") as f:
			for k in DOCS_COMMENT_REGEX.findall(f.read().replace("\r\n","\n")):
				dt={"flag":[],"name":None,"group":None,"subgroup":None,"desc":None,"api_fmt":None,"args":[],"ret":None,"type":None}
				for e in k.split("\n"):
					e=e.strip()
					if (e[:1]=="*"):
						e=e[1:].strip()
					if (len(e)==0):
						continue
					t=e.split(" ")[0]
					if (t=="\\flags"):
						for se in e.lower().split(" ")[1:]:
							if (se not in VALID_FLAGS):
								raise RuntimeError(f"{nm} ({dt['name']}): Unknown flag '{se}'")
							if (se not in dt["flag"]):
								dt["flag"].append(se)
					elif (t=="\\name"):
						if (dt["name"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\name' tag can be present")
						dt["name"]=e[5:].strip()
					elif (t=="\\group"):
						if (dt["group"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\group' tag can be present")
						dt["group"]=e.split(" ")[1]
					elif (t=="\\subgroup"):
						if (dt["subgroup"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\subgroup' tag can be present")
						dt["subgroup"]=e.split(" ")[1]
					elif (t=="\\desc"):
						if (dt["desc"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\desc' tag can be present")
						dt["desc"]=e[6:]
					elif (t=="\\api"):
						if (dt["api_fmt"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\api' tag can be present")
						dt["api_fmt"]=e[5:].strip()
					elif (t=="\\arg"):
						i=(e.index("->") if "->" in e else len(e))
						se=e[4:i].strip()
						e=e[i+2:].strip()
						if ("api" in dt["flag"]):
							dt["args"].append({"type":se,"desc":e})
						else:
							if (" " not in se):
								raise RuntimeError(f"{nm} ({dt['name']}): No variable name present")
							i=se.rindex(" ")
							dt["args"].append({"type":se[:i],"name":se[i+1:],"desc":e})
					elif (t=="\\ret"):
						if (dt["ret"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\ret' tag can be present")
						i=(e.index("->") if "->" in e else len(e))
						dt["ret"]={"type":e[4:i].strip(),"desc":e[i+2:].strip()}
					elif (t=="\\type"):
						if (dt["type"] is not None):
							raise RuntimeError(f"{nm} ({dt['name']}): Only one '\\type' tag can be present")
						i=(e.index("->") if "->" in e else len(e))
						dt["type"]={"type":e[5:i].strip(),"desc":e[i+2:].strip()}
					else:
						raise RuntimeError(f"{nm} ({dt['name']}): Unknown tag '{t}'")
				if (dt["name"] is None):
					raise RuntimeError(f"{nm} ({dt['name']}): A '\\name' tag is required")
				if (dt["group"] is None):
					raise RuntimeError(f"{nm} ({dt['name']}): A '\\group' tag is required")
				if ("macro" in dt["flag"] and "type" in dt["flag"]):
					raise RuntimeError(f"{nm} ({dt['name']}): 'macro' and 'type' flags cannot be present together")
				if ("union" in dt["flag"] and "type" not in dt["flag"]):
					raise RuntimeError(f"{nm} ({dt['name']}): 'union' flag cannot be present without the 'type' flag")
				if ("var" not in dt["flag"]):
					del dt["type"]
				if ("group" in dt["flag"]):
					if (len(dt["flag"])>1):
						raise RuntimeError(f"{nm} ({dt['name']}): 'group' flag has to be alone")
					g_dt[dt["group"]]={"name":dt["name"],"desc":dt["desc"]}
				elif ("subgroup" in dt["flag"]):
					if (len(dt["flag"])>1):
						raise RuntimeError(f"{nm} ({dt['name']}): 'subgroup' flag has to be alone")
					sg_dt[dt["subgroup"]]={"name":dt["name"],"group":dt["group"],"desc":dt["desc"]}
				else:
					o.append(dt)
	for k in o:
		if (k["group"] not in g_dt):
			raise RuntimeError(f"Unknown group '{k['group']}'")
		if (k["subgroup"] is not None and k["subgroup"] not in sg_dt):
			raise RuntimeError(f"Unknown subgroup '{k['subgroup']}'")
	return {"groups":g_dt,"subgroups":sg_dt,"data":o}
