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
bool		MapInt_Exist(MapInt *t,intptr_t key);
void 		MapInt_Set(MapInt *t,intptr_t key,void * val);
void * 		MapInt_Get(MapInt *t,intptr_t key);
void 		MapInt_Erase(MapInt *t,intptr_t key);
void 		MapInt_EraseAndFreeItem(MapInt *t,intptr_t key);
void 		MapInt_Clear(MapInt *t, bool _free_all_items);
//void 		MapInt_ClearAndFreeAllItems(MapInt *m, bool delete_value);
void 		MapInt_Delete(MapInt *m, bool _free_all_items);
//void 		MapInt_DeleteAndFreeAllItems(MapInt *m);


#endif
