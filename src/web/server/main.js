async function _read_url(url){
	url="/"+url.match(/^\/*([^?#]*)/)[1];
	if (url=="/sh"){
		url="shell_install.sh";
	}
	else if (url.endsWith("/")){
		url+="index.html"
	}
	else if (!url.startsWith("/apt/")){
		const s=url.split("/");
		if (s[s.length-1].indexOf(".")==-1){
			url+=".html";
		}
	}
	let dt=await SLL.get(url,"arrayBuffer");
	if (!dt){
		url="404.html"
		dt=await SLL.get("/404.html","arrayBuffer");
	}
	r=new Response(dt);
	url=url.split(".");
	let ct="text/plain";
	switch (url[url.length-1]){
		case "css":
			ct="text/css";
			break;
		case "gz":
			ct="application/gzip";
			break;
		case "html":
			ct="text/html";
			break;
		case "js":
			ct="application/javascript";
			break;
	}
	r.headers.set("Content-Type",ct+";charset=utf-8");
	return r;
}



addEventListener("fetch",(e)=>{
	try{
		return e.respondWith(_read_url(new URL(e.request.url).pathname));
	}
	catch (err){
		return e.respondWith(new Response("Internal Error",{status:500}));
	}
});
