#include "@zg_base.h"

typedef struct{
	ZG_MapString 		*map;
	int 			idx_current_hash;
	ZG_MapStringNode 	*current_node;
	ZG_MapStringNode 	*current_list;
	const char 		*key;
	void 			*value;
}ZG_MapStringIteratorData;

void ZG_MapStringIterator_FindNextListNotEmpty(ZG_MapStringIterator *_this);


ZG_MapStringIterator *ZG_MapStringIterator_New(ZG_MapString *_map){
	ZG_MapStringIterator *_this=ZG_NEW(ZG_MapStringIterator);
	ZG_MapStringIteratorData *data=ZG_NEW(ZG_MapStringIteratorData);
	data->map=_map;
	data->current_node=NULL;
	data->current_list=NULL;
	data->idx_current_hash=0;
	_this->data=data;

	ZG_MapStringIterator_FindNextListNotEmpty(_this);

	return _this;
}

const char *ZG_MapStringIterator_GetKey(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	return data->key;
}

void *ZG_MapStringIterator_GetValue(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	return data->value;
}

void ZG_MapStringIterator_SetCurrentPair(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	data->key=data->current_node->key;
	data->value=data->current_node->val;
}


void ZG_MapStringIterator_FindNextListNotEmpty(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	for(; data->idx_current_hash < ZG_HASH_MAP_SIZE && data->current_list==NULL; data->idx_current_hash++){
		if(data->map->list[data->idx_current_hash] != NULL){
			data->current_list=(ZG_MapStringNode *)data->map->list[data->idx_current_hash];
			data->current_node=data->current_list;
			ZG_MapStringIterator_SetCurrentPair(_this);
		}
	}
}

void ZG_MapStringIterator_Next(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	if(data->current_node->next == data->current_node->previous){ // set current list as null for next search
		data->current_list=NULL;
	}else{
		data->current_node =data->current_node->next;
	}

	if(data->current_list==NULL){ // get first node not NULL
		ZG_MapStringIterator_FindNextListNotEmpty(_this);
	}else{
		//else if(current_node != NULL){
		// set current key/value
		ZG_MapStringIterator_SetCurrentPair(_this);
	}
	//}
}

bool ZG_MapStringIterator_End(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	return data->idx_current_hash == ZG_HASH_MAP_SIZE;
}

void				ZG_MapStringIterator_Delete(ZG_MapStringIterator *_this){
	ZG_MapStringIteratorData *data=_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
