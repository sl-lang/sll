import hashlib
import os
import util



global hash_list,source_file_path,update_list
hash_list=None
source_file_path=None
update_list=None



def _check(fp):
	nm=util.unique_file_path(fp)
	if (nm in update_list or fp[-2:]==".c" and not os.path.exists(util.output_file_path(fp))):
		return 1
	sha256=hashlib.sha256()
	with open(fp,"rb") as rf:
		while (True):
			c=rf.read(4096)
			if (len(c)==0):
				break
			sha256.update(c)
	hash_str=sha256.hexdigest()
	old=(None if nm not in hash_list else hash_list[nm])
	hash_list[nm]=hash_str
	if (old!=hash_str):
		update_list.add(nm)
		return 1
	return 0



def fail(fp):
	fp=util.unique_file_path(fp)
	if (fp in hash_list):
		del hash_list[fp]



def load_hash_list(fp):
	global hash_list,source_file_path,update_list
	util.log(f"Loading file hashes from '{fp}'...")
	source_file_path=fp
	hash_list={}
	update_list=set()
	if (os.path.exists(fp)):
		with open(fp,"r") as f:
			for k in f.read().split("\n"):
				k=k.strip()
				if (len(k)==0):
					continue
				hash_list[k[:-64]]=k[-64:]
	util.log(f"  Loaded {len(hash_list)} hashes")



def update(fp,*inc):
	o=_check(fp)
	for k in inc:
		for r,_,fl in os.walk(k):
			for f in fl:
				if (f[-2:]==".h"):
					o|=_check(os.path.join(r,f))
	if (o):
		with open(source_file_path,"w") as f:
			for k,v in hash_list.items():
				f.write(f"{k}{v}\n")
		return True
	return False
