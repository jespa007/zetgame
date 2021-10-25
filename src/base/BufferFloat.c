#include "zg_base.h"

BufferFloat *BufferFloat_New(size_t len){
	BufferFloat *buffer=NEW(BufferFloat);
	buffer->data=(float *)malloc(sizeof(float)*len);
	buffer->len=len;
	memset(buffer->data,0,len);
	return buffer;
}

void BufferFloat_Delete(BufferFloat *buffer){
	if(buffer!=NULL){
		if(buffer->data!=NULL){
			FREE(buffer->data);
		}
		FREE(buffer);
	}
}
