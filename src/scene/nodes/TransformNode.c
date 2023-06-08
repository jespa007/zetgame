#include "scene/zg_scene.h"

typedef struct{
	Scene *scene;
}TransformNodeData;

TransformNode *	TransformNode_New(Scene *_scene,ZG_Entity *_entity){

	if(_entity == NULL){
		ZG_Log_ErrorF("TransformNode_New : Cannot create transform_node. ZG_Entity == NULL");
		return NULL;
	}

	TransformNode *transform_node = ZG_NEW(TransformNode);
	transform_node->ec_transform=_entity->components[EC_TRANSFORM];

	TransformNodeData *data = ZG_NEW(TransformNodeData);
	transform_node->data=data;
	data->scene=_scene;
	return transform_node;
}


void 	 	TransformNode_Delete(TransformNode *_this){
	TransformNodeData *data = _this->data;

	ZG_FREE(data);
	ZG_FREE(_this);
}
