#ifndef __ZG_MEMORY_H__
#define __ZG_MEMORY_H__


#define NEW(S) ((S*)zg_malloc(sizeof(S),__FILE__,__LINE__))
#define FREE(s) free(s)

void * zg_malloc(size_t size,const char *file, int line);

#endif
