#ifndef __BUFFER_SHORT_H__
#define __BUFFER_SHORT_H__

typedef struct BufferShort BufferShort;

struct BufferShort{
	uint16_t * data;
	size_t len;
};


BufferShort *BufferShort_New(size_t len);
void BufferShort_Delete(BufferShort *buffer);

#endif
