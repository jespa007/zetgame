#ifndef __MAP_STRING_ITERATOR_H__
#define __MAP_STRING_ITERATOR_H__


typedef struct MapStringIterator MapStringIterator;

struct MapStringIterator{
	void *data;
};



MapStringIterator*	MapStringIterator_New(MapString *map);
void				MapStringIterator_Next(MapStringIterator *_this);
bool 				MapStringIterator_End(MapStringIterator *_this);
const char 			*MapStringIterator_GetKey(MapStringIterator *_this);
void * 				MapStringIterator_GetValue(MapStringIterator *_this);
void				MapStringIterator_Delete(MapStringIterator *_this);

#endif
