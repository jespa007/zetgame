#ifndef __TRANSFORM_NODE_H__
#define __TRANSFORM_NODE_H__

struct TransformNode{
	ZG_ECTransform	*ec_transform;
	void	*data;
};

TransformNode *	TransformNode_New(Scene *_scene,ZG_Entity *_entity);
void 	 		TransformNode_Delete(TransformNode *_this);





#endif
