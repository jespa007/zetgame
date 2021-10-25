#include "zg_base.h"

BufferByte *BufferByte_New(size_t len){
	BufferByte *buffer=NEW(BufferByte);
	buffer->ptr=(uint8_t *)malloc(sizeof(uint8_t)*len);
	buffer->len=len;
	memset(buffer->ptr,0,len);
	return buffer;
}

void BufferByte_Delete(BufferByte *buffer){
	if(buffer!=NULL){
		if(buffer->ptr!=NULL){
			FREE(buffer->ptr);
		}
		FREE(buffer);
	}
}
