import platform
import sys
import zipfile



with zipfile.ZipFile(platform.system().lower()+".zip","w",compression=zipfile.ZIP_DEFLATED) as zf:
	for k in sys.argv[1:]:
		zf.write(k,arcname=k[6:])
