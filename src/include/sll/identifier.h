#ifndef __SLL_IDENTIFIER_H__
#define __SLL_IDENTIFIER_H__ 1



#define SLL_MAX_SHORT_IDENTIFIER_LENGTH 15
#define SLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)
#define SLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)
#define SLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))
#define SLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))



#endif
