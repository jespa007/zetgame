#ifndef __TRANSFORM_NODE_H__
#define __TRANSFORM_NODE_H__

struct TransformNode{
	ECTransform	*ec_transform;
	ECTransform	*ec_transform_animation;
	void	*data;
};

TransformNode *	TransformNode_New(Entity *_entity);
void 	 		TransformNode_Delete(TransformNode *_this);





#endif
