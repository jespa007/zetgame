#ifndef __ZG_BUFFER_FLOAT_H__
#define __ZG_BUFFER_FLOAT_H__

typedef struct ZG_BufferFloat ZG_BufferFloat;

struct ZG_BufferFloat{
	float * data;
	size_t len;
};


ZG_BufferFloat *ZG_BufferFloat_New(size_t len);
void ZG_BufferFloat_Delete(ZG_BufferFloat *buffer);

#endif
