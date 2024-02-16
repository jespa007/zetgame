#include "ecs/zg_ecs.h"


typedef struct{
	ZG_ECTransformNode		*	parent;
	ZG_List					*	child_nodes;
	ZG_Transform 				transform_default;
	ZG_Transform 			*	transform_local;
	uint16_t					transform_attributes;
}ZG_ECTransformData;

void SetParent(ZG_ECTransformNode *_this, ZG_ECTransformNode *_parent);

void ClearChilds(ZG_ECTransformNode * node){
	ZG_ECTransformData *data=node->data;
	for(unsigned i = 0; i < data->child_nodes->count; i++){
		ClearChilds(data->child_nodes->items[i]);
	}

	SetParent(node,NULL);
	ZG_List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

ZG_ECTransformNode * ZG_ECTransformNode_New(void){

	ZG_ECTransformNode * sg_node = ZG_NEW(ZG_ECTransformNode);
	ZG_ECTransformData *data= ZG_NEW(ZG_ECTransformData);
	sg_node->data=data;

	data->transform_local=&data->transform_default;
	data->child_nodes=ZG_List_New();


	// by default...
	data->transform_attributes =
				TRANSFORM_SCALE
			| 	TRANSFORM_TRANSLATE
			|	TRANSFORM_ROTATE;

	//sg_node->sgnode_type=ZG_ECTransformTypeNode;

	return sg_node;
}


void ZG_ECTransformNode_ClearNodes(ZG_ECTransformNode *_this){

	if(_this == NULL) return;

	ZG_ECTransformData *data = _this->data;


	for(unsigned i=0; i < data->child_nodes->count; i++){
		ClearNodes(data->child_nodes->items[i]);
	}

	ZG_List_Clear(data->child_nodes);
}

void ZG_ECTransformNode_SetTranslate3f(ZG_ECTransformNode *_this,float x, float y, float z){
	ZG_Transform_SetTranslate3f(&_this->transform,x,y,z);
}

bool ZG_ECTransformNode_IsParentNodeRoot(ZG_ECTransformNode *_this){
	ZG_ECTransformData *data=_this->data;
	if(data->parent != NULL){
		ZG_ECTransformData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}

void	ZG_ECTransformNode_SetParent(ZG_ECTransformNode *_this, ZG_ECTransformNode *parent_node){
	ZG_ECTransformData *data = _this->data;
	data->parent=parent_node;
}

ZG_ECTransformNode	*	ZG_ECTransformNode_GetParent(ZG_ECTransformNode *_this){
	ZG_ECTransformData *data = _this->data;
	return data->parent;
}

bool ZG_ECTransformNode_DetachNode(ZG_ECTransformNode *_this,ZG_ECTransformNode * obj) {

	ZG_ECTransformData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		ZG_ECTransformData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			ZG_LOG_ERROR("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	ZG_ECTransformNode_SetParent(obj,NULL);

	// already deattached
	return true;

}

bool ZG_ECTransformNode_AttachNode(ZG_ECTransformNode *_this,ZG_ECTransformNode * obj) {

	ZG_ECTransformData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!DetachNode(_this,obj)){
		return false;
	}

	SetParent(obj,_this);
	ZG_List_Add(data->child_nodes,obj);

	return false;
}

bool ZG_ECTransformNode_Detach(ZG_ECTransformNode *_this){

	ZG_ECTransformNode * parent = GetParent(_this);
	if(parent != NULL){
		DetachNode(parent,_this);
	}

	return true;

}

void ZG_ECTransformNode_UpdateChilds(ZG_ECTransformNode *_this) {

	ZG_ECTransformData *data = _this->data;
	ZG_ECTransformNode *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(unsigned i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		Update(o);
	}
}

void ZG_ECTransformNode_PostUpdate(ZG_ECTransformNode *_this){
	ZG_ECTransformNode_UpdateChilds(_this);
}

//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void ZG_ECTransformNode_UpdateSceneGraph(ZG_ECTransformNode *_this) {

	ZG_ECTransformData *data = _this->data;

	ZG_Quaternion local_quaternion;
	ZG_Transform *transform_world=&_this->transform;
	ZG_Transform *transform_local=data->transform_local;

	// transfer local -> absolute ...
	*transform_world=*transform_local;

	// todo: quaternions
	_this->transform.quaternion = transform_world->quaternion = local_quaternion = ZG_Quaternion_FromEulerV3f(transform_local->rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(!ZG_TransformNodeIsParentNodeRoot(_this)) { // Conditioned to transformations of m_scrParent....

		ZG_ECTransformNode *parent=data->parent;
		if(parent == NULL){
			ZG_LOG_ERROR("Expected parent not null!!");
			return;
		}

		//ZG_ECTransformData *parent_data = parent->data;
		ZG_Transform *parent_transform_world=&parent->transform;

		// todo: quaternions
		//ZG_Quaternion total_quaternion=parent->quaternion;//getTransform()getActualRotateMatrix());

		// ok. Let's to transform position child from rotation m_scrParent value ...
		ZG_Vector3f transform_child_from_parent=transform_local->translate;

		// the is propagated ...
		if(((data->transform_attributes & ZG_ECTRANSFORM_NODE_SCALE) == ZG_ECTRANSFORM_NODE_SCALE)) {
			// transforms the scale ...
			transform_world->scale=ZG_Vector3f_Mul(transform_world->scale,parent_transform_world->scale);
		}
		// Set origin translation ...
		if(((data->transform_attributes & TRANSFORM_TRANSLATE) == TRANSFORM_TRANSLATE)){
			transform_world->translate=parent_transform_world->translate;

			// Scale the translation...
			transform_child_from_parent=ZG_Vector3f_Mul(transform_child_from_parent,parent_transform_world->scale);

		}else{
			transform_world->translate=transform_local->translate;
		}

		transform_child_from_parent=ZG_Quaternion_InverseTransformV3f(parent_transform_world->quaternion,transform_child_from_parent);
		transform_world->translate=ZG_Vector3f_Add(transform_world->translate,transform_child_from_parent);
		transform_world->quaternion=ZG_Quaternion_Mul(local_quaternion,parent_transform_world->quaternion);

	}
	else { // Is the root, then add origin on their initial values ...

		/*ZG_Vector3f origin=ZG_ViewPort_GetProjectionOrigin();
		if(_this->transform->transform_properties & TRANSFORM_PROPERTY_POSITION_RELATIVE_X){ //  add x offset origin according opengl
			transform_absolute->translate.x+=origin.x;
		}

		if(_this->transform->transform_properties & TRANSFORM_PROPERTY_POSITION_RELATIVE_Y){ //  add x offset origin according opengl
			transform_absolute->translate.y+=origin.y;
		}*/

		if((data->transform_attributes & TRANSFORM_ROTATE)){
			transform_world->quaternion=local_quaternion;
		}
	}
}

void ZG_ECTransformNode_Update(ZG_ECTransformNode *_this) {

	ZG_ECTransformData *data = _this->data;
	if(data->parent!=NULL){ // it has parent, is not update
		return;
	}


	// update coord3d  scene graph...
	ZG_ECTransformNode_UpdateSceneGraph(_this);
	ZG_ECTransformNode_PostUpdate(_this);
}

ZG_Transform *ZG_ECTransformNode_GetTransform(ZG_ECTransformNode *_this, TransformNodeType sgtransform){
	ZG_ECTransformData *data=_this->data;
	if(sgtransform == TRANSFORM_NODE_TYPE_WORLD){
		return &_this->transform;
	}

	return data->transform_local;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 ZG_ECTransformNode_Delete(ZG_ECTransformNode *_this){

	if(_this==NULL) return;

	ZG_ECTransformData *_data = _this->data;

	ZG_ECTransformNode_ClearNodes(_this);
	ZG_List_Delete(_data->child_nodes);

	ZG_FREE(_data);
	ZG_FREE(_this);
}
