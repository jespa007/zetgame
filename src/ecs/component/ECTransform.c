#include "ecs/zg_ecs.h"


typedef struct{
	ECTransform		*parent;
	List		*child_nodes;
	//Transform 	transform_default;
	Transform 	transform_local;
	uint16_t	transform_attributes;
}ECTransformData;

void ECTransform_SetParent(ECTransform *_this, ECTransform *_parent);

void ECTransform_ClearChilds(ECTransform * node){
	ECTransformData *data=node->data;
	for(uint16_t i = 0; i < data->child_nodes->count; i++){
		ECTransform_ClearChilds(data->child_nodes->items[i]);
	}

	ECTransform_SetParent(node,NULL);
	List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

void ECTransform_Setup(void *_this, Entity *_entity){

	ECTransform * ec_transform = _this;
	ec_transform->header.entity=_entity;
	ec_transform->header.id=EC_TRANSFORM;
	_entity->components[EC_TRANSFORM]=_this;




	ECTransformData *data= ZG_NEW(ECTransformData);
	ec_transform->data=data;

	ec_transform->transform=Transform_DefaultValues();
	data->transform_local=Transform_DefaultValues();

	data->child_nodes=List_New();


	// by default...
	data->transform_attributes =
				EC_TRANSFORM_SCALE
			| 	EC_TRANSFORM_TRANSLATE
			|	EC_TRANSFORM_ROTATE;

	//sg_node->sgnode_type=ECTransformTypeNode;

}

void ECTransform_ClearNodes(ECTransform *_this){

	if(_this == NULL) return;

	ECTransformData *data = _this->data;


	for(uint16_t i=0; i < data->child_nodes->count; i++){
		ECTransform_ClearNodes(data->child_nodes->items[i]);
	}

	List_Clear(data->child_nodes);
}

void ECTransform_SetTranslate3f(ECTransform *_this,float x, float y, float z){
	ECTransformData *data=_this->data;
	Transform_SetTranslate3f(&data->transform_local,x,y,z);

	// erase relative flags flags...
	//data->transform_attributes&=~EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes&=~EC_TRANSFORM_POSITION_RELATIVE_Y;

}
/*
bool ECTransform_IsParentNodeRoot(ECTransform *_this){
	ECTransformData *data=_this->data;
	if(data->parent != NULL){
		ECTransformData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}
*/
void ECTransform_SetPosition2i(ECTransform *_this,int x, int y){
	ECTransformData *data=_this->data;
	//Vector3f v=ViewPort_ScreenToWorldDimension2i(x,y);
	//Transform_SetPosition2i(&data->transform_local,x,y);

	Transform_SetPosition2i(&data->transform_local, x,y);

	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_Y;
	//=ViewPort_ScreenToWorldDimension2i(x,y);
}

void ECTransform_SetDisplacement2i(ECTransform *_this,int x, int y){
	ECTransformData *data=_this->data;
	//Vector3f v=ViewPort_ScreenToWorldDimension2i(x,y);
	//Transform_SetPosition2i(&data->transform_local,x,y);
	Transform_SetDisplacement2i(&data->transform_local, x,y);

	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_X;
	//data->transform_attributes|=EC_TRANSFORM_POSITION_RELATIVE_Y;
	//=ViewPort_ScreenToWorldDimension2i(x,y);
}

Vector2i	ECTransform_GetPosition2i(ECTransform *_this){
	return Transform_GetPosition2i(&_this->transform);
}

void ECTransform_SetRotateZ(ECTransform *_this,float z){
	ECTransformData *data=_this->data;
	Transform_SetRotateZ(&data->transform_local,z);
}

void ECTransform_SetRotate3f(ECTransform *_this,float x, float y, float z){
	ECTransformData *data=_this->data;
	Transform_SetRotate3f(&data->transform_local,x,y,z);
}

void ECTransform_SetScale3f(ECTransform *_this,float x, float y, float z){
	ECTransformData *data=_this->data;
	Transform_SetScale3f(&data->transform_local,x,y,z);
}

void	ECTransform_SetParent(ECTransform *_this, ECTransform *parent_node){
	ECTransformData *data = _this->data;
	data->parent=parent_node;
}

ECTransform	*	ECTransform_GetParent(ECTransform *_this){
	ECTransformData *data = _this->data;
	return data->parent;
}


bool ECTransform_Detach(ECTransform *_this,ECTransform * obj) {
	UNUSUED_PARAM(_this);

	ECTransformData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		ECTransformData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			Log_ErrorF("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	ECTransform_SetParent(obj,NULL);

	// already deattached
	return true;

}

bool ECTransform_Attach(ECTransform *_this,ECTransform * obj) {

	ECTransformData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!ECTransform_Detach(_this,obj)){
		return false;
	}

	ECTransform_SetParent(obj,_this);
	List_Add(data->child_nodes,obj);

	return true;
}
/*
bool ECTransform_Detach(ECTransform *_this){

	ECTransform * parent = ECTransform_GetParent(_this);
	if(parent != NULL){
		ECTransform_Detach(parent,_this);
	}

	return true;

}*/
/*
void ECTransform_UpdateChilds(ECTransform *_this) {

	ECTransformData *data = _this->data;
	ECTransform *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(uint16_t i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		ECTransform_UpdateSceneGraph(o);
	}
}

void ECTransform_PostUpdate(ECTransform *_this){
	ECTransform_UpdateChilds(_this);
}*/

//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void ECTransform_UpdateSceneGraph(ECTransform *_this) {

	ECTransformData *data = _this->data;

	Quaternion local_quaternion;
	Quaternion world_quaternion;
	//Transform *transform_world=&_this->transform;
	//Transform *transform_local=&data->transform_local;

	// transfer local -> absolute ...
	_this->transform=data->transform_local;

	// todo: quaternions
	_this->quaternion = world_quaternion= local_quaternion = Quaternion_FromEulerV3f(data->transform_local.rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(data->parent != NULL) { // it's not root node....

		ECTransform *parent=data->parent;
		//ECTransformData *parent_data = parent->data;
		Transform parent_transform=parent->transform;

		// todo: quaternions
		//Quaternion parent_quaternion_world=parent->quaternion;//getTransform()getActualRotateMatrix());

		// ok. Let's to transform position child from rotation m_scrParent value ...
		Vector3f translate_from_parent=data->transform_local.translate;

		// the is propagated ...
		if(data->transform_attributes & EC_TRANSFORM_SCALE) {
			// transforms the scale ...
			_this->transform.scale=Vector3f_Mul(_this->transform.scale,parent_transform.scale);
		}
		// Set origin translation ...
		if(data->transform_attributes & EC_TRANSFORM_TRANSLATE){
			_this->transform.translate=parent_transform.translate;

			// Scale the translation...
			translate_from_parent=Vector3f_Mul(translate_from_parent,parent_transform.scale);

		}else{
			_this->transform.translate=data->transform_local.translate;
		}

		translate_from_parent=Quaternion_InverseTransformV3f(parent->quaternion,translate_from_parent);
		_this->transform.translate=Vector3f_Add(_this->transform.translate,translate_from_parent);
		_this->transform.rotate=Vector3f_Add(_this->transform.rotate,parent->transform.rotate);

		//_this->quaternion=Quaternion_Mul(local_quaternion,parent->quaternion);

	}
	else { // Is the root, then add origin on their initial values ...

		//Vector3f origin=ViewPort_GetProjectionOrigin();
		//if(data->transform_attributes & EC_TRANSFORM_POSITION_RELATIVE_X){ //  add x offset origin according opengl
		//	transform_world->translate.x+=origin.x;
		//}

		//if(data->transform_attributes & EC_TRANSFORM_POSITION_RELATIVE_Y){ //  add x offset origin according opengl
		//	transform_world->translate.y+=origin.y;
		//}

		if((data->transform_attributes & EC_TRANSFORM_ROTATE)){
			_this->quaternion=local_quaternion;
		}
	}
}

void ECTransform_UpdateChild(void *_this) {
	ECTransform *ec_transform=_this;
	ECTransformData *data = ec_transform->data;

	// update coord3d  scene graph...
	ECTransform_UpdateSceneGraph(ec_transform);

	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(uint16_t i=0; i < data->child_nodes->count; i++) {
		ECTransform *t=data->child_nodes->items[i];
		ECTransform_UpdateChild(t);
	}
}

void ECTransform_Update(void *_this) {
	ECTransform *ec_transform=_this;
	ECTransformData *data = ec_transform->data;
	if(data->parent!=NULL){ // it has parent, not update because it was updated before
		return;
	}

	ECTransform_UpdateChild(_this);

}

Transform *ECTransform_GetTransform(ECTransform *_this, ECTransformType ec_transform_type){
	ECTransformData *data=_this->data;
	if(ec_transform_type == EC_TRANSFORM_TYPE_WORLD){
		return &_this->transform;
	}

	return &data->transform_local;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 ECTransform_Destroy(void *_this){

	ECTransformData *_data = ((ECTransform *)_this)->data;

	ECTransform_ClearNodes(_this);
	List_Delete(_data->child_nodes);

	ZG_FREE(_data);
}
