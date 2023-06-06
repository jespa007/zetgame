#include "_zg_base_.h"

ZG_BufferFloat *ZG_BufferFloat_New(size_t len){
	ZG_BufferFloat *buffer=ZG_NEW(ZG_BufferFloat);
	buffer->data=(float *)malloc(sizeof(float)*len);
	buffer->len=len;
	memset(buffer->data,0,len);
	return buffer;
}

void ZG_BufferFloat_Delete(ZG_BufferFloat *buffer){
	if(buffer!=NULL){
		if(buffer->data!=NULL){
			ZG_FREE(buffer->data);
		}
		ZG_FREE(buffer);
	}
}
