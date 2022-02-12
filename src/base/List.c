#include "zg_base.h"

#define MAX_ELEMENTS_VECTOR	16000

bool 		List_AddSlot(List *v){
	if (v->size == 0) {
		v->size = 10;
		v->items = malloc(sizeof(void*) * v->size);
		memset(v->items, '\0', sizeof(void *) * v->size);
	}

	// condition to increase v->items:
	// last slot exhausted
	if (v->size == v->count) {
		if((v->size+10) >= MAX_ELEMENTS_VECTOR){
			Log_ErrorF("Max elements vector");
			return false;
		}
		v->size += 10;
		v->items = realloc(v->items, sizeof(void*) * v->size);
	}

	v->count++;
	return true;
}

List * List_New(){
	List *v=ZG_NEW(List);
	memset(v,0,sizeof(List));
	return v;
}



void List_Set(List *v, uint16_t idx, void *e){
	if (idx >= v->count) {
		Log_ErrorF("idx out of bounds");
		return;
	}
	v->items[idx] = e;
}

size_t List_Count(List *v){
	return v->count;
}

void *List_Get(List *v, uint16_t idx){
	if (idx >= v->count) {
		Log_ErrorF("idx out of bounds");
		return NULL;
	}

	return v->items[idx];
}

void List_Erase(List *_this, uint16_t idx){
	if (idx >= _this->count) {
		Log_ErrorF("idx out of bounds");
		return;
	}

	_this->items[idx] = NULL;

	for (uint16_t i = idx; i < (_this->count-1); i++) {
		_this->items[i] = _this->items[i+1];
	}

	_this->items[_this->count-1] = NULL;
	_this->count--;
}

bool 		List_Exist(List *_this, void *e){
	for (unsigned i = 0; i < _this->count; i++) {
		if(_this->items[i] == e){
			return true;
		}
	}

	return false;
}

bool 		List_RemoveIfExist(List *_this, void *e){
	for (unsigned i = 0; i < _this->count; i++) {
		if(_this->items[i] == e){
			List_Erase(_this,i);
			return true;
		}
	}

	return false;
}

void List_Add(List *v, void *e){
	if(List_AddSlot(v)){
		v->items[v->count-1] = e; // add element to end list...
	}
}

void List_Concat(List *_this, List *list){
	if(_this != NULL && list) {
		for(unsigned i=0; i <  list->count; i++){
			List_Add(_this, list->items[i]);
		}
	}
}

void 		List_Insert(List *v, uint16_t idx, void *e){
	if(idx > (v->count+1)){
		Log_Error("idx should be 0 to %i",v->count+1);
		return;
	}

	if(List_AddSlot(v)){
		// 1. move all elements...
		for(int i=v->count-1;i>idx;i--){
			v->items[i]=v->items[i-1];
		}
		// 2. Assign element
		v->items[idx]=e;
	}
}

void * 		List_Pop(List *_this){

	if(_this->count == 0){
		Log_ErrorF("list empty");
		return NULL;
	}

	return _this->items[--_this->count];
}

void 		List_Clear(List *_this){
	if(_this == NULL) return;

	if(_this->items!=NULL){
		FREE(_this->items);
	}
	memset(_this,0,sizeof(List));
}

void 		List_ClearAndFreeAllItems(List *_this){
	if(_this == NULL) return;


	if(_this->items!=NULL){

		for(unsigned i=0; i < _this->count; i++){
			FREE(_this->items[i]);
		}

		FREE(_this->items);
	}
	memset(_this,0,sizeof(List));
}


void List_Delete(List *_this){
	if(_this == NULL) return;

	if(_this->items!=NULL){
		FREE(_this->items);
	}
	FREE(_this);
}

void List_DeleteAndFreeAllItems(List *_this){
	if(_this == NULL) return;

	for(unsigned i=0; i < _this->count; i++){
		FREE(_this->items[i]);
	}
	List_Delete(_this);
}

