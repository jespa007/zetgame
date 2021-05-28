#include "core/ecs/zg_ecs.h"


typedef struct{
	ECTransform		*parent;
	List		*child_nodes;
	Transform 	transform_default;
	Transform 	*transform_local;
	uint16_t	transform_attributes;
}ECSGNodeData;

void ECSGNode_SetParent(ECTransform *_this, ECTransform *_parent);

void ECSGNode_ClearChilds(ECTransform * node){
	ECSGNodeData *data=node->data;
	for(unsigned i = 0; i < data->child_nodes->count; i++){
		ECSGNode_ClearChilds(data->child_nodes->items[i]);
	}

	ECSGNode_SetParent(node,NULL);
	List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

ECTransform * ECSGNode_New(void){

	ECTransform * sg_node = NEW(ECTransform);
	ECSGNodeData *data= NEW(ECSGNodeData);
	sg_node->data=data;

	data->transform_local=&data->transform_default;
	data->child_nodes=List_New();


	// by default...
	data->transform_attributes =
				TRANSFORM_SCALE
			| 	TRANSFORM_TRANSLATE
			|	TRANSFORM_ROTATE;

	//sg_node->sgnode_type=ECSGNodeTypeNode;

	return sg_node;
}


void ECSGNode_ClearNodes(ECTransform *_this){

	if(_this == NULL) return;

	ECSGNodeData *data = _this->data;


	for(unsigned i=0; i < data->child_nodes->count; i++){
		ECSGNode_ClearNodes(data->child_nodes->items[i]);
	}

	List_Clear(data->child_nodes);
}

void ECSGNode_SetTranslate3f(ECTransform *_this,float x, float y, float z){
	Transform_SetTranslate3f(&_this->transform,x,y,z);
}

bool ECSGNode_IsParentNodeRoot(ECTransform *_this){
	ECSGNodeData *data=_this->data;
	if(data->parent != NULL){
		ECSGNodeData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}

void ECSGNode_SetPosition2i(ECTransform *_this,int x, int y){
	ECSGNodeData *data=_this->data;
	Vector3f v=ViewPort_ScreenToWorldDim2i(x,y);
	Transform_SetPosition2i(data->transform_local,v.x,v.y);
}

Vector2i	ECSGNode_GetPosition2i(ECTransform *_this){

	return Transform_GetPosition2i(&_this->transform);
}

void ECSGNode_SetRotateZ(ECTransform *_this,float z){
	ECSGNodeData *data=_this->data;
	Transform_SetRotateZ(data->transform_local,z);
}

void ECSGNode_SetRotate3f(ECTransform *_this,float x, float y, float z){
	ECSGNodeData *data=_this->data;
	Transform_SetRotate3f(data->transform_local,x,y,z);
}

void ECSGNode_SetScale3f(ECTransform *_this,float x, float y, float z){
	ECSGNodeData *data=_this->data;
	Transform_SetScale3f(data->transform_local,x,y,z);
}

void	ECSGNode_SetParent(ECTransform *_this, ECTransform *parent_node){
	ECSGNodeData *data = _this->data;
	data->parent=parent_node;
}

ECTransform	*	ECSGNode_GetParent(ECTransform *_this){
	ECSGNodeData *data = _this->data;
	return data->parent;
}


bool ECSGNode_DetachNode(ECTransform *_this,ECTransform * obj) {

	ECSGNodeData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		ECSGNodeData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			Log_Error("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	ECSGNode_SetParent(obj,NULL);

	// already deattached
	return true;

}

bool ECSGNode_AttachNode(ECTransform *_this,ECTransform * obj) {

	ECSGNodeData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!ECSGNode_DetachNode(_this,obj)){
		return false;
	}

	ECSGNode_SetParent(obj,_this);
	List_Add(data->child_nodes,obj);

	return false;
}

bool ECSGNode_Detach(ECTransform *_this){

	ECTransform * parent = ECSGNode_GetParent(_this);
	if(parent != NULL){
		ECSGNode_DetachNode(parent,_this);
	}

	return true;

}

void ECSGNode_UpdateChilds(ECTransform *_this) {

	ECSGNodeData *data = _this->data;
	ECTransform *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(unsigned i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		ECSGNode_Update(o);
	}
}

void ECSGNode_PostUpdate(ECTransform *_this){
	ECSGNode_UpdateChilds(_this);
}

//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void ECSGNode_UpdateSceneGraph(ECTransform *_this) {

	ECSGNodeData *data = _this->data;

	Quaternion local_quaternion;
	Transform *transform_world=&_this->transform;
	Transform *transform_local=data->transform_local;

	// transfer local -> absolute ...
	*transform_world=*transform_local;

	// todo: quaternions
	_this->transform.quaternion = transform_world->quaternion = local_quaternion = Quaternion_FromEulerV3f(transform_local->rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(!ECSGNode_IsParentNodeRoot(_this)) { // Conditioned to transformations of m_scrParent....

		ECTransform *parent=data->parent;
		if(parent == NULL){
			Log_Error("Expected parent not null!!");
			return;
		}

		//ECSGNodeData *parent_data = parent->data;
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

void ECSGNode_Update(ECTransform *_this) {

	ECSGNodeData *data = _this->data;
	if(data->parent!=NULL){ // it has parent, is not update
		return;
	}


	// update coord3d  scene graph...
	ECSGNode_UpdateSceneGraph(_this);
	ECSGNode_PostUpdate(_this);
}

Transform *ECSGNode_GetTransform(ECTransform *_this, TransformNodeType sgtransform){
	ECSGNodeData *data=_this->data;
	if(sgtransform == TRANSFORM_NODE_TYPE_WORLD){
		return &_this->transform;
	}

	return data->transform_local;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 ECSGNode_Delete(ECTransform *_this){

	if(_this==NULL) return;

	ECSGNodeData *_data = _this->data;

	ECSGNode_ClearNodes(_this);
	List_Delete(_data->child_nodes);

	FREE(_data);
	FREE(_this);
}
