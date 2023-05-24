#ifndef __TRANSFORM_NODE_H__
#define __TRANSFORM_NODE_H__

struct TransformNode{
	ECTransform	*ec_transform;
	void	*data;
};

TransformNode *	TransformNode_New(Scene *_scene,Entity *_entity);
void 	 		TransformNode_Delete(TransformNode *_this);





#endif
