#include "_zg_base_.h"

ZG_MapInt *ZG_MapInt_New(){
	ZG_MapInt *_this = ZG_NEW(ZG_MapInt);
	memset(_this,0,sizeof(ZG_MapInt));
	_this->list=ZG_List_New();
    return _this;
}

int MapString_GetIdx(ZG_MapInt *_this,intptr_t  key){
	ZG_List *l=_this->list;
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
		ZG_MapIntNode * node = (ZG_MapIntNode *)l->items[idx_mid];
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

int MapString_GetIdxToInsert(ZG_MapInt *_this,intptr_t  key){
	// PRE: array is already ordered
	ZG_List *l=_this->list;
	intptr_t size=l->count - 1;
	intptr_t idx_min = 0, idx_max = size;
	ZG_MapIntNode *max_node=NULL;

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
		ZG_MapIntNode *mid_node=l->items[idx_mid];

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

bool		ZG_MapInt_Exist(ZG_MapInt *_this,intptr_t key){
	if(_this==NULL) return false;

	return MapString_GetIdx(_this,key) != ZG_ERROR;

}

void 		ZG_MapInt_Set(ZG_MapInt *_this,intptr_t key,void * val){
	ZG_MapIntNode *node = NULL;

	if(_this==NULL) return;

	int pos=MapString_GetIdx(_this,key);
	if(pos != ZG_ERROR){ // value already exist (assign)...
		node=_this->list->items[pos];
		node->val=val;
	}
	else{ // insert

		// get position to insert...
		if((pos=MapString_GetIdxToInsert(_this,key))!=ZG_ERROR){
			node=malloc(sizeof(ZG_MapIntNode));
			memset(node,0,sizeof(ZG_MapIntNode));

			// set values
			node->key=key;
			node->val=val;


			// .. and finally insert.
			ZG_List_Insert(_this->list,pos,node);
		}
	}
}

void * 		ZG_MapInt_Get(ZG_MapInt *_this,intptr_t key){

	if(_this==NULL) return NULL;

	int pos=MapString_GetIdx(_this,key);
	if(pos != ZG_ERROR){
		return ((ZG_MapIntNode *)_this->list->items[pos])->val;
	}

	return NULL;
}

void ZG_MapInt_Erase(ZG_MapInt *_this,intptr_t key){
	int pos=MapString_GetIdx(_this,key);
	if(pos == ZG_ERROR){ // value already exist (assign)...
		ZG_LOG_ERRORF("key not found");
		return;
	}

	ZG_MapInt *node=_this->list->items[pos];
	ZG_FREE(node);

	ZG_List_Erase(_this->list,pos);
}

void	ZG_MapInt_EraseAndFreeItem(ZG_MapInt *_this,intptr_t key){
	void * val =ZG_MapInt_Get(_this,key);

	if(val != NULL){
		ZG_FREE(val);
		ZG_MapInt_Erase(_this,key);
	}else{
		ZG_LOG_WARNING("key %i not exist in map",key);
	}

}

void 	ZG_MapInt_ClearInternal(ZG_MapInt *_this, bool _free_all_items){
	/*if(_this==NULL) return;

	ZG_List_Clear(_this->list);*/

	if(_this==NULL) return;

	for(unsigned i=0;i<_this->list->count;i++){
		ZG_MapIntNode * node=_this->list->items[i];

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

	ZG_List_Clear(_this->list);
}

void 	ZG_MapInt_ClearAndFreeAllItems(ZG_MapInt *_this){
	ZG_MapInt_ClearInternal(_this,true);
}

void 	ZG_MapInt_Clear(ZG_MapInt *_this){
	ZG_MapInt_ClearInternal(_this,false);
}






void ZG_MapInt_DeleteAndFreeAllItems(ZG_MapInt *_this){
	if(_this==NULL) return;

	ZG_MapInt_ClearAndFreeAllItems(_this);

	ZG_List_Delete(_this->list);
	ZG_FREE(_this);
}

void 		ZG_MapInt_Delete(ZG_MapInt *_this){
	if(_this==NULL) return;

	ZG_MapInt_Clear(_this);

	ZG_List_Delete(_this->list);
	ZG_FREE(_this);
}
