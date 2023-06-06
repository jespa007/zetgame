#ifndef __ZG_BUFFER_SHORT_H__
#define __ZG_BUFFER_SHORT_H__

typedef struct ZG_BufferShort ZG_BufferShort;

struct ZG_BufferShort{
	uint16_t * data;
	size_t len;
};


ZG_BufferShort *ZG_BufferShort_New(size_t len);
void ZG_BufferShort_Delete(ZG_BufferShort *buffer);

#endif
