#ifndef __ZG_MAP_STRING_H__
#define __ZG_MAP_STRING_H__


typedef struct ZG_MapString ZG_MapString;
typedef struct ZG_MapStringNode ZG_MapStringNode;

struct ZG_MapStringNode{
    char  *key;
    void * val;
    ZG_MapStringNode *next;
    ZG_MapStringNode *previous;
};

struct ZG_MapString{
	uint16_t count;
	ZG_MapStringNode **list;
	void  (* on_delete)(ZG_MapStringNode *node);
};

ZG_MapString * ZG_MapString_New(void);
bool		ZG_MapString_Exist(ZG_MapString *_this,const char * key);
void 		ZG_MapString_SetValue(ZG_MapString *_this,const char * key,void * val);
void 		ZG_MapString_Erase(ZG_MapString *_this,const char * key);
void * 		ZG_MapString_GetValue(ZG_MapString *_this,const char * key, bool *exists);
void		ZG_MapString_Clear(ZG_MapString *_this);
void 		ZG_MapString_Delete(ZG_MapString *_this);



#endif
