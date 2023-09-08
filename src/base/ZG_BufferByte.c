#include "@zg_base.h"

ZG_BufferByte *ZG_BufferByte_New(size_t len){
	ZG_BufferByte *buffer=ZG_NEW(ZG_BufferByte);
	buffer->ptr=(uint8_t *)malloc(sizeof(uint8_t)*len);
	buffer->len=len;
	memset(buffer->ptr,0,len);
	return buffer;
}

void ZG_BufferByte_Delete(ZG_BufferByte *buffer){
	if(buffer!=NULL){
		if(buffer->ptr!=NULL){
			ZG_FREE(buffer->ptr);
		}
		ZG_FREE(buffer);
	}
}
