#include "zg_base.h"

#define HASH_MAP_SIZE 256



/*struct table{
    int size;
    struct node *list[HASH_MAP_SIZE];
};*/

uint8_t MapString_HashCode(const char * key){
	size_t key_len=strlen(key);
	uint8_t p=0;

	for(unsigned i=0; i < key_len; i++){
		p+=key[i];
	}

    return p;
}

MapString *MapString_New(){
	MapString *t = ZG_NEW(MapString);// (struct table*)malloc(sizeof(struct table));
	memset(t,0,sizeof(MapString));
	t->count=HASH_MAP_SIZE;
    t->list = (MapStringNode **)malloc(sizeof(MapStringNode*)*t->count);


    for(unsigned i=0;i<t->count;i++){
    	t->list[i]=NULL;
    }
    return t;
}

MapStringNode * MapString_LookUpNode(MapString *_this,const char * key){
	uint8_t pos = MapString_HashCode(key);
	MapStringNode *list = _this->list[pos];
	MapStringNode *temp = list;

    while(temp){
        if(strcmp(temp->key,key)==0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void MapString_SetValue(MapString *_this,const char * key,void * val){

	MapStringNode *node = NULL;

	if((node=MapString_LookUpNode(_this,key))==NULL){
		uint8_t pos = MapString_HashCode(key);
		MapStringNode *first_node = _this->list[pos];
		node = (MapStringNode *)malloc(sizeof(MapStringNode));

		// insert node at the end (not ordered)
		size_t key_len=strlen(key)+1;
		node->key = (char *)malloc(key_len);
		memset(node->key,0,key_len);
		strcpy(node->key,key);
		node->val = val;
		node->next = first_node;
		node->previous=NULL;

		if(first_node != NULL){
			first_node->previous=node;
		}

		_this->list[pos] = node; //Replace first node by _this one...
	}
	node->val = val;
}

void * 	MapString_GetValue(MapString *_this,const char * key, bool * exists){
	MapStringNode * node=MapString_LookUpNode(_this,key);
	if(exists!=NULL){
		*exists=false;
	}

	if(node != NULL){
		if(exists!=NULL) {
			*exists=true;
		}
		return node->val;
	}

	return NULL;
}

bool		MapString_Exist(MapString *_this,const char * key){
	return MapString_GetValue(_this,key,NULL) != NULL;
}

void 		MapString_Erase(MapString *_this,const char * key){
	uint8_t pos = MapString_HashCode(key);
	MapStringNode * node=MapString_LookUpNode(_this,key);

	if(node == NULL){
		Log_Error("Element %s not exist",key);
		return;
	}

	// not first...
	if(node->previous != NULL){
		node->previous->next=node->next; // link previous
	}else{ // first, set first element as next
		_this->list[pos]=node->next;
	}

	// not last
	if(node->next!=NULL){
		node->next->previous=node->previous; // link previous-next
	}

	free(node->key);
	free(node);

}

void MapString_Clear(MapString *_this){
	for(unsigned i=0;i<_this->count;i++){
		MapStringNode * temp=_this->list[i];

		while(temp){
			MapStringNode * to_deallocate = temp;
			temp=temp->next;

			/*if(t->on_delete != NULL){
				t->on_delete(to_deallocate);
			}*/

			free(to_deallocate->key);
			free(to_deallocate);

		}
	}
	memset(_this->list,0,sizeof(MapStringNode*)*_this->count);
}

void MapString_Delete(MapString *t){
	if(t == NULL) return;

    for(unsigned i=0;i<t->count;i++){
    	MapStringNode * temp=t->list[i];

        while(temp){
        	MapStringNode * to_deallocate = temp;
        	temp=temp->next;

        	if(t->on_delete != NULL){
        		t->on_delete(to_deallocate);
        	}

        	free(to_deallocate->key);
        	free(to_deallocate);

        }
    }

    FREE(t->list);
    FREE(t);
}
