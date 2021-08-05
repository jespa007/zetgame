#ifndef __LIST_H__
#define __LIST_H__

typedef struct List List;

struct List{
	void** items;
	uint16_t size; // size vector (user count for iterate through items)
	uint16_t count; //number of items
};

List  	*	List_New(void);

void 		List_Set(List *_this, uint16_t idx, void *e);
void	*	List_Get(List *_this, uint16_t idx);
void 		List_Erase(List *_this, uint16_t idx);
bool 		List_Exist(List *_this, void *e);
bool 		List_RemoveIfExist(List *_this, void *e);


/**
 * Insert item at the end
 */
void 		List_Add(List *_this, void *e);
/**
 * Insert item at position idx.
 */
void 		List_Insert(List *_this, uint16_t idx, void *e);
void 		List_Concat(List *_this, List *list);
/**
 * Removes last item
 */
void * 		List_Pop(List *_this);

void 		List_Clear(List *_this);
void		List_ClearAndFreeAllItems(List *_this);

size_t 		List_Count(List *_this);
void 		List_Delete(List *_this);

/**
 * Deletes list and its elements.
 */
void 		List_DeleteAndFreeAllItems(List *v);



#endif
