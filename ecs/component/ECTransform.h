#ifndef __EC_TRANSFORM_H__
#define __EC_TRANSFORM_H__

typedef struct ECTransform ECTransform;
typedef struct Entity	Entity;


struct ECTransform{
	Entity 			*entity;
	TransformNode 	transform; // world coordinates
	void 			*data;
};

ECTransform *	ECSGNode_New(void);
bool		ECSGNode_AttachNode(ECTransform *_this, ECTransform *node);
bool		ECSGNode_DetachNode(ECTransform *_this, ECTransform *node);
void 		ECSGNode_SetTranslate3f(ECTransform *_this,float x, float y, float z);

// 2D Stuff
void 		ECSGNode_SetPosition2i(ECTransform *_this,int x, int y);
Vector2i	ECSGNode_GetPosition2i(ECTransform *_this);

ECTransform	*	ECSGNode_GetParent(ECTransform *_this);

Transform *ECSGNode_GetTransform(ECTransform *node, TransformNodeType sgtransform);

void 		ECSGNode_SetRotate3f(ECTransform *_this,float x, float y, float z);
void 		ECSGNode_SetScale3f(ECTransform *_this,float x, float y, float z);
void		ECSGNode_Update(ECTransform *_this);
void 	 	ECSGNode_Delete(ECTransform *_this);


#endif
