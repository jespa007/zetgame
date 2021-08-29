#ifndef __MAP_STRING_H__
#define __MAP_STRING_H__


typedef struct MapString MapString;
typedef struct MapStringNode MapStringNode;

struct MapStringNode{
    char  *key;
    void * val;
    MapStringNode *next;
    MapStringNode *previous;
};

struct MapString{
	uint16_t count;
	MapStringNode **list;
	void  (* on_delete)(MapStringNode *node);
};

MapString * MapString_New(void);
bool		MapString_Exist(MapString *_this,const char * key);
void 		MapString_SetValue(MapString *_this,const char * key,void * val);
void 		MapString_Erase(MapString *_this,const char * key);
void * 		MapString_GetValue(MapString *_this,const char * key, bool *exists);
void		MapString_Clear(MapString *_this);
void 		MapString_Delete(MapString *_this);



#endif
