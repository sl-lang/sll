import header
import os
import util



hash_list={}
update_list=set()



def _check(fp):
	nm=util.unique_file_path(fp)
	if (nm in update_list or (fp[-2:]==".c" and not os.path.exists(util.output_file_path(fp)))):
		return 1
	hash_str=util.hash_file(fp)[1]
	old=(None if nm not in hash_list else hash_list[nm])
	hash_list[nm]=hash_str
	if (old!=hash_str):
		update_list.add(nm)
		return 1
	return 0



def _flush_data():
	with open(load_hash_list.source_file_path,"w") as f:
		for k,v in hash_list.items():
			f.write(k+v+"\n")



def fail(fp):
	fp=util.unique_file_path(fp)
	if (fp in hash_list):
		del hash_list[fp]
		_flush_data()



def load_hash_list(fp):
	util.log(f"Loading file hashes from '{fp}'...")
	load_hash_list.source_file_path=fp
	hash_list.clear()
	update_list.clear()
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
	if (not o):
		with open(fp,"rb") as rf:
			for e in header.INCLUDE_REGEX.findall(rf.read()):
				e=str(e[1:-1],"utf-8")
				for k in inc:
					nm=os.path.join(k,e)
					if (os.path.exists(nm) and _check(nm)):
						o=1
						break
				if (o):
					break
	if (o):
		_flush_data()
		return True
	return False
