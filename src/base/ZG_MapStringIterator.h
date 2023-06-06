#ifndef __ZG_MAP_STRING_ITERATOR_H__
#define __ZG_MAP_STRING_ITERATOR_H__


typedef struct ZG_MapStringIterator ZG_MapStringIterator;

struct ZG_MapStringIterator{
	void *data;
};



ZG_MapStringIterator*	ZG_MapStringIterator_New(ZG_MapString *map);
void				ZG_MapStringIterator_Next(ZG_MapStringIterator *_this);
bool 				ZG_MapStringIterator_End(ZG_MapStringIterator *_this);
const char 			*ZG_MapStringIterator_GetKey(ZG_MapStringIterator *_this);
void * 				ZG_MapStringIterator_GetValue(ZG_MapStringIterator *_this);
void				ZG_MapStringIterator_Delete(ZG_MapStringIterator *_this);

#endif
