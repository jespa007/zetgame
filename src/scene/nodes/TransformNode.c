#include "scene/zg_scene.h"

typedef struct{
	void *data;
}TransformNodeData;

TransformNode *	TransformNode_New(Entity *_entity){

	if(_entity == NULL){
		Log_ErrorF("TransformNode_New : Cannot create transform_node. Entity == NULL");
		return NULL;
	}

	TransformNode *transform_node = ZG_NEW(TransformNode);
	transform_node->ec_transform=_entity->components[EC_TRANSFORM];
	transform_node->ec_transform_animation=_entity->components[EC_TRANSFORM_ANIMATION];

	TransformNodeData *data = ZG_NEW(TransformNodeData);
	transform_node->data=data;
	return transform_node;
}


void 	 	TransformNode_Delete(TransformNode *_this){
	TransformNodeData *data = _this->data;

	ZG_FREE(data);
	ZG_FREE(_this);
}
