#ifndef __SGNODE_H__
#define __SGNODE_H__



typedef enum{
	 SGNODE_TRANSFORM_FACE_TO_CAMERA=0x1 << 0
	,SGNODE_TRANSFORM_SCALE=0x1 << 1
	,SGNODE_TRANSFORM_TRANSLATE=0x1 << 2
	,SGNODE_TRANSFORM_ROTATE=0x1 << 4
}SGNodeTransform;

typedef enum{
	 SGNODE_TRANSFORM_TYPE_LOCAL=0
	,SGNODE_TRANSFORM_TYPE_WORLD
}SGNodeTransformType;

typedef struct SGNode SGNode;
typedef struct Scene  Scene;

struct SGNode{
	Transform  	*transform; // transform absolute (coords)
	Appearance 		*appearance;
	Geometry		*geometry;
	//SGNodeType		sgnode_type;
	void 			*data;
};

SGNode *	SGNode_New(void);
void 		SGNode_SetScene(SGNode *_this, Scene *scene);
bool		SGNode_AttachNode(SGNode *_this, SGNode *node);
bool		SGNode_DetachNode(SGNode *_this, SGNode *node);
void 		SGNode_SetTranslate3f(SGNode *_this,float x, float y, float z);

// 2D Stuff
void 		SGNode_SetPosition2i(SGNode *_this,int x, int y);
Vector2i	SGNode_GetPosition2i(SGNode *_this);

SGNode	*	SGNode_GetParent(SGNode *_this);


Transform *SGNode_GetTransform(SGNode *node, SGNodeTransformType sgtransform);



void 		SGNode_SetRotate3f(SGNode *_this,float x, float y, float z);
void 		SGNode_SetScale3f(SGNode *_this,float x, float y, float z);
void		SGNode_Update(SGNode *_this);
void 	 	SGNode_Delete(SGNode *_this);





#endif
