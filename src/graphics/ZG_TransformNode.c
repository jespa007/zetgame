#include "@zg_graphics.h"


typedef struct{
	ZG_TransformNode		*	parent;
	ZG_List					*	child_nodes;
	ZG_Transform  				world_transform; // world coordinates
	ZG_Quaternion				world_quaternion;
	ZG_Transform 				local_transform;
	ZG_Quaternion				local_quaternion;
	uint16_t					transform_attributes;
}ZG_TransformNodeData;

void	ZG_TransformNode_SetParent(ZG_TransformNode *_this, ZG_TransformNode *_parent);
void 	ZG_TransformNode_UpdateTransformNode(ZG_TransformNode *_this);

void ZG_TransformNode_ClearChilds(ZG_TransformNode * node){
	ZG_TransformNodeData *data=node->data;
	for(unsigned i = 0; i < data->child_nodes->count; i++){
		ZG_TransformNode_ClearChilds(data->child_nodes->items[i]);
	}

	ZG_TransformNode_SetParent(node,NULL);
	ZG_List_Clear(data->child_nodes);
}
//------------------------------------------------------------------------------------

ZG_TransformNode * ZG_TransformNode_New(void){

	ZG_TransformNode * transform_node = ZG_NEW(ZG_TransformNode);
	ZG_TransformNodeData *data= ZG_NEW(ZG_TransformNodeData);

	data->world_transform=ZG_Transform_New();
	data->world_quaternion=ZG_Quaternion_New();

	data->local_transform=ZG_Transform_New();
	data->local_quaternion=ZG_Quaternion_New();

	data->child_nodes=ZG_List_New();


	// by default...
	data->transform_attributes =
				ZG_TRANSFORM_NODE_PROPERTY_SCALE
			| 	ZG_TRANSFORM_NODE_PROPERTY_TRANSLATE
			|	ZG_TRANSFORM_NODE_PROPERTY_ROTATE;

	transform_node->data=data;

	return transform_node;
}


void ZG_TransformNode_ClearNodes(ZG_TransformNode *_this){

	if(_this == NULL) return;

	ZG_TransformNodeData *data = _this->data;


	for(unsigned i=0; i < data->child_nodes->count; i++){
		ZG_TransformNode_ClearNodes(data->child_nodes->items[i]);
	}

	ZG_List_Clear(data->child_nodes);
}

void	ZG_TransformNode_Apply(ZG_TransformNode *_this){
	ZG_TransformNodeData *data = _this->data;
	ZG_Transform world_transform=data->world_transform;

	ZG_Matrix4f rotation_matrix4x4=ZG_Quaternion_ToMatrix4f(data->world_quaternion);

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Transform_GL_Apply(&world_transform,&rotation_matrix4x4);
			break;
	}
}

void	ZG_TransformNode_Restore(ZG_TransformNode *_this){
	ZG_TransformNodeData *data = _this->data;
	ZG_Transform world_transform=data->world_transform;

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Transform_GL_Restore(&world_transform);
			break;
	}
}


void ZG_TransformNode_SetTranslate3f(ZG_TransformNode *_this,float _x, float _y, float _z){
	ZG_TransformNodeData *data=_this->data;
	ZG_Transform_SetTranslate3f(&data->local_transform,_x,_y,_z);
}

void ZG_TransformNode_SetRotate3f(ZG_TransformNode *_this,float _x, float _y, float _z){
	ZG_TransformNodeData *data=_this->data;
	ZG_Transform_SetRotate3f(&data->local_transform,_x,_y,_z);
}

void ZG_TransformNode_SetDisplacement2i(ZG_TransformNode *_this, int _offset_x, int _offset_y){
	ZG_TransformNodeData *data=_this->data;
	ZG_Transform_SetDisplacement2i(&data->local_transform,_offset_x,_offset_y);
}

void ZG_TransformNode_SetPosition2i(ZG_TransformNode *_this, int _x,int _y){
	ZG_TransformNodeData *data=_this->data;
	ZG_Transform_SetPosition2i(&data->local_transform,_x,_y);
}


bool ZG_TransformNode_IsParentNodeRoot(ZG_TransformNode *_this){
	ZG_TransformNodeData *data=_this->data;
	return data->parent==NULL;
}

void	ZG_TransformNode_SetParent(ZG_TransformNode *_this, ZG_TransformNode *_parent){
	ZG_TransformNodeData *data = _this->data;
	data->parent=_parent;
}

ZG_TransformNode	*	ZG_TransformNode_GetParent(ZG_TransformNode *_this){
	ZG_TransformNodeData *data = _this->data;
	return data->parent;
}

