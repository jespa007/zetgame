#ifndef __BUFFER_FLOAT_H__
#define __BUFFER_FLOAT_H__

typedef struct BufferFloat BufferFloat;

struct BufferFloat{
	float * data;
	size_t len;
};


BufferFloat *BufferFloat_New(size_t len);
void BufferFloat_Delete(BufferFloat *buffer);

#endif
