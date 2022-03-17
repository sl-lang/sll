const MIME_TYPES={
	".css": "text/css;charset=utf-8",
	".deb": "application/octet-stream",
	".gz": "application/gzip",
	".html": "text/html;charset=utf-8",
	".js": "application/javascript;charset=utf-8",
	".zip": "application/zip"
};
const REDIRECTS={
	"/": "/index.html",
	"/sh": "shell_install.sh"
};



addEventListener("fetch",(e)=>e.respondWith((async (url)=>{
	url=REDIRECTS[url]||url;
	const opt={
		status: 200,
		statusText: "OK",
		headers: new Headers({
			"Content-Type": MIME_TYPES["."+url.split("/").at(-1).split(".").at(-1)]||"text/plain;charset=utf-8",
			"Referrer-Policy": "no-referrer",
			"Strict-Transport-Security": "max-age=31536000; includeSubDomains",
			"X-Content-Type-Options": "nosniff"
		})
	};
	if (!(await SLL.get("__links")).split("\n").includes(url)){
		opt.status=404;
		opt.statusText="Not Found";
		url="not_found.html";
	}
	const bf=new Uint8Array(await crypto.subtle.digest("SHA-256",new TextEncoder().encode(url)));
	let id="";
	for (let k of bf){
		id+=k.toString(16).padStart(2,"0");
	}
	return new Response(await SLL.get(id,{
		type: "stream"
	}),opt);
})("/"+e.request.url.match(/^https?:\/\/[a-z0-9._]+\/+([^?#]*)/)[1])));
