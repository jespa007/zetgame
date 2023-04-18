#include "zg_base.h"

typedef struct{
	MapString 		*map;
	int 			idx_current_hash;
	MapStringNode 	*current_node;
	MapStringNode 	*current_list;
	const char 		*key;
	void 			*value;
}MapStringIteratorData;

void MapStringIterator_FindNextListNotEmpty(MapStringIterator *_this);


MapStringIterator *MapStringIterator_New(MapString *_map){
	MapStringIterator *_this=ZG_NEW(MapStringIterator);
	MapStringIteratorData *data=ZG_NEW(MapStringIteratorData);
	data->map=_map;
	data->current_node=NULL;
	data->current_list=NULL;
	data->idx_current_hash=0;
	_this->data=data;

	MapStringIterator_FindNextListNotEmpty(_this);

	return _this;
}

const char *MapStringIterator_GetKey(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	return data->key;
}

void *MapStringIterator_GetValue(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	return data->value;
}

void MapStringIterator_SetCurrentPair(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	data->key=data->current_node->key;
	data->value=data->current_node->val;
}


void MapStringIterator_FindNextListNotEmpty(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	for(; data->idx_current_hash < HASH_MAP_SIZE && data->current_list==NULL; data->idx_current_hash++){
		if(data->map->list[data->idx_current_hash] != NULL){
			data->current_list=(MapStringNode *)data->map->list[data->idx_current_hash];
			data->current_node=data->current_list;
			MapStringIterator_SetCurrentPair(_this);
		}
	}
}

void MapStringIterator_Next(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	if(data->current_node->next == data->current_node->previous){ // set current list as null for next search
		data->current_list=NULL;
	}else{
		data->current_node =data->current_node->next;
	}

	if(data->current_list==NULL){ // get first node not NULL
		MapStringIterator_FindNextListNotEmpty(_this);
	}else{
		//else if(current_node != NULL){
		// set current key/value
		MapStringIterator_SetCurrentPair(_this);
	}
	//}
}

bool MapStringIterator_End(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	return data->idx_current_hash == HASH_MAP_SIZE;
}

void				MapStringIterator_Delete(MapStringIterator *_this){
	MapStringIteratorData *data=_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
