#ifndef __TRANSFORM_NODE_H__
#define __TRANSFORM_NODE_H__


#define  TRANSFORM_NODE_PROPERTY_FACE_TO_CAMERA		0x1 << 0
#define  TRANSFORM_NODE_PROPERTY_SCALE				0x1 << 1
#define  TRANSFORM_NODE_PROPERTY_TRANSLATE			0x1 << 2
#define  TRANSFORM_NODE_PROPERTY_ROTATE				0x1 << 4

typedef enum{
	 TRANSFORM_NODE_TYPE_LOCAL=0
	,TRANSFORM_NODE_TYPE_WORLD
}TransformNodeType;

typedef struct TransformNode TransformNode;


struct TransformNode{
	Transform  		transform; // world coordinates
	void 			*data;
};

TransformNode *	TransformNode_New(void);
bool		TransformNode_AttachNode(TransformNode *_this, TransformNode *node);
bool		TransformNode_DetachNode(TransformNode *_this, TransformNode *node);
void 		TransformNode_SetTranslate3f(TransformNode *_this,float x, float y, float z);

// 2D Stuff
void 		TransformNode_SetPosition2i(TransformNode *_this,int x, int y);
Vector2i	TransformNode_GetPosition2i(TransformNode *_this);

TransformNode	*	TransformNode_GetParent(TransformNode *_this);

Transform *TransformNode_GetTransform(TransformNode *node, TransformNodeType sgtransform);

void 		TransformNode_SetRotate3f(TransformNode *_this,float x, float y, float z);
void 		TransformNode_SetScale3f(TransformNode *_this,float x, float y, float z);
void		TransformNode_Update(TransformNode *_this);
void 	 	TransformNode_Delete(TransformNode *_this);


#endif
