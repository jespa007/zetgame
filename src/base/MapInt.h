#ifndef __MAP_INT_H__
#define __MAP_INT_H__

typedef struct MapInt MapInt;
typedef struct MapIntNode MapIntNode;

struct MapIntNode{
    intptr_t	 key;
    void 		*val;
};

struct MapInt{
	List	 	*	list;
	void 		 	(* on_delete)(MapIntNode *node);

};

MapInt * 	MapInt_New(void);
bool		MapInt_Exist(MapInt *_this,intptr_t key);
void 		MapInt_Set(MapInt *_this,intptr_t key,void * val);
void * 		MapInt_Get(MapInt *_this,intptr_t key);
void 		MapInt_Erase(MapInt *_this,intptr_t key);
void 		MapInt_EraseAndFreeItem(MapInt *_this,intptr_t key);
void 		MapInt_Clear(MapInt *_this);
void 		MapInt_ClearAndFreeAllItems(MapInt *_this);
//void 		MapInt_ClearAndFreeAllItems(MapInt *m, bool delete_value);
void 		MapInt_Delete(MapInt *_this);
void 		MapInt_DeleteAndFreeAllItems(MapInt *_this);


#endif
