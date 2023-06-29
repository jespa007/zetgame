#include "ecs/zg_ecs.h"

typedef struct{
	ZG_ECTransform				*	parent;
	ZG_List					*	child_nodes;
	//ZG_Transform 	transform_default;
	ZG_Transform 					transform_local;
	uint16_t					transform_attributes;
	ZG_ACTransformAnimation 	*	transform_animation_component; // in case entity it has associated transform animation component
}ZG_ECTransformData;

void ZG_ECTransform_SetParent(ZG_ECTransform *_this, ZG_ECTransform *_parent);

void ZG_ECTransform_ClearChilds(ZG_ECTransform * node){
	ZG_ECTransformData *data=node->data;
	for(uint16_t i = 0; i < data->child_nodes->count; i++){
		ZG_ECTransform_ClearChilds(data->child_nodes->items[i]);
	}

	ZG_ECTransform_SetParent(node,NULL);
	ZG_List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

void ZG_ECTransform_OnCreate(void *_this, ZG_ComponentId _id){

	ZG_ECTransform * ec_transform = _this;
	//ec_transform->header.entity=_entity;
	ec_transform->header.id=_id;
	//_entity->components[EC_TRANSFORM]=_this;




	ZG_ECTransformData *data= ZG_NEW(ZG_ECTransformData);
	ec_transform->data=data;

	ec_transform->transform=ZG_Transform_DefaultValues();
	data->transform_local=ZG_Transform_DefaultValues();

	data->child_nodes=ZG_List_New();


	// by default...
	data->transform_attributes =
				ZG_ECTRANSFORM_SCALE
			| 	ZG_ECTRANSFORM_TRANSLATE
			|	ZG_ECTRANSFORM_ROTATE;

	//sg_node->sgnode_type=ZG_ECTransformTypeNode;

}


void ZG_ECTransform_OnUpdate(void *_this) {
	ZG_ECTransform *ec_transform=_this;
	ZG_ECTransformData *data = ec_transform->data;
	if(data->parent!=NULL){ // it has parent, not update because it was updated before
		return;
	}

	ZG_ECTransform_UpdateChild(_this);

}

void 	 ZG_ECTransform_OnDestroy(void *_this){

	ZG_ECTransformData *_data = ((ZG_ECTransform *)_this)->data;

	ZG_ECTransform_ClearNodes(_this);
	ZG_List_Delete(_data->child_nodes);

	ZG_FREE(_data);
}

//------------------------------------------------------------------------------------------------

void ZG_ECTransform_ClearNodes(ZG_ECTransform *_this){

	if(_this == NULL) return;

	ZG_ECTransformData *data = _this->data;


	for(uint16_t i=0; i < data->child_nodes->count; i++){
		ZG_ECTransform_ClearNodes(data->child_nodes->items[i]);
	}

	ZG_List_Clear(data->child_nodes);
}

void ZG_ECTransform_SetTranslate3f(ZG_ECTransform *_this,float x, float y, float z){
	ZG_ECTransformData *data=_this->data;
	ZG_Transform_SetTranslate3f(&data->transform_local,x,y,z);

	// erase relative flags flags...
	//data->transform_attributes&=~EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes&=~EC_TRANSFORM_POSITION_RELATIVE_Y;

}
/*
bool ZG_ECTransform_IsParentNodeRoot(ZG_ECTransform *_this){
	ZG_ECTransformData *data=_this->data;
	if(data->parent != NULL){
		ZG_ECTransformData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}
*/
void ZG_ECTransform_SetPosition2i(ZG_ECTransform *_this,int x, int y){
	ZG_ECTransformData *data=_this->data;
	//ZG_Vector3f v=ZG_ViewPort_ScreenToWorldDimension2i(x,y);
	//ZG_Transform_SetPosition2i(&data->transform_local,x,y);

	ZG_Transform_SetPosition2i(&data->transform_local, x,y);

	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_Y;
	//=ZG_ViewPort_ScreenToWorldDimension2i(x,y);
}

void ZG_ECTransform_SetDisplacement2i(ZG_ECTransform *_this,int x, int y){
	ZG_ECTransformData *data=_this->data;
	//ZG_Vector3f v=ZG_ViewPort_ScreenToWorldDimension2i(x,y);
	//ZG_Transform_SetPosition2i(&data->transform_local,x,y);
	ZG_Transform_SetDisplacement2i(&data->transform_local, x,y);

	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_Y;
	//=ZG_ViewPort_ScreenToWorldDimension2i(x,y);
}

ZG_Vector2i	ZG_ECTransform_GetPosition2i(ZG_ECTransform *_this){
	return ZG_Transform_GetPosition2i(&_this->transform);
}

void ZG_ECTransform_SetRotateZ(ZG_ECTransform *_this,float z){
	ZG_ECTransformData *data=_this->data;
	ZG_Transform_SetRotateZ(&data->transform_local,z);
}

void ZG_ECTransform_SetRotate3f(ZG_ECTransform *_this,float x, float y, float z){
	ZG_ECTransformData *data=_this->data;
	ZG_Transform_SetRotate3f(&data->transform_local,x,y,z);
}

void ZG_ECTransform_SetScale3f(ZG_ECTransform *_this,float x, float y, float z){
	ZG_ECTransformData *data=_this->data;
	ZG_Transform_SetScale3f(&data->transform_local,x,y,z);
}

void	ZG_ECTransform_SetParent(ZG_ECTransform *_this, ZG_ECTransform *parent_node){
	ZG_ECTransformData *data = _this->data;
	data->parent=parent_node;
}

ZG_ECTransform	*	ZG_ECTransform_GetParent(ZG_ECTransform *_this){
	ZG_ECTransformData *data = _this->data;
	return data->parent;
}


bool ZG_ECTransform_Detach(ZG_ECTransform *_this,ZG_ECTransform * obj) {
	ZG_UNUSUED_PARAM(_this);

	ZG_ECTransformData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		ZG_ECTransformData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			ZG_LOG_ERRORF("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	ZG_ECTransform_SetParent(obj,NULL);

	// already deattached
	return true;

}

bool ZG_ECTransform_Attach(ZG_ECTransform *_this,ZG_ECTransform * obj) {

	ZG_ECTransformData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!ZG_ECTransform_Detach(_this,obj)){
		return false;
	}

	ZG_ECTransform_SetParent(obj,_this);
	ZG_List_Add(data->child_nodes,obj);

	return true;
}


/*
bool ZG_ECTransform_Detach(ZG_ECTransform *_this){

	ZG_ECTransform * parent = ZG_ECTransform_GetParent(_this);
	if(parent != NULL){
		ZG_ECTransform_Detach(parent,_this);
	}

	return true;

}*/
/*
void ZG_ECTransform_UpdateChilds(ZG_ECTransform *_this) {

	ZG_ECTransformData *data = _this->data;
	ZG_ECTransform *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(uint16_t i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		ZG_ECTransform_UpdateSceneGraph(o);
	}
}

void ZG_ECTransform_PostUpdate(ZG_ECTransform *_this){
	ZG_ECTransform_UpdateChilds(_this);
}*/


/**
 * ZG_Transform animation
 */
void ZG_ECTransform_SetTransformAnimation(ZG_ECTransform *_this, ZG_ACTransformAnimation * _transform_animation_component){
	ZG_ECTransformData *data=_this->data;
	data->transform_animation_component = _transform_animation_component;
}

ZG_ACTransformAnimation * ZG_ECTransform_GetTransformAnimation(ZG_ECTransform *_this){
	ZG_ECTransformData *data=_this->data;
	return data->transform_animation_component;

}


//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void ZG_ECTransform_UpdateSceneGraph(ZG_ECTransform *_this) {

	ZG_ECTransformData *data = _this->data;

	ZG_Quaternion local_quaternion;
	ZG_Quaternion world_quaternion;
	//ZG_Transform *transform_world=&_this->transform;
	//ZG_Transform *transform_local=&data->transform_local;

	// transfer local -> absolute ...
	_this->transform=data->transform_local;

	// todo: quaternions
	_this->quaternion = world_quaternion= local_quaternion = ZG_Quaternion_FromEulerV3f(data->transform_local.rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(data->parent != NULL) { // it's not root node....

		ZG_ECTransform *parent=data->parent;
		//ZG_ECTransformData *parent_data = parent->data;
		ZG_Transform parent_transform=parent->transform;

		// todo: quaternions
		//ZG_Quaternion parent_quaternion_world=parent->quaternion;//getTransform()getActualRotateMatrix());

		// ok. Let's to transform position child from rotation m_scrParent value ...
		ZG_Vector3f translate_from_parent=data->transform_local.translate;

		// the is propagated ...
		if(data->transform_attributes & ZG_ECTRANSFORM_SCALE) {
			// transforms the scale ...
			_this->transform.scale=ZG_Vector3f_Mul(_this->transform.scale,parent_transform.scale);
		}
		// Set origin translation ...
		if(data->transform_attributes & ZG_ECTRANSFORM_TRANSLATE){
			_this->transform.translate=parent_transform.translate;

			// Scale the translation...
			translate_from_parent=ZG_Vector3f_Mul(translate_from_parent,parent_transform.scale);

		}else{
			_this->transform.translate=data->transform_local.translate;
		}

		translate_from_parent=ZG_Quaternion_InverseTransformV3f(parent->quaternion,translate_from_parent);
		_this->transform.translate=ZG_Vector3f_Add(_this->transform.translate,translate_from_parent);
		_this->transform.rotate=ZG_Vector3f_Add(_this->transform.rotate,parent->transform.rotate);

		//_this->quaternion=ZG_Quaternion_Mul(local_quaternion,parent->quaternion);

	}
	else { // Is the root, then add origin on their initial values ...

		//ZG_Vector3f origin=ZG_ViewPort_GetProjectionOrigin();
		//if(data->transform_attributes & EC_TRANSFORM_POSITION_RELATIVE_X){ //  add x offset origin according opengl
		//	transform_world->translate.x+=origin.x;
		//}

		//if(data->transform_attributes & EC_TRANSFORM_POSITION_RELATIVE_Y){ //  add x offset origin according opengl
		//	transform_world->translate.y+=origin.y;
		//}

		if((data->transform_attributes & ZG_ECTRANSFORM_ROTATE)){
			_this->quaternion=local_quaternion;
		}
	}
}

void ZG_ECTransform_UpdateChild(void *_this) {
	ZG_ECTransform *ec_transform=_this;
	ZG_ECTransformData *data = ec_transform->data;

	// update coord3d  scene graph...
	ZG_ECTransform_UpdateSceneGraph(ec_transform);

	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(uint16_t i=0; i < data->child_nodes->count; i++) {
		ZG_ECTransform *t=data->child_nodes->items[i];
		ZG_ECTransform_UpdateChild(t);
	}
}


ZG_Transform *ZG_ECTransform_GetTransform(ZG_ECTransform *_this, ZG_ECTransformType ec_transform_type){
	ZG_ECTransformData *data=_this->data;
	if(ec_transform_type == ZG_ECTRANSFORM_TYPE_WORLD){
		return &_this->transform;
	}

	return &data->transform_local;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v


