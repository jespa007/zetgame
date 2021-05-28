#ifndef __MAP_STRING_H__
#define __MAP_STRING_H__


typedef struct MapString MapString;
typedef struct MapStringNode MapStringNode;

struct MapStringNode{
    char  *key;
    void * val;
    MapStringNode *next;
};

struct MapString{
	uint16_t count;
	MapStringNode **list;
	void * (* on_delete)(MapStringNode *node);
};

MapString * MapString_New(void);
bool		MapString_Exist(MapString *t,const char * key);
void 		MapString_SetValue(MapString *t,const char * key,void * val);
void * 		MapString_GetValue(MapString *t,const char * key);
void 		MapString_Delete(MapString *m);



#endif
