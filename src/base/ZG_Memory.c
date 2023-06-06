#include "_zg_base_.h"

void * zg_malloc(size_t size,const char *file, int line){
	void * p=NULL;
#ifdef __MEMMANAGER__
	p=MEMMGR_malloc(size,file,line);
#else
	UNUSUED_PARAM(file);
	UNUSUED_PARAM(line);
	p=malloc(size);
#endif

	if(p==NULL){
		ZG_Log_Error("Cannot allocate %i",size);
		return NULL;
	}

	memset(p,0,size);

	return p;
}
