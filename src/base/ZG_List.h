#ifndef __ZG_LIST_H__
#define __ZG_LIST_H__

typedef struct ZG_List ZG_List;

struct ZG_List{
	void** items;
	size_t size; // size vector (user count for iterate through items)
	size_t count; //number of items
};

ZG_List  	*	ZG_List_New(void);

void 		ZG_List_Set(ZG_List *_this, size_t idx, void *e);
void	*	ZG_List_Get(ZG_List *_this, size_t idx);
void 		ZG_List_Erase(ZG_List *_this, size_t idx);
bool 		ZG_List_Exist(ZG_List *_this, void *e);
bool 		ZG_List_RemoveIfExist(ZG_List *_this, void *e);


/**
 * Insert item at the end
 */
void 		ZG_List_Add(ZG_List *_this, void *e);
/**
 * Insert item at position idx.
 */
void 		ZG_List_Insert(ZG_List *_this, size_t idx, void *e);
void 		ZG_List_Concat(ZG_List *_this, ZG_List *list);
/**
 * Removes last item
 */
void * 		ZG_List_Pop(ZG_List *_this);

void 		ZG_List_Clear(ZG_List *_this);
void		ZG_List_ClearAndFreeAllItems(ZG_List *_this);

size_t 		ZG_List_Count(ZG_List *_this);
void 		ZG_List_Delete(ZG_List *_this);

/**
 * Deletes list and its elements.
 */
void 		ZG_List_DeleteAndFreeAllItems(ZG_List *v);



#endif
