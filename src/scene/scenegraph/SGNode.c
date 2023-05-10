#include "scene/zg_scene.h"

typedef struct{
	ECTransform 			*	ec_transform;
	ECTransformAnimation 	*	ec_transform_animation;
}SGNodeData;

SGNode *	SGNode_New(Scene *_scene,Entity *_entity){

	if(_entity == NULL){
		Log_ErrorF("SGNode_New : Cannot create sg_node. Entity == NULL");
		return NULL;
	}

	SGNode *sg_node = ZG_NEW(SGNode);
	sg_node->scene=_scene;
	sg_node->entity=_entity;

	SGNodeData *data = ZG_NEW(SGNodeData);
	data->ec_transform=_entity->components[EC_TRANSFORM];
	data->ec_transform_animation=_entity->components[EC_TRANSFORM_ANIMATION];
	sg_node->data=data;
	return sg_node;
}

bool		SGNode_AttachNode(SGNode *_this, SGNode *_node){
	SGNodeData *data = _this->data;
	return ECTransform_Attach(
		data->ec_transform
		,(ECTransform *)_node->entity->components[EC_TRANSFORM]
	);
}

bool		SGNode_DetachNode(SGNode *_this, SGNode *_node){
	SGNodeData *data = _this->data;
	return ECTransform_Detach(
		data->ec_transform
		,_node->entity->components[EC_TRANSFORM]
	);
}

void 		SGNode_SetTranslate3f(SGNode *_this,float _x, float _y, float _z){
	SGNodeData *data = _this->data;
	ECTransform_SetTranslate3f(
		data->ec_transform
		,_x
		,_y
		,_z
	);
}

void 		SGNode_SetRotate3f(SGNode *_this,float _x, float _y, float _z){
	SGNodeData *data = _this->data;
	ECTransform_SetRotate3f(
		data->ec_transform
		,_x
		,_y
		,_z
	);
}

void 		SGNode_SetScale3f(SGNode *_this,float _x, float _y, float _z){
	SGNodeData *data = _this->data;
	ECTransform_SetScale3f(
		data->ec_transform
		,_x
		,_y
		,_z
	);
}

void 		SGNode_SetPosition2i(SGNode *_this,int _x, int _y){
	SGNodeData *data = _this->data;
	ECTransform_SetPosition2i(
		data->ec_transform
		,_x
		,_y
	);
}

void 		SGNode_SetDisplacement2i(SGNode *_this,int _x, int _y){
	SGNodeData *data = _this->data;
	ECTransform_SetDisplacement2i(
		data->ec_transform
		,_x
		,_y
	);
}


Vector2i	SGNode_GetPosition2i(SGNode *_this){
	SGNodeData *data = _this->data;
	return ECTransform_GetPosition2i(
		data->ec_transform
	);
}


void 	 	SGNode_Delete(SGNode *_this){
	SGNodeData *data = _this->data;



	ZG_FREE(data);
	ZG_FREE(_this);
}
