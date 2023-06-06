#ifndef __ZG_BUFFER_BYTE_H__
#define __ZG_BUFFER_BYTE_H__

typedef struct ZG_BufferByte ZG_BufferByte;

struct ZG_BufferByte{
	uint8_t * ptr;
	size_t len;
};


ZG_BufferByte *ZG_BufferByte_New(size_t len);
void ZG_BufferByte_Delete(ZG_BufferByte *buffer);

#endif
