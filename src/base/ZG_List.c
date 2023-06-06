#include "_zg_base_.h"

#define ZG_MAX_ELEMENTS_VECTOR	16000

bool 		ZG_List_AddSlot(ZG_List *v){
	if (v->size == 0) {
		v->size = 10;
		v->items = malloc(sizeof(void*) * v->size);
		memset(v->items, '\0', sizeof(void *) * v->size);
	}

	// condition to increase v->items:
	// last slot exhausted
	if (v->size == v->count) {
		if((v->size+10) >= ZG_MAX_ELEMENTS_VECTOR){
			ZG_Log_ErrorF("Max elements vector");
			return false;
		}
		v->size += 10;
		v->items = realloc(v->items, sizeof(void*) * v->size);
	}

	v->count++;
	return true;
}

ZG_List * ZG_List_New(){
	ZG_List *v=ZG_NEW(ZG_List);
	memset(v,0,sizeof(ZG_List));
	return v;
}



void ZG_List_Set(ZG_List *v, uint16_t idx, void *e){
	if (idx >= v->count) {
		ZG_Log_ErrorF("idx out of bounds");
		return;
	}
	v->items[idx] = e;
}

size_t ZG_List_Count(ZG_List *v){
	return v->count;
}

void *ZG_List_Get(ZG_List *v, uint16_t idx){
	if (idx >= v->count) {
		ZG_Log_ErrorF("idx out of bounds");
		return NULL;
	}

	return v->items[idx];
}

void ZG_List_Erase(ZG_List *_this, uint16_t idx){
	if (idx >= _this->count) {
		ZG_Log_ErrorF("idx out of bounds");
		return;
	}

	_this->items[idx] = NULL;

	for (uint16_t i = idx; i < (_this->count-1); i++) {
		_this->items[i] = _this->items[i+1];
	}

	_this->items[_this->count-1] = NULL;
	_this->count--;
}

bool 		ZG_List_Exist(ZG_List *_this, void *e){
	for (unsigned i = 0; i < _this->count; i++) {
		if(_this->items[i] == e){
			return true;
		}
	}

	return false;
}

bool 		ZG_List_RemoveIfExist(ZG_List *_this, void *e){
	for (unsigned i = 0; i < _this->count; i++) {
		if(_this->items[i] == e){
			ZG_List_Erase(_this,i);
			return true;
		}
	}

	return false;
}

void ZG_List_Add(ZG_List *v, void *e){
	if(ZG_List_AddSlot(v)){
		v->items[v->count-1] = e; // add element to end list...
	}
}

void ZG_List_Concat(ZG_List *_this, ZG_List *list){
	if(_this != NULL && list) {
		for(unsigned i=0; i <  list->count; i++){
			ZG_List_Add(_this, list->items[i]);
		}
	}
}

void 		ZG_List_Insert(ZG_List *v, uint16_t idx, void *e){
	if(idx > (v->count+1)){
		ZG_Log_Error("idx should be 0 to %i",v->count+1);
		return;
	}

	if(ZG_List_AddSlot(v)){
		// 1. move all elements...
		for(int i=v->count-1;i>idx;i--){
			v->items[i]=v->items[i-1];
		}
		// 2. Assign element
		v->items[idx]=e;
	}
}

void * 		ZG_List_Pop(ZG_List *_this){

	if(_this->count == 0){
		ZG_Log_ErrorF("list empty");
		return NULL;
	}

	return _this->items[--_this->count];
}

void 		ZG_List_Clear(ZG_List *_this){
	if(_this == NULL) return;

	if(_this->items!=NULL){
		ZG_FREE(_this->items);
	}
	memset(_this,0,sizeof(ZG_List));
}

void 		ZG_List_ClearAndFreeAllItems(ZG_List *_this){
	if(_this == NULL) return;


	if(_this->items!=NULL){

		for(unsigned i=0; i < _this->count; i++){
			ZG_FREE(_this->items[i]);
		}

		ZG_FREE(_this->items);
	}
	memset(_this,0,sizeof(ZG_List));
}


void ZG_List_Delete(ZG_List *_this){
	if(_this == NULL) return;

	if(_this->items!=NULL){
		ZG_FREE(_this->items);
	}
	ZG_FREE(_this);
}

void ZG_List_DeleteAndFreeAllItems(ZG_List *_this){
	if(_this == NULL) return;

	for(unsigned i=0; i < _this->count; i++){
		ZG_FREE(_this->items[i]);
	}
	ZG_List_Delete(_this);
}

