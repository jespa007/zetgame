#include "_zg_base_.h"

ZG_BufferShort *ZG_BufferShort_New(size_t len){
	ZG_BufferShort *buffer=ZG_NEW(ZG_BufferShort);
	buffer->data=(uint16_t *)malloc(sizeof(uint16_t)*len);
	buffer->len=len;
	memset(buffer->data,0,len);
	return buffer;
}

void ZG_BufferShort_Delete(ZG_BufferShort *buffer){
	if(buffer!=NULL){
		if(buffer->data!=NULL){
			ZG_FREE(buffer->data);
		}
		ZG_FREE(buffer);
	}
}
