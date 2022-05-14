import header
import os
import util



HASHLIST_EXTENSIONS=[".c",".h",".asm"]



hash_list={}
update_map={}



def _check(fp,inc):
	nm=util.output_file_path(fp)
	if (nm in update_map):
		return update_map[nm]
	if (fp[-2:]!=".h" and not os.path.exists(util.output_file_path(fp))):
		update_map[nm]=1
		return 1
	with open(fp,"r") as rf:
		for e in header.INCLUDE_REGEX.findall(rf.read()):
			e=e[1:-1]
			if (e[:4]!="sll/"):
				continue
			for k in inc:
				f_fp=os.path.join(k,e)
				if (not os.path.exists(f_fp) or _check(f_fp,inc)):
					update_map[nm]=1
					return 1
	update_map[nm]=0
	return 0



def _flush_data():
	with open(load_hash_list.source_file_path,"w") as f:
		for k,v in hash_list.items():
			f.write(k+v+"\n")



def fail(fp):
	fp=util.output_file_path(fp)
	if (fp in hash_list):
		del hash_list[fp]
		_flush_data()



def load_hash_list(fp):
	util.log(f"Loading file hashes from '{fp}'...")
	load_hash_list.source_file_path=fp
	hash_list.clear()
	update_map.clear()
	if (os.path.exists(fp)):
		with open(fp,"r") as f:
			for k in f.read().split("\n"):
				k=k.strip()
				if (len(k)==0):
					continue
				hash_list[k[:-64]]=k[-64:]
	util.log(f"  Loaded {len(hash_list)} hashes")
	util.log("Checking hashes...")
	for r,_,fl in os.walk("src"):
		for f in fl:
			if (f[f.rindex("."):] in HASHLIST_EXTENSIONS):
				fp=os.path.join(r,f)
				h=util.hash_file(fp)[1]
				nm=util.output_file_path(fp)
				if (nm not in hash_list or hash_list[nm]!=h):
					hash_list[nm]=h
					update_map[nm]=1
	_flush_data()



def update(fp,*inc):
	if (_check(fp,inc)):
		_flush_data()
		return True
	return False
