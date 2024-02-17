#ifndef __ZG_TRANSFORM_NODE_H__
#define __ZG_TRANSFORM_NODE_H__


#define  ZG_TRANSFORM_NODE_PROPERTY_TRANSLATE			0x1 << 0
#define  ZG_TRANSFORM_NODE_PROPERTY_ROTATE				0x1 << 1
#define  ZG_TRANSFORM_NODE_PROPERTY_SCALE				0x1 << 2



typedef enum{
	 ZG_TRANSFORM_NODE_TYPE_LOCAL=0
	,ZG_TRANSFORM_NODE_TYPE_WORLD
}ZG_TransformNodeType;

typedef struct ZG_TransformNode ZG_TransformNode;


struct ZG_TransformNode{
	ZG_Transform  			world_transform; // world coordinates
	ZG_Quaternion			world_quaternion;
	void 					*data;
};


// MEMBER
ZG_TransformNode 	*	ZG_TransformNode_New(void);
bool					ZG_TransformNode_Attach(ZG_TransformNode *_this, ZG_TransformNode * _transform_node);
bool 					ZG_TransformNode_Detach(ZG_TransformNode * _this);
ZG_TransformNode	*	ZG_TransformNode_GetParent(ZG_TransformNode *_this);
void					ZG_TransformNode_SetDisplacement2i(ZG_TransformNode *_this, int _offset_x, int _offset_y);
void					ZG_TransformNode_SetPosition2i(ZG_TransformNode *_this, int _x,int _y);
void 					ZG_TransformNode_SetTranslate3f(ZG_TransformNode *_this,float _x, float _y, float _z);
void					ZG_TransformNode_SetRotate3f(ZG_TransformNode *_this,float _x, float _y, float _z);

ZG_Transform 		*	ZG_TransformNode_GetTransform(ZG_TransformNode *node, ZG_TransformNodeType _transform_node_type);
void					ZG_TransformNode_Update(ZG_TransformNode *_this);
void 	 				ZG_TransformNode_Delete(ZG_TransformNode *_this);


#endif
