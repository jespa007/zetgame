#include "ecs/zg_ecs.h"


typedef struct{
	TransformNode		*parent;
	List		*child_nodes;
	Transform 	transform_default;
	Transform 	*transform_local;
	uint16_t	transform_attributes;
}ECTransformData;

void SetParent(TransformNode *_this, TransformNode *_parent);

void ClearChilds(TransformNode * node){
	ECTransformData *data=node->data;
	for(unsigned i = 0; i < data->child_nodes->count; i++){
		ClearChilds(data->child_nodes->items[i]);
	}

	SetParent(node,NULL);
	List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

TransformNode * New(void){

	TransformNode * sg_node = ZG_NEW(TransformNode);
	ECTransformData *data= ZG_NEW(ECTransformData);
	sg_node->data=data;

	data->transform_local=&data->transform_default;
	data->child_nodes=List_New();


	// by default...
	data->transform_attributes =
				TRANSFORM_SCALE
			| 	TRANSFORM_TRANSLATE
			|	TRANSFORM_ROTATE;

	//sg_node->sgnode_type=ECTransformTypeNode;

	return sg_node;
}


void ClearNodes(TransformNode *_this){

	if(_this == NULL) return;

	ECTransformData *data = _this->data;


	for(unsigned i=0; i < data->child_nodes->count; i++){
		ClearNodes(data->child_nodes->items[i]);
	}

	List_Clear(data->child_nodes);
}

void SetTranslate3f(TransformNode *_this,float x, float y, float z){
	Transform_SetTranslate3f(&_this->transform,x,y,z);
}

bool IsParentNodeRoot(TransformNode *_this){
	ECTransformData *data=_this->data;
	if(data->parent != NULL){
		ECTransformData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}

void SetPosition2i(TransformNode *_this,int x, int y){
	ECTransformData *data=_this->data;
	Vector3f v=ViewPort_ScreenToWorldDimension2i(x,y);
	Transform_SetPosition2i(data->transform_local,v.x,v.y);
}

Vector2i	GetPosition2i(TransformNode *_this){

	return Transform_GetPosition2i(&_this->transform);
}

void SetRotateZ(TransformNode *_this,float z){
	ECTransformData *data=_this->data;
	Transform_SetRotateZ(data->transform_local,z);
}

void SetRotate3f(TransformNode *_this,float x, float y, float z){
	ECTransformData *data=_this->data;
	Transform_SetRotate3f(data->transform_local,x,y,z);
}

void SetScale3f(TransformNode *_this,float x, float y, float z){
	ECTransformData *data=_this->data;
	Transform_SetScale3f(data->transform_local,x,y,z);
}

void	SetParent(TransformNode *_this, TransformNode *parent_node){
	ECTransformData *data = _this->data;
	data->parent=parent_node;
}

TransformNode	*	GetParent(TransformNode *_this){
	ECTransformData *data = _this->data;
	return data->parent;
}


bool DetachNode(TransformNode *_this,TransformNode * obj) {

	ECTransformData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		ECTransformData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			Log_Error("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	SetParent(obj,NULL);

	// already deattached
	return true;

}

bool AttachNode(TransformNode *_this,TransformNode * obj) {

	ECTransformData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!DetachNode(_this,obj)){
		return false;
	}

	SetParent(obj,_this);
	List_Add(data->child_nodes,obj);

	return false;
}

bool Detach(TransformNode *_this){

	TransformNode * parent = GetParent(_this);
	if(parent != NULL){
		DetachNode(parent,_this);
	}

	return true;

}

void UpdateChilds(TransformNode *_this) {

	ECTransformData *data = _this->data;
	TransformNode *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(unsigned i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		Update(o);
	}
}

void PostUpdate(TransformNode *_this){
	UpdateChilds(_this);
}

//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void UpdateSceneGraph(TransformNode *_this) {

	ECTransformData *data = _this->data;

	Quaternion local_quaternion;
	Transform *transform_world=&_this->transform;
	Transform *transform_local=data->transform_local;

	// transfer local -> absolute ...
	*transform_world=*transform_local;

	// todo: quaternions
	_this->transform.quaternion = transform_world->quaternion = local_quaternion = Quaternion_FromEulerV3f(transform_local->rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(!IsParentNodeRoot(_this)) { // Conditioned to transformations of m_scrParent....

		TransformNode *parent=data->parent;
		if(parent == NULL){
			Log_Error("Expected parent not null!!");
			return;
		}

		//ECTransformData *parent_data = parent->data;
		Transform *parent_transform_world=&parent->transform;

		// todo: quaternions
		//Quaternion total_quaternion=parent->quaternion;//getTransform()getActualRotateMatrix());

		// ok. Let's to transform position child from rotation m_scrParent value ...
		Vector3f transform_child_from_parent=transform_local->translate;

		// the is propagated ...
		if(((data->transform_attributes & TRANSFORM_SCALE) == TRANSFORM_SCALE)) {
			// transforms the scale ...
			transform_world->scale=Vector3f_Mul(transform_world->scale,parent_transform_world->scale);
		}
		// Set origin translation ...
		if(((data->transform_attributes & TRANSFORM_TRANSLATE) == TRANSFORM_TRANSLATE)){
			transform_world->translate=parent_transform_world->translate;

			// Scale the translation...
			transform_child_from_parent=Vector3f_Mul(transform_child_from_parent,parent_transform_world->scale);

		}else{
			transform_world->translate=transform_local->translate;
		}

		transform_child_from_parent=Quaternion_InverseTransformV3f(parent_transform_world->quaternion,transform_child_from_parent);
		transform_world->translate=Vector3f_Add(transform_world->translate,transform_child_from_parent);
		transform_world->quaternion=Quaternion_Mul(local_quaternion,parent_transform_world->quaternion);

	}
	else { // Is the root, then add origin on their initial values ...

		/*Vector3f origin=ViewPort_GetProjectionOrigin();
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

void Update(TransformNode *_this) {

	ECTransformData *data = _this->data;
	if(data->parent!=NULL){ // it has parent, is not update
		return;
	}


	// update coord3d  scene graph...
	UpdateSceneGraph(_this);
	PostUpdate(_this);
}

Transform *GetTransform(TransformNode *_this, TransformNodeType sgtransform){
	ECTransformData *data=_this->data;
	if(sgtransform == TRANSFORM_NODE_TYPE_WORLD){
		return &_this->transform;
	}

	return data->transform_local;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 Delete(TransformNode *_this){

	if(_this==NULL) return;

	ECTransformData *_data = _this->data;

	ClearNodes(_this);
	List_Delete(_data->child_nodes);

	FREE(_data);
	FREE(_this);
}
