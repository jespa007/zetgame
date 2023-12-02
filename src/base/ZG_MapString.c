#include "@zg_base.h"

#define ZG_HASH_MAP_SIZE 256



/*struct table{
    int size;
    struct node *list[HASH_MAP_SIZE];
};*/

uint8_t ZG_MapString_HashCode(const char * key){
	size_t key_len=strlen(key);
	uint8_t p=0;

	for(unsigned i=0; i < key_len; i++){
		p+=key[i];
	}

    return p;
}

ZG_MapString *ZG_MapString_New(){
	ZG_MapString *t = ZG_NEW(ZG_MapString);// (struct table*)malloc(sizeof(struct table));
	memset(t,0,sizeof(ZG_MapString));
	t->count=ZG_HASH_MAP_SIZE;
    t->list = (ZG_MapStringNode **)malloc(sizeof(ZG_MapStringNode*)*t->count);


    for(unsigned i=0;i<t->count;i++){
    	t->list[i]=NULL;
    }
    return t;
}

ZG_MapStringNode * ZG_MapString_LookUpNode(ZG_MapString *_this,const char * key){
	uint8_t pos = ZG_MapString_HashCode(key);
	ZG_MapStringNode *list = _this->list[pos];
	ZG_MapStringNode *temp = list;

    while(temp){
        if(strcmp(temp->key,key)==0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void ZG_MapString_Set(ZG_MapString *_this,const char * key,void * val){

	ZG_MapStringNode *node = NULL;

	if((node=ZG_MapString_LookUpNode(_this,key))==NULL){
		uint8_t pos = ZG_MapString_HashCode(key);
		ZG_MapStringNode *first_node = _this->list[pos];
		node = (ZG_MapStringNode *)malloc(sizeof(ZG_MapStringNode));

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

void * 	ZG_MapString_Get(ZG_MapString *_this,const char * key, bool * exists){
	ZG_MapStringNode * node=ZG_MapString_LookUpNode(_this,key);
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

bool		ZG_MapString_Exist(ZG_MapString *_this,const char * key){
	return ZG_MapString_Get(_this,key,NULL) != NULL;
}

void 		ZG_MapString_Erase(ZG_MapString *_this,const char * key){
	uint8_t pos = ZG_MapString_HashCode(key);
	ZG_MapStringNode * node=ZG_MapString_LookUpNode(_this,key);

	if(node == NULL){
		ZG_LOG_ERROR("Element %s not exist",key);
		return;
	}

	// deallocate
	if(_this->on_delete != NULL){
		_this->on_delete(node);
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

void ZG_MapString_Clear(ZG_MapString *_this){
	for(unsigned i=0;i<_this->count;i++){
		ZG_MapStringNode * temp=_this->list[i];

		while(temp){
			ZG_MapStringNode * to_deallocate = temp;
			temp=temp->next;

			/*if(t->on_delete != NULL){
				t->on_delete(to_deallocate);
			}*/

			free(to_deallocate->key);
			free(to_deallocate);

		}
	}
	memset(_this->list,0,sizeof(ZG_MapStringNode*)*_this->count);
}

void ZG_MapString_Delete(ZG_MapString *_this){
	if(_this == NULL) return;

    for(unsigned i=0;i<_this->count;i++){
    	ZG_MapStringNode * temp=_this->list[i];

        while(temp){
        	ZG_MapStringNode * to_deallocate = temp;
        	temp=temp->next;

        	if(_this->on_delete != NULL){
        		_this->on_delete(to_deallocate);
        	}

        	free(to_deallocate->key);
        	free(to_deallocate);

        }
    }

    ZG_FREE(_this->list);
    ZG_FREE(_this);
}