bool ZG_TransformNode_Attach(ZG_TransformNode *_this,ZG_TransformNode * _transform_node) {

	ZG_TransformNodeData *data = _this->data;
	if(_transform_node == NULL){
		return false;
	}

	if(!ZG_TransformNode_Detach(_transform_node)){
		return false;
	}

	ZG_TransformNode_SetParent(_transform_node,_this);
	ZG_List_Add(data->child_nodes,_transform_node);

	return true;
}


bool ZG_TransformNode_Detach(ZG_TransformNode * _this) {

	ZG_TransformNodeData *data = _this->data;

	if(data->parent != NULL){ // Already parented, try to deattach from parent first
		ZG_TransformNodeData *parent_data = data->parent->data;
		if(!ZG_List_RemoveIfExist(parent_data->child_nodes,_this)){
			ZG_LOG_ERRORF("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	ZG_TransformNode_SetParent(_this,NULL);

	// already deattached
	return true;

}



void ZG_TransformNode_UpdateChilds(ZG_TransformNode *_this) {

	ZG_TransformNodeData *data = _this->data;
	ZG_TransformNode *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(unsigned i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		ZG_TransformNode_Update(o);
	}
}


void ZG_TransformNode_Update(ZG_TransformNode *_this) {

	// update coord3d  scene graph...
	ZG_TransformNode_UpdateTransformNode(_this);
	ZG_TransformNode_UpdateChilds(_this);
}

//--------------------------- MAIN UPDATE TRANSFORM NODE ----------------------------

void ZG_TransformNode_UpdateTransformNode(ZG_TransformNode *_this) {

	ZG_TransformNodeData *data = _this->data;

	ZG_Quaternion local_quaternion;
	ZG_Transform *world_transform=&data->world_transform;
	ZG_Quaternion *world_quaternion=&data->world_quaternion;
	ZG_Transform *local_transform=&data->local_transform;


	// transfer local -> absolute ...
	*world_transform=*local_transform;

	// todo: quaternions
	data->world_quaternion = *world_quaternion = local_quaternion = ZG_Quaternion_FromEulerV3f(local_transform->rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(!ZG_TransformNode_IsParentNodeRoot(_this)) { // Conditioned to transformations of m_scrParent....

		ZG_TransformNode *parent=data->parent;
		if(parent == NULL){
			ZG_LOG_ERRORF("Expected parent not null!!");
			return;
		}

		ZG_TransformNodeData *parent_data = parent->data;
		ZG_Transform *parent_world_transform=&parent_data->world_transform;


		// Transform position child from parent value ...
		ZG_Vector3f transform_translate_from_parent=local_transform->translate;

		if(((data->transform_attributes & ZG_TRANSFORM_NODE_PROPERTY_SCALE) == ZG_TRANSFORM_NODE_PROPERTY_SCALE)) {
			// transforms the scale ...
			world_transform->scale=ZG_Vector3f_Mul(world_transform->scale,parent_world_transform->scale);
		}

		if(((data->transform_attributes & ZG_TRANSFORM_NODE_PROPERTY_TRANSLATE) == ZG_TRANSFORM_NODE_PROPERTY_TRANSLATE)){

			// Set origin translation ...
			world_transform->translate=parent_world_transform->translate;

			// Rotate/scale the translation...
			transform_translate_from_parent=ZG_Vector3f_Mul(transform_translate_from_parent,parent_world_transform->scale);

		}else{
			world_transform->translate=local_transform->translate;
		}

		transform_translate_from_parent=ZG_Quaternion_InverseTransformV3f(parent_data->world_quaternion,transform_translate_from_parent);

		world_transform->translate=ZG_Vector3f_Add(world_transform->translate,transform_translate_from_parent);

		// Temporal way to calcule Euler angles rotation...
		world_transform->rotate=ZG_Vector3f_Add(world_transform->rotate,parent_world_transform->rotate);

		*world_quaternion=ZG_Quaternion_Mul(local_quaternion,parent_data->world_quaternion);
	}
	else { // Is the root, then add origin on their initial values ...

		if((data->transform_attributes & ZG_TRANSFORM_NODE_PROPERTY_ROTATE)){
			*world_quaternion=local_quaternion;
		}
	}
}

ZG_Transform *ZG_TransformNode_GetTransform(ZG_TransformNode *_this, ZG_TransformNodeType _transform_node_type){
	ZG_TransformNodeData *data=_this->data;
	if(_transform_node_type == ZG_TRANSFORM_NODE_TYPE_WORLD){
		return &data->world_transform;
	}

	return &data->local_transform;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 ZG_TransformNode_Delete(ZG_TransformNode *_this){

	if(_this==NULL) return;

	ZG_TransformNodeData *_data = _this->data;


	ZG_TransformNode_ClearNodes(_this);
	ZG_List_Delete(_data->child_nodes);

	ZG_FREE(_data);
	ZG_FREE(_this);
}
