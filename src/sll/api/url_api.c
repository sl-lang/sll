#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/map.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/url.h>
#include <stdlib.h>



__API_FUNC(url_execute_request){
	sll_header_list_t http_m={
		malloc((d->l>>1)*sizeof(sll_header_t*)),
		d->l>>1
	};
	for (sll_header_count_t i=0;i<(d->l>>1);i++){
		sll_header_t* h_kv=malloc(sizeof(sll_header_t));
		SLL_ASSERT(d->v[i<<1]->t==SLL_RUNTIME_OBJECT_TYPE_STRING);
		h_kv->k=d->v[i<<1]->dt.s;
		SLL_ASSERT(d->v[(i<<1)+1]->t==SLL_RUNTIME_OBJECT_TYPE_STRING);
		h_kv->v=d->v[(i<<1)+1]->dt.s;
		*(http_m.dt+i)=h_kv;
	}
	sll_runtime_object_t* rc_s=SLL_CREATE();
	rc_s->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_header_list_t hl;
	sll_runtime_object_t* dt_s=SLL_CREATE();
	dt_s->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_http_response_t r={
		&(rc_s->dt.s),
		&hl,
		&(dt_s->dt.s)
	};
	sll_return_code_t rc=sll_url_http_request(a,b,c,&http_m,e,&r);
	for (sll_header_count_t i=0;i<(d->l>>1);i++){
		free(*(http_m.dt+i));
	}
	free(http_m.dt);
	sll_runtime_object_t* hl_m=SLL_CREATE();
	hl_m->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
	sll_map_create(hl.l<<1,&(hl_m->dt.m));
	for (sll_map_length_t i=0;i<hl.l;i++){
		sll_header_t* h_kv=*(hl.dt+i);
		sll_runtime_object_t* k=SLL_CREATE();
		k->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		k->dt.s=h_kv->k;
		hl_m->dt.m.v[i<<1]=k;
		sll_runtime_object_t* v=SLL_CREATE();
		v->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		v->dt.s=h_kv->v;
		hl_m->dt.m.v[(i<<1)+1]=v;
		free(h_kv);
	}
	free(hl.dt);
	sll_runtime_object_t* o=SLL_CREATE();
	o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
	sll_array_create(4,&(o->dt.a));
	o->dt.a.v[0]=SLL_FROM_INT(rc);
	o->dt.a.v[1]=rc_s;
	o->dt.a.v[2]=hl_m;
	o->dt.a.v[3]=dt_s;
	return o;
}
