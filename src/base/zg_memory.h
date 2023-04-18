#ifndef __ZG_MEMORY_H__
#define __ZG_MEMORY_H__


#define ZG_NEW(S) ((S*)zg_malloc(sizeof(S),__FILE__,__LINE__))
#define ZG_FREE(s) {free(s);s=NULL;}

void * zg_malloc(size_t size,const char *file, int line);

#endif
