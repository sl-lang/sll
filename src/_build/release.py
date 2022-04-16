import header
import json
import os
import re
import requests
import sys
import util



FILE_PATH_MAIN_REGEX=re.compile(r"\[(`[^`]+?`)\]\[main((?:\/[a-zA-Z0-9_\-\.]+)+?)\]")
FILE_PATH_REGEX=re.compile(r"\[`[^`]+?`\]\[([0-9]+\.[0-9]+\.[0-9]+)((?:\/[a-zA-Z0-9_\-\.]+)+?)\]")
HEADING_REGEX=re.compile(r"\n#(#+) ")
ISSUE_REGEX=re.compile(r"\[#([0-9]+)\]")



if (__name__=="__main__"):
	util.create_output_dir()
	v=header.read_version("src/sll/include/sll/version.h")
	util.log("Reading Release Changelog...")
	with open("changelog/latest.md","r") as f:
		desc=f.read().replace("\r\n","\n")
	headers={"Accept":"application/vnd.github.v3+json","Authorization":"token "+sys.argv[-1],"Content-Type":"application/json"}
	util.log("Creating Release...")
	sha=requests.get("https://api.github.com/repos/sl-lang/sll/git/ref/heads/main",headers=headers).json()["object"]["sha"]
	r_id=requests.post("https://api.github.com/repos/sl-lang/sll/releases",headers=headers,data=json.dumps({"tag_name":f"sll-v{v[0]}.{v[1]}.{v[2]}","target_commitish":sha,"prerelease":True,"body":desc,"name":f"sll-v{v[0]}.{v[1]}.{v[2]}"})).json()["id"]
	util.log("Uploading Assets...")
	headers["Content-Type"]="application/octet-stream"
	for r,_,fl in os.walk("assets"):
		for fp in fl:
			if (fp[-3:]==".dt"):
				continue
			util.log(f"  Uploading '{os.path.join(r,fp)}'...")
			with open(os.path.join(r,fp),"rb") as f:
				requests.post(f"https://uploads.github.com/repos/sl-lang/sll/releases/{r_id}/assets?name={fp}",headers=headers,data=f.read())
