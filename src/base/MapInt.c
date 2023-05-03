#include "zg_base.h"

MapInt *MapInt_New(){
	MapInt *_this = ZG_NEW(MapInt);
	memset(_this,0,sizeof(MapInt));
	_this->list=List_New();
    return _this;
}

int MapString_GetIdx(MapInt *_this,intptr_t  key){
	List *l=_this->list;
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

int MapString_GetIdxToInsert(MapInt *_this,intptr_t  key){
	// PRE: array is already ordered
	List *l=_this->list;
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

bool		MapInt_Exist(MapInt *_this,intptr_t key){
	if(_this==NULL) return false;

	return MapString_GetIdx(_this,key) != ZG_ERROR;

}

void 		MapInt_Set(MapInt *_this,intptr_t key,void * val){
	MapIntNode *node = NULL;

	if(_this==NULL) return;

	int pos=MapString_GetIdx(_this,key);
	if(pos != ZG_ERROR){ // value already exist (assign)...
		node=_this->list->items[pos];
		node->val=val;
	}
	else{ // insert

		// get position to insert...
		if((pos=MapString_GetIdxToInsert(_this,key))!=ZG_ERROR){
			node=malloc(sizeof(MapIntNode));
			memset(node,0,sizeof(MapIntNode));

			// set values
			node->key=key;
			node->val=val;


			// .. and finally insert.
			List_Insert(_this->list,pos,node);
		}
	}
}

void * 		MapInt_Get(MapInt *_this,intptr_t key){

	if(_this==NULL) return NULL;

	int pos=MapString_GetIdx(_this,key);
	if(pos != ZG_ERROR){
		return ((MapIntNode *)_this->list->items[pos])->val;
	}

	return NULL;
}

void MapInt_Erase(MapInt *_this,intptr_t key){
	int pos=MapString_GetIdx(_this,key);
	if(pos == ZG_ERROR){ // value already exist (assign)...
		Log_ErrorF("key not found");
		return;
	}

	MapInt *node=_this->list->items[pos];
	ZG_FREE(node);

	List_Erase(_this->list,pos);
}

void	MapInt_EraseAndFreeItem(MapInt *_this,intptr_t key){
	void * val =MapInt_Get(_this,key);

	if(val != NULL){
		ZG_FREE(val);
		MapInt_Erase(_this,key);
	}else{
		Log_Warning("key %i not exist in map",key);
	}

}

void 	MapInt_Clear(MapInt *_this, bool _free_all_items){
	/*if(_this==NULL) return;

	List_Clear(_this->list);*/

	if(_this==NULL) return;

	for(unsigned i=0;i<_this->list->count;i++){
		MapIntNode * node=_this->list->items[i];

		if(_this->on_delete != NULL){
			_this->on_delete(node);
		}

		// user asks to delete value too
		if(_free_all_items){
			if(node->val){
				ZG_FREE(node->val);
			}
		}

		free(node);
	}

	List_Clear(_this->list);
}



void MapInt_DeleteAndFreeAllItems(MapInt *_this){
	if(_this==NULL) return;

	MapInt_Clear(_this,true);

	List_Delete(_this->list);
	ZG_FREE(_this);
}

void 		MapInt_Delete(MapInt *_this){
	if(_this==NULL) return;

	MapInt_Clear(_this,false);

	List_Delete(_this->list);
	ZG_FREE(_this);
}
