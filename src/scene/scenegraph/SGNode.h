#ifndef __SGNODE_H__
#define __SGNODE_H__


typedef struct SGNode SGNode;
typedef struct Scene  Scene;

struct SGNode{
	Entity  *entity;
	void	*data;
};

SGNode *	SGNode_New(Entity *_entity);
Entity *	SGNode_GetEntity(SGNode *_this);
bool		SGNode_AttachNode(SGNode *_this, SGNode *node);
bool		SGNode_DetachNode(SGNode *_this, SGNode *node);
void 		SGNode_SetTranslate3f(SGNode *_this,float x, float y, float z);
void 		SGNode_SetRotate3f(SGNode *_this,float x, float y, float z);
void 		SGNode_SetScale3f(SGNode *_this,float x, float y, float z);
void 		SGNode_SetPosition2i(SGNode *_this,int x, int y);
Vector2i	SGNode_GetPosition2i(SGNode *_this);
void 		SGNode_SetDisplacement2i(SGNode *_this,int x, int y);
void 	 	SGNode_Delete(SGNode *_this);





#endif
