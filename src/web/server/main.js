const MIME_TYPES={"css":"text/css;charset=utf-8","deb":"application/octet-stream","dll":"application/octet-stream","gz":"application/gzip","html":"text/html;charset=utf-8","js":"application/javascript;charset=utf-8","so":"application/octet-stream","zip":"application/zip"};
const REDIRECTS={"/":"/index.html","/sh":"shell_install.sh"};



addEventListener("fetch",(e)=>{
	return e.respondWith((async _=>{
		let url="/"+e.request.url.toLowerCase().match(/^https?:\/\/[a-z0-9._]+\/+([^?#]*)/)[1];
		const table=await SLL.get("__table","json");
		url=REDIRECTS[url]||(table.includes(url)?url:"/404.html");
		return new Response(await SLL.get(Array.from(new Uint8Array(await crypto.subtle.digest("SHA-1",new TextEncoder().encode(url)))).map(e=>{
			return e.toString(16).padStart(2,"0");
		}).join(""),"arrayBuffer"),{headers:new Headers({
			"Content-Type":MIME_TYPES[url.split(".").at(-1)]||"text/plain;charset=utf-8"
		})});
	})());
});
