#ifndef __ZG_ECTRANSFORM_NODE_H__
#define __ZG_ECTRANSFORM_NODE_H__


#define  ZG_ECTRANSFORM_NODE_PROPERTY_FACE_TO_CAMERA		0x1 << 0
#define  ZG_ECTRANSFORM_NODE_PROPERTY_SCALE				0x1 << 1
#define  ZG_ECTRANSFORM_NODE_PROPERTY_TRANSLATE			0x1 << 2
#define  ZG_ECTRANSFORM_NODE_PROPERTY_ROTATE				0x1 << 4

typedef enum{
	 ZG_ECTRANSFORM_NODE_TYPE_LOCAL=0
	,ZG_ECTRANSFORM_NODE_TYPE_WORLD
}ZG_ECTransformNodeType;

typedef struct ZG_TransformNode ZG_TransformNode;


struct ZG_ECTransformNode{
	ZG_ECTransform  		transform; // world coordinates
	void 					*data;
};

ZG_TransformNode *	ZG_ECTransformNode_New(void);
bool		ZG_ECTransformNode_AttachNode(ZG_TransformNode *_this, ZG_TransformNode *node);
bool		ZG_TransformNode_DetachNode(ZG_TransformNode *_this, ZG_TransformNode *node);
ZG_TransformNode	*	ZG_ECTransformNode_GetParent(ZG_TransformNode *_this);

ZG_Transform *ZG_ECTransformNode_GetTransform(ZG_TransformNode *node, ZG_ECTransformNodeType sgtransform);
void		ZG_ECTransformNode_Update(ZG_TransformNode *_this);
void 	 	ZG_ECTransformNode_Delete(ZG_TransformNode *_this);


#endif
