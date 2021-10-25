#include "zg_base.h"

MapInt *MapInt_New(){
	MapInt *t = NEW(MapInt);
	memset(t,0,sizeof(MapInt));
	t->list=List_New();
    return t;
}

int MapString_GetIdx(MapInt *t,intptr_t  key){
	List *l=t->list;
	intptr_t idx_min=0;
	intptr_t idx_max=l->count-1;//n_allocated_pointers-1;

	if(idx_max==0){
		return ZG_ERROR;
	}
	// continue searching while [imin,imax] is not empty
	while (idx_max >= idx_min)
	{
		// calculate the midpoint for roughly equal partition
		int idx_mid = (idx_min + idx_max ) >> 1;
		MapIntNode * node = (MapIntNode *)l->items[idx_mid];
		if(node->key == key){
			// key found at index idx_mid
			return idx_mid;
		// determine which subarray to search
		}else if (node->key < key){
			// change min index to search upper subarray
			idx_min = idx_mid + 1;
		}else{
			// change max index to search lower subarray
			idx_max = idx_mid - 1;
		}
    }
	// key was not found
	return ZG_ERROR;
}

int MapString_GetIdxToInsert(MapInt *t,intptr_t  key){
	// PRE: array is already ordered
	List *l=t->list;
	intptr_t size=l->count - 1;
	intptr_t idx_min = 0, idx_max = size;
	MapIntNode *max_node=NULL;

	if(size<0){ // no items...
		return 0;
	}

	max_node=l->items[idx_max];

	if (key > max_node->key){
	  idx_min=size;
	}
	else{
	  while (idx_max > idx_min) {
		int idx_mid = (idx_min + idx_max) >> 1;
		MapIntNode *mid_node=l->items[idx_mid];

		if (mid_node->key > key) {
			idx_max = idx_mid;
		}
		else{
			idx_min = idx_mid + 1;
		}
	  }
	}

	if(idx_min >= 0){
		return idx_min;
	}
	return ZG_ERROR;
}

bool		MapInt_Exist(MapInt *t,intptr_t key){
	if(t==NULL) return false;

	return MapString_GetIdx(t,key) != ZG_ERROR;

}

void 		MapInt_Set(MapInt *t,intptr_t key,void * val){
	MapIntNode *node = NULL;

	if(t==NULL) return;

	int pos=MapString_GetIdx(t,key);
	if(pos != ZG_ERROR){ // value already exist (assign)...
		node=t->list->items[pos];
		node->val=val;
	}
	else{ // insert

		// get position to insert...
		if((pos=MapString_GetIdxToInsert(t,key))!=ZG_ERROR){
			node=malloc(sizeof(MapIntNode));
			memset(node,0,sizeof(MapIntNode));

			// set values
			node->key=key;
			node->val=val;


			// .. and finally insert.
			List_Insert(t->list,pos,node);
		}
	}
}

void * 		MapInt_Get(MapInt *t,intptr_t key){

	if(t==NULL) return NULL;

	int pos=MapString_GetIdx(t,key);
	if(pos != ZG_ERROR){
		return ((MapIntNode *)t->list->items[pos])->val;
	}

	return NULL;
}

void MapInt_Erase(MapInt *t,intptr_t key){
	int pos=MapString_GetIdx(t,key);
	if(pos == ZG_ERROR){ // value already exist (assign)...
		Log_Error("key not found");
		return;
	}

	MapInt *node=t->list->items[pos];
	FREE(node);

	List_Erase(t->list,pos);
}

void	MapInt_EraseAndFreeItem(MapInt *t,intptr_t key){
	void * val =MapInt_Get(t,key);

	if(val != NULL){
		FREE(val);
		MapInt_Erase(t,key);
	}else{
		Log_Warning("key %i not exist in map",key);
	}

}

void 	MapInt_Clear(MapInt *t){
	if(t==NULL) return;

	List_Clear(t->list);
}

void 	MapInt_ClearAndFreeAllItems(MapInt *t, bool delete_value){
	if(t==NULL) return;

	for(unsigned i=0;i<t->list->count;i++){
		MapIntNode * node=t->list->items[i];

		// user asks to delete value too
		if(delete_value){
			if(node->val){
				FREE(node->val);
			}
		}

		if(t->on_delete != NULL){
			t->on_delete(node);
		}
		free(node);
	}

	List_Clear(t->list);
}

void MapInt_DeleteAndFreeAllItems(MapInt *t){
	if(t==NULL) return;

	MapInt_ClearAndFreeAllItems(t,true);

	List_Delete(t->list);
	FREE(t);
}

void 		MapInt_Delete(MapInt *t){
	if(t==NULL) return;

	MapInt_ClearAndFreeAllItems(t,false);

	List_Delete(t->list);
	FREE(t);
}
