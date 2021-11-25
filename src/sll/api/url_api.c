#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/url.h>



__API_FUNC(url_execute_request){
	sll_header_list_t http_m={
		sll_allocate(d->l*sizeof(sll_header_t*)),
		d->l
	};
	for (sll_header_count_t i=0;i<d->l;i++){
		sll_header_t* h_kv=sll_allocate(sizeof(sll_header_t));
		SLL_ASSERT(SLL_OBJECT_GET_TYPE(d->v[i<<1])==SLL_OBJECT_TYPE_STRING);
		h_kv->k=d->v[i<<1]->dt.s;
		SLL_ASSERT(SLL_OBJECT_GET_TYPE(d->v[(i<<1)+1])==SLL_OBJECT_TYPE_STRING);
		h_kv->v=d->v[(i<<1)+1]->dt.s;
		*(http_m.dt+i)=h_kv;
	}
	sll_object_t* rc_s=SLL_CREATE();
	rc_s->t=SLL_OBJECT_TYPE_STRING;
	sll_header_list_t hl;
	sll_object_t* dt_s=SLL_CREATE();
	dt_s->t=SLL_OBJECT_TYPE_STRING;
	sll_http_response_t r={
		&(rc_s->dt.s),
		&hl,
		&(dt_s->dt.s)
	};
	sll_return_code_t rc=sll_url_http_request(a,b,c,&http_m,e,&r);
	for (sll_header_count_t i=0;i<d->l;i++){
		sll_deallocate(*(http_m.dt+i));
	}
	sll_deallocate(http_m.dt);
	sll_object_t* hl_m=SLL_CREATE();
	hl_m->t=SLL_OBJECT_TYPE_MAP;
	sll_map_create(hl.l,&(hl_m->dt.m));
	for (sll_map_length_t i=0;i<hl.l;i++){
		sll_header_t* h_kv=*(hl.dt+i);
		sll_object_t* k=SLL_CREATE();
		k->t=SLL_OBJECT_TYPE_STRING;
		k->dt.s=h_kv->k;
		hl_m->dt.m.v[i<<1]=k;
		sll_object_t* v=SLL_CREATE();
		v->t=SLL_OBJECT_TYPE_STRING;
		v->dt.s=h_kv->v;
		hl_m->dt.m.v[(i<<1)+1]=v;
		sll_deallocate(h_kv);
	}
	sll_deallocate(hl.dt);
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(4,&(o->dt.a));
	o->dt.a.v[0]=SLL_FROM_INT(rc);
	o->dt.a.v[1]=rc_s;
	o->dt.a.v[2]=hl_m;
	o->dt.a.v[3]=dt_s;
	return o;
}
