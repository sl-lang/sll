#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/url.h>
#include <sll/util.h>



#define DEFAULT_HTTP_PORT 80
#define STR_LEN(s) (sizeof(s)/sizeof(char)-1)



static char _url_init=0;



__SLL_EXTERNAL void sll_free_header_list(sll_header_list_t* hl){
	for (sll_header_count_t i=0;i<hl->l;i++){
		sll_header_t* kv=*(hl->dt+i);
		sll_deallocate(kv->k.v);
		sll_deallocate(kv->v.v);
		sll_deallocate(*(hl->dt+i));
	}
	sll_deallocate(hl->dt);
	hl->dt=NULL;
	hl->l=0;
}



__SLL_EXTERNAL void sll_free_http_response(sll_http_response_t* r){
	if (r->rc){
		sll_deallocate(r->rc->v);
		SLL_INIT_STRING(r->rc);
	}
	if (r->hl){
		sll_free_header_list(r->hl);
		r->hl->dt=NULL;
		r->dt->l=0;
	}
	if (r->dt){
		sll_deallocate(r->dt->v);
		SLL_INIT_STRING(r->dt);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_url_http_request(const sll_string_t* m,const sll_string_t* h,const sll_string_t* p,const sll_header_list_t* hl,const sll_string_t* dt,sll_http_response_t* o){
	if (o->rc){
		SLL_INIT_STRING(o->rc);
	}
	if (o->hl){
		o->hl->dt=NULL;
		o->dt->l=0;
	}
	if (o->dt){
		SLL_INIT_STRING(o->dt);
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
	sll_copy_data(m->v,m->l,s.v);
	sll_string_length_t i=m->l;
	s.v[i]=' ';
	i++;
	sll_copy_data(p->v,p->l,s.v+i);
	i+=p->l;
	sll_copy_string(SLL_CHAR(" HTTP/1.1"),s.v+i);
	i+=9;
	for (sll_header_count_t j=0;j<hl->l;j++){
		sll_header_t* kv=*(hl->dt+j);
		sll_copy_data(kv->k.v,kv->k.l,s.v+i);
		i+=kv->k.l;
		s.v[i]=':';
		i++;
		sll_copy_data(kv->v.v,kv->v.l,s.v+i);
		i+=kv->v.l;
		s.v[i]='\r';
		s.v[i+1]='\n';
		i+=2;
	}
	sll_copy_string(SLL_CHAR("\r\nHost:"),s.v+i);
	i+=7;
	sll_copy_data(h->v,h->l,s.v+i);
	i+=h->l;
	sll_copy_string(SLL_CHAR("\r\n\r\n"),s.v+i);
	i+=4;
	sll_copy_data(dt->v,dt->l,s.v+i);
	sll_string_t r=SLL_INIT_STRING_STRUCT;
	sll_bool_t err=sll_platform_socket_execute(h,DEFAULT_HTTP_PORT,&s,&r);
	sll_deallocate(s.v);
	if (err==0){
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
	if (sll_string_compare_pointer(r.v,SLL_CHAR("HTTP/1.1"))!=SLL_COMPARE_RESULT_EQUAL){
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
		sll_string_from_pointer_length(r.v+j,i-j,o->rc);
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
			o->hl->dt=sll_reallocate(o->hl->dt,o->hl->l*sizeof(sll_header_t*));
			kv=sll_allocate(sizeof(sll_header_t));
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
			sll_string_from_pointer_length(r.v+j,i-j,&(kv->k));
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
			sll_string_from_pointer_length(r.v+j,i-j,&(kv->v));
		}
	}
	i+=4;
	if (o->dt){
		sll_string_from_pointer_length(r.v+i,r.l-i,o->dt);
	}
	sll_deallocate(r.v);
	return rc;
_error:
	sll_deallocate(r.v);
	return -1;
}
