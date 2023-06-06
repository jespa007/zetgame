#ifndef __ZG_MAP_INT_H__
#define __ZG_MAP_INT_H__

typedef struct ZG_MapInt ZG_MapInt;
typedef struct ZG_MapIntNode ZG_MapIntNode;

struct ZG_MapIntNode{
    intptr_t	 key;
    void 		*val;
};

struct ZG_MapInt{
	ZG_List	 	*	list;
	void 		 	(* on_delete)(ZG_MapIntNode *node);

};

ZG_MapInt * 	ZG_MapInt_New(void);
bool		ZG_MapInt_Exist(ZG_MapInt *_this,intptr_t key);
void 		ZG_MapInt_Set(ZG_MapInt *_this,intptr_t key,void * val);
void * 		ZG_MapInt_Get(ZG_MapInt *_this,intptr_t key);
void 		ZG_MapInt_Erase(ZG_MapInt *_this,intptr_t key);
void 		ZG_MapInt_EraseAndFreeItem(ZG_MapInt *_this,intptr_t key);
void 		ZG_MapInt_Clear(ZG_MapInt *_this);
void 		ZG_MapInt_ClearAndFreeAllItems(ZG_MapInt *_this);
//void 		ZG_MapInt_ClearAndFreeAllItems(ZG_MapInt *m, bool delete_value);
void 		ZG_MapInt_Delete(ZG_MapInt *_this);
void 		ZG_MapInt_DeleteAndFreeAllItems(ZG_MapInt *_this);


#endif
