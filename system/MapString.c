#include "zg_system.h"

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
	MapString *t = NEW(MapString);// (struct table*)malloc(sizeof(struct table));
	memset(t,0,sizeof(MapString));
	t->count=HASH_MAP_SIZE;
    t->list = (MapStringNode **)malloc(sizeof(MapStringNode*)*t->count);


    for(unsigned i=0;i<t->count;i++){
    	t->list[i]=NULL;
    }
    return t;
}

MapStringNode * MapString_LookUpNode(MapString *t,const char * key){
	uint8_t pos = MapString_HashCode(key);
	MapStringNode *list = t->list[pos];
	MapStringNode *temp = list;

    while(temp){
        if(strcmp(temp->key,key)==0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void MapString_SetValue(MapString *t,const char * key,void * val){

    MapStringNode *node = NULL;

	if(t == NULL) return;

	if((node=MapString_LookUpNode(t,key))==NULL){
		uint8_t pos = MapString_HashCode(key);
		MapStringNode *first_node = t->list[pos];
		node = NEW(MapStringNode);

		// insert node at the end (not ordered)
		size_t key_len=strlen(key)+1;
		node->key = malloc(key_len);
		memset(node->key,0,key_len);
		strcpy(node->key,key);
		node->val = val;
		node->next = first_node;
		t->list[pos] = node; //Replace first node by _this one...
	}
	node->val = val;
}

void * 	MapString_GetValue(MapString *t,const char * key){
	MapStringNode * node=MapString_LookUpNode(t,key);

	if(node != NULL){
		return node->val;
	}

	return NULL;
}

bool		MapString_Exist(MapString *t,const char * key){
	return MapString_GetValue(t,key) != NULL;
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
