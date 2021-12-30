const MIME_TYPES={
	"css": "text/css;charset=utf-8",
	"deb": "application/octet-stream",
	"dll": "application/octet-stream",
	"gz": "application/gzip",
	"html": "text/html;charset=utf-8",
	"js": "application/javascript;charset=utf-8",
	"so": "application/octet-stream",
	"zip": "application/zip"
};
const REDIRECTS={
	"/": "/index.html",
	"/sh": "shell_install.sh"
};



addEventListener("fetch",(e)=>e.respondWith((async (url)=>{
	url=REDIRECTS[url]||url;
	let h={};
	if (!(await SLL.get("__table")).split("\x00").includes(url)){
		h.status=404;
		h.statusText="Not Found";
		url="not_found.html"
	}
	h.headers=new Headers({
		"Content-Type":MIME_TYPES[url.split(".").at(-1)]||"text/plain;charset=utf-8"
	});
	return new Response(await SLL.get(Array.from(new Uint8Array(await crypto.subtle.digest("SHA-256",new TextEncoder().encode(url)))).map(k=>k.toString(16).padStart(2,"0")).join(""),"arrayBuffer"),h);
})("/"+e.request.url.match(/^https?:\/\/[a-z0-9._]+\/+([^?#]*)/)[1])));
