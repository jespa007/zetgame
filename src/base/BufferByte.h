#ifndef __BUFFER_BYTE_H__
#define __BUFFER_BYTE_H__

typedef struct BufferByte BufferByte;

struct BufferByte{
	uint8_t * ptr;
	size_t len;
};


BufferByte *BufferByte_New(size_t len);
void BufferByte_Delete(BufferByte *buffer);

#endif
