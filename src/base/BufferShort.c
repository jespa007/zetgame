#include "zg_base.h"

BufferShort *BufferShort_New(size_t len){
	BufferShort *buffer=NEW(BufferShort);
	buffer->data=(uint16_t *)malloc(sizeof(uint16_t)*len);
	buffer->len=len;
	memset(buffer->data,0,len);
	return buffer;
}

void BufferShort_Delete(BufferShort *buffer){
	if(buffer!=NULL){
		if(buffer->data!=NULL){
			FREE(buffer->data);
		}
		FREE(buffer);
	}
}
