const MIME_TYPES={
	"css": "text/css;charset=utf-8",
	"deb": "application/octet-stream",
	"gz": "application/gzip",
	"html": "text/html;charset=utf-8",
	"js": "application/javascript;charset=utf-8",
	"zip": "application/zip"
};
const REDIRECTS={
	"/": "/index.html",
	"/sh": "shell_install.sh"
};



const _get_page=async (url)=>{
	const bf=new Uint8Array(await crypto.subtle.digest("SHA-256",new TextEncoder().encode(url)));
	let id="";
	for (let k of bf){
		id+=k.toString(16).padStart(2,"0");
	}
	return await SLL.get(id,{
		type: "stream"
	});
}



addEventListener("fetch",(e)=>e.respondWith((async (url)=>{
	url=REDIRECTS[url]||url;
	const ext=url.split(".").at(-1);
	const headers=new Headers({
		"Content-Type": MIME_TYPES[ext]||"text/plain;charset=utf-8"
	});
	if (!(await SLL.get("__table")).split("\x00").includes(url)){
		return new Response((ext==="html"?await _get_page("not_found.html"):"Not Found"),{
			status: 404,
			statusText: "Not Found",
			headers: headers
		});
	}
	return new Response(await _get_page(url),{
		headers: headers
	});
})("/"+e.request.url.match(/^https?:\/\/[a-z0-9._]+\/+([^?#]*)/)[1])));
