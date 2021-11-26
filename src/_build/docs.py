import json
import re



DOCS_COMMENT_REGEX=re.compile(r"\/\*\*(.*?)\*\/",re.S)



def create_docs(fl):
	o=[]
	ap_dt=[]
	for nm in fl:
		with open(nm,"r") as f:
			for k in DOCS_COMMENT_REGEX.findall(f.read().replace("\r\n","\n")):
				dt={"flag":[],"name":None,"desc":None,"args":[],"ret":[],"type":None}
				for e in k.split("\n"):
					e=e.strip()
					if (e[:1]=="*"):
						e=e[1:].strip()
					if (len(e)==0):
						continue
					t=e.split(" ")[0]
					if (t=="\\flags"):
						for se in e.lower().split(" ")[1:]:
							if (se not in dt["flag"]):
								dt["flag"].append(se)
					elif (t=="\\name"):
						if (dt["name"] is not None):
							raise RuntimeError("Only one '\\name' tag can be present")
						dt["name"]=e.split(" ")[1]
					elif (t=="\\desc"):
						if (dt["desc"] is not None):
							raise RuntimeError("Only one '\\desc' tag can be present")
						dt["desc"]=e[6:]
					elif (t=="\\arg"):
						i=(e.index("->") if "->" in e else len(e))
						se=e[4:i].strip()
						e=e[i+2:].strip()
						if ("api" in dt["flag"]):
							dt["args"].append({"type":se.replace("?",""),"opt":("?" in se),"desc":e})
						else:
							i=se.rindex(" ")
							dt["args"].append({"type":se[:i],"name":se[i+1:],"desc":e})
					elif (t=="\\ret"):
						i=(e.index("->") if "->" in e else len(e))
						dt["ret"].append({"type":e[4:i].strip(),"desc":e[i+2:].strip()})
					elif (t=="\\type"):
						if (dt["type"] is not None):
							raise RuntimeError("Only one '\\type' tag can be present")
						i=(e.index("->") if "->" in e else len(e))
						dt["type"]={"type":e[5:i].strip(),"desc":e[i+2:].strip()}
					else:
						raise RuntimeError(f"Unknown tag '{t}'")
				if (dt["name"] is None):
					raise RuntimeError("A '\\name' tag is required")
				if ("var" not in dt["flag"]):
					del dt["type"]
				elif ("api" in dt["flag"]):
					raise RuntimeError("'api' and 'var' cannot be both present")
				if ("api" in dt["flag"]):
					ap_dt.append(dt)
				else:
					if (len(dt["ret"])>1):
						raise RuntimeError("Only one '\\ret' tag can be present")
					dt["ret"]=(None if len(dt["ret"])==0 else dt["ret"][0])
					o.append(dt)
	with open("build/docs.json","w") as f:
		f.write(json.dumps(o,indent="\t"))
	return (o,ap_dt)
