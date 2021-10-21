#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/url.h>
#include <sll/util.h>
#include <stdlib.h>
#include <string.h>



#define DEFAULT_HTTP_PORT 80
#define STR_LEN(s) (sizeof(s)/sizeof(char)-1)



static char _url_init=0;



__SLL_FUNC void sll_free_header_list(sll_header_list_t* hl){
	for (sll_header_count_t i=0;i<hl->l;i++){
		sll_header_t* kv=*(hl->dt+i);
		free(kv->k.v);
		free(kv->v.v);
		free(*(hl->dt+i));
	}
	free(hl->dt);
	hl->dt=NULL;
	hl->l=0;
}



__SLL_FUNC void sll_free_http_response(sll_http_response_t* r){
	if (r->rc){
		free(r->rc->v);
		SLL_ZERO_STRING(r->rc);
	}
	if (r->hl){
		sll_free_header_list(r->hl);
		SLL_ZERO_HEADER_LIST(r->hl);
	}
	if (r->dt){
		free(r->dt->v);
		SLL_ZERO_STRING(r->dt);
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_code_t sll_url_http_request(const sll_string_t* m,const sll_string_t* h,const sll_string_t* p,const sll_header_list_t* hl,const sll_string_t* dt,sll_http_response_t* o){
	if (o->rc){
		SLL_ZERO_STRING(o->rc);
	}
	if (o->hl){
		SLL_ZERO_HEADER_LIST(o->hl);
	}
	if (o->dt){
		SLL_ZERO_STRING(o->dt);
	}
	if (!_url_init){
		_url_init=1;
		sll_platform_socket_init();
	}
	sll_string_length_t sz=m->l+p->l+h->l+dt->l+STR_LEN("  HTTP/1.1\r\nHost:\r\n\r\n");
	for (sll_header_count_t i=0;i<hl->l;i++){
		sll_header_t* kv=*(hl->dt+i);
		sz+=kv->k.l+kv->v.l+STR_LEN("\r\n ");
	}
	sll_string_t s;
	sll_string_create(sz,&s);
	memcpy(s.v,m->v,m->l);
	sll_string_length_t i=m->l;
	s.v[i]=' ';
	i++;
	memcpy(s.v+i,p->v,p->l);
	i+=p->l;
	memcpy(s.v+i," HTTP/1.1",9);
	i+=9;
	for (sll_header_count_t j=0;j<hl->l;j++){
		sll_header_t* kv=*(hl->dt+j);
		memcpy(s.v+i,kv->k.v,kv->k.l);
		i+=kv->k.l;
		s.v[i]=':';
		i++;
		memcpy(s.v+i,kv->v.v,kv->v.l);
		i+=kv->v.l;
		s.v[i]='\r';
		s.v[i+1]='\n';
		i+=2;
	}
	memcpy(s.v+i,"\r\nHost:",7);
	i+=7;
	memcpy(s.v+i,h->v,h->l);
	i+=h->l;
	memcpy(s.v+i,"\r\n\r\n",4);
	i+=4;
	memcpy(s.v+i,dt->v,dt->l);
	sll_string_t r=SLL_ZERO_STRING_STRUCT;
	sll_return_t err=sll_platform_socket_execute(h,DEFAULT_HTTP_PORT,&s,&r);
	free(s.v);
	if (err==SLL_RETURN_ERROR){
		goto _error;
	}
	i=0;
	while (r.v[i]!=' '){
		if (!r.v[i]){
			goto _error;
		}
		i++;
	}
	r.v[i]=0;
	if (strcmp((char*)r.v,"HTTP/1.1")){
		goto _error;
	}
	sll_return_code_t rc=((sll_return_code_t)r.v[i+1])*100+((sll_return_code_t)r.v[i+2])*10+r.v[i+3]-5328;
	if (rc<100||rc>999){
		goto _error;
	}
	i+=4;
	if (r.v[i]!=' '){
		goto _error;
	}
	i++;
	if (o->rc){
		sll_string_length_t j=i;
		while (r.v[i]!='\r'||r.v[i+1]!='\n'){
			if (!r.v[i]){
				goto _error;
			}
			i++;
		}
		sll_string_create(i-j,o->rc);
		memcpy(o->rc->v,r.v+j,i-j);
		sll_string_calculate_checksum(o->rc);
	}
	else{
		while (r.v[i]!='\r'||r.v[i+1]!='\n'){
			if (!r.v[i]){
				goto _error;
			}
			i++;
		}
	}
	while (r.v[i]!='\r'||r.v[i+1]!='\n'||r.v[i+2]!='\r'||r.v[i+3]!='\n'){
		SLL_ASSERT(r.v[i]=='\r'&&r.v[i+1]=='\n');
		i+=2;
		sll_header_t* kv=NULL;
		if (o->hl){
			o->hl->l++;
			o->hl->dt=realloc(o->hl->dt,o->hl->l*sizeof(sll_header_t*));
			kv=malloc(sizeof(sll_header_t));
			*(o->hl->dt+o->hl->l-1)=kv;
		}
		sll_string_length_t j=i;
		while (r.v[i]!=':'){
			if (!r.v[i]||r.v[i]=='\r'||r.v[i]=='\n'){
				goto _error;
			}
			if (r.v[i]>64&&r.v[i]<91){
				r.v[i]+=32;
			}
			i++;
		}
		if (kv){
			sll_string_create(i-j,&(kv->k));
			memcpy(kv->k.v,r.v+j,i-j);
		}
		do{
			i++;
		} while (r.v[i]==' ');
		j=i;
		while (r.v[i]!='\r'||r.v[i+1]!='\n'){
			if (!r.v[i]){
				goto _error;
			}
			i++;
		}
		if (kv){
			sll_string_create(i-j,&(kv->v));
			memcpy(kv->v.v,r.v+j,i-j);
		}
	}
	i+=4;
	if (o->dt){
		sll_string_create(r.l-i,o->dt);
		memcpy(o->dt->v,r.v+i,r.l-i);
	}
	free(r.v);
	return rc;
_error:
	free(r.v);
	return -1;
}
