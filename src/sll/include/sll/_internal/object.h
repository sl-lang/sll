#ifndef __SLL__INTERNAL_OBJECT_H__
#define __SLL__INTERNAL_OBJECT_H__ 1



#define OBJECT_TYPE_DATA_ENTRY_FLAG_CONST 1
#define OBJECT_TYPE_DATA_ENTRY_INIT 0xfffffffe
#define OBJECT_TYPE_DATA_ENTRY_OFFSET_EMPTY 0x7fffffff

#define OBJECT_TYPE_DATA_ENTRY_GET_OFFSET(e) ((e)->_off>>1)
#define OBJECT_TYPE_DATA_ENTRY_SET_OFFSET(e,off) ((e)->_off=((off)<<1)|((e)->_off&1))



#endif
