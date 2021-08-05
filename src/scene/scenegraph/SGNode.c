#include "scene/zg_scene.h"


typedef struct{
	SGNode		*parent;
	List		*child_nodes;
	Transform *transform_default;
	Transform *transform_absolute;
	Appearance  *appearance_default;
	Scene		*scene;

	uint32_t		transform_attributes;



}SGNodeData;

void SGNode_SetParent(SGNode *_this, SGNode *_parent);


void SGNode_ClearChilds(SGNode * node){
	SGNodeData *data=node->data;
	for(unsigned i = 0; i < data->child_nodes->count; i++){
		SGNode_ClearChilds(data->child_nodes->items[i]);
	}

	SGNode_SetParent(node,NULL);
	List_Clear(data->child_nodes);
}


//------------------------------------------------------------------------------------

SGNode * SGNode_New(void){

	SGNode * sg_node = NEW(SGNode);
	SGNodeData *data= NEW(SGNodeData);
	sg_node->data=data;

	sg_node->transform=data->transform_default=Transform_New();
	sg_node->appearance=data->appearance_default=Appearance_New();

	data->transform_absolute=Transform_New();
	data->child_nodes=List_New();


	// by default...
	data->transform_attributes =
				SGNODE_TRANSFORM_SCALE
			| 	SGNODE_TRANSFORM_TRANSLATE
			|	SGNODE_TRANSFORM_ROTATE;

	//sg_node->sgnode_type=SGNodeTypeNode;

	return sg_node;
}


void SGNode_ClearNodes(SGNode *_this){

	if(_this == NULL) return;

	SGNodeData *data = _this->data;

	for(unsigned i=0; i < data->child_nodes->count; i++){
		SGNode_ClearNodes(data->child_nodes->items[i]);
	}

	List_Clear(data->child_nodes);
}

void SGNode_SetTranslate3f(SGNode *_this,float x, float y, float z){
	Transform_SetTranslate3f(_this->transform,x,y,z);
}

bool SGNode_IsParentNodeRoot(SGNode *_this){
	SGNodeData *data=_this->data;
	if(data->parent != NULL){
		SGNodeData *parent_data = data->parent->data;

		return (parent_data->parent==NULL);

	}else{ // is the scene itself
		return true;
	}
	return false;
}

void SGNode_SetPosition2i(SGNode *_this,int x, int y){
	Transform_SetPositionRelative2i(_this->transform,x,y);
}

Vector2i	SGNode_GetPosition2i(SGNode *_this){
	SGNodeData *data=_this->data;
	return Transform_GetPosition2i(data->transform_absolute);
}

void SGNode_SetRotateZ(SGNode *_this,float z){
	Transform_SetRotateZ(_this->transform,z);
}


void SGNode_SetRotate3f(SGNode *_this,float x, float y, float z){
	Transform_SetRotate3f(_this->transform,x,y,z);
}

void SGNode_SetScale3f(SGNode *_this,float x, float y, float z){
	Transform_SetScale3f(_this->transform,x,y,z);
}



/*void 		SGNode_SetDimensions3f(SGNode *_this,float width, float height, float depth){
	_this->dimensions=Vector3f_New3f(width,height,depth);
}

void 		SGNode_SetDimensions2i(SGNode *_this,uint16_t width, uint16_t height){
	_this->dimensions=ViewPort_ScreenToWorldDimv2i(Vector2i_New(width,height));

}*/
/*
bool SGNode_Is2d(SGNode *_this){
	return    _this->sgnode_type==SGNODE_TYPE_IMAGE
		   || _this->sgnode_type==SGNODE_TYPE_TEXTBOX;

}*/


Scene	*	SGNode_GetScene(SGNode *_this){
	SGNodeData *data = _this->data;
	return data->scene;
}

void SGNode_SetScene(SGNode *_this, Scene *scene){
	SGNodeData *data = _this->data;
	data->scene=scene;

	for(unsigned i = 0; i < data->child_nodes->count; i++){
		SGNode_SetScene(data->child_nodes->items[i], scene);
	}
}

void	SGNode_SetParent(SGNode *_this, SGNode *parent_node){
	Scene *scene=NULL;
	SGNodeData *data = _this->data;
	data->parent=parent_node;

	if(parent_node != NULL){
		scene=SGNode_GetScene(parent_node);
	}

	SGNode_SetScene(_this,scene);

	// set scene parent...
}

SGNode	*	SGNode_GetParent(SGNode *_this){
	SGNodeData *data = _this->data;
	return data->parent;
}


bool SGNode_DetachNode(SGNode *_this,SGNode * obj) {

	SGNodeData *obj_data = obj->data;

	if(obj_data->parent != NULL){ // Already parented, try to deattach from parent first
		SGNodeData *parent_data = obj_data->parent->data;
		if(!List_RemoveIfExist(parent_data->child_nodes,obj)){
			Log_Error("Cannot add node child because cannot deattach from parent");
			return false;
		}
	}

	SGNode_SetParent(obj,NULL);

	// already deattached
	return true;

}

bool SGNode_AttachNode(SGNode *_this,SGNode * obj) {

	SGNodeData *data = _this->data;
	if(obj == NULL){
		return false;
	}

	if(!SGNode_DetachNode(_this,obj)){
		return false;
	}

	SGNode_SetParent(obj,_this);
	List_Add(data->child_nodes,obj);

	return false;
}

bool SGNode_Detach(SGNode *_this){

	SGNode * parent = SGNode_GetParent(_this);
	if(parent != NULL){
		SGNode_DetachNode(parent,_this);
	}

	return true;

}

void SGNode_UpdateChilds(SGNode *_this) {

	SGNodeData *data = _this->data;

	unsigned i;
	SGNode *o;
	//-------- UPDATE TRANFORMS OF THEIR CHILDS ----
	for(i=0; i < data->child_nodes->count; i++) {
		o = data->child_nodes->items[i];
		SGNode_Update(o);
	}
}

void SGNode_PostUpdate(SGNode *_this){

	SGNode_UpdateChilds(_this);

}

//--------------------------- MAIN UPDATE SCENEGRAPH ----------------------------
void SGNode_UpdateSceneGraph(SGNode *_this) {

	SGNodeData *data = _this->data;

	Quaternion local_quaternion;
	Transform *transform_absolute=data->transform_absolute;
	Transform *transform_relative=_this->transform;

	// init transform_absolute equals to relative...
	*transform_absolute=*_this->transform;

	// todo: quaternions
	_this->transform->quaternion = transform_absolute->quaternion = local_quaternion = Quaternion_FromEulerV3f(transform_relative->rotate);

	//----------- ADD TRANSFORMATIONS ACCORD ITS PARENT ----------------
	if(!SGNode_IsParentNodeRoot(_this)) { // Conditioned to transformations of m_scrParent....

		SGNode *parent=data->parent;
		if(parent == NULL){
			Log_Error("Expected parent not null!!");
			return;
		}


		SGNodeData *parent_data = parent->data;
		Transform *parent_transform_absolute= parent_data->transform_absolute;

		// todo: quaternions
		//Quaternion total_quaternion=parent->quaternion;//getTransform()getActualRotateMatrix());

		// ok. Let's to transform position child from rotation m_scrParent value ...
		Vector3f transform_child_from_parent=transform_relative->translate;

		// the is propagated ...
		if(((data->transform_attributes & SGNODE_TRANSFORM_SCALE) == SGNODE_TRANSFORM_SCALE)) {
			// transforms the scale ...
			transform_absolute->scale=Vector3f_Mul(transform_absolute->scale,parent_transform_absolute->scale);
		}
		// Set origin translation ...
		if(((data->transform_attributes & SGNODE_TRANSFORM_TRANSLATE) == SGNODE_TRANSFORM_TRANSLATE)){
			transform_absolute->translate=parent_transform_absolute->translate;

			// Scale the translation...
			transform_child_from_parent=Vector3f_Mul(transform_child_from_parent,parent_transform_absolute->scale);

		}else{
			transform_absolute->translate=transform_relative->translate;
		}

		//transform_child_from_parent=Quaternion_Transformv3f(parent->quaternion,transform_child_from_parent);
		transform_child_from_parent=Quaternion_InverseTransformV3f(parent_transform_absolute->quaternion,transform_child_from_parent);
		//transform_child_from_parent=Matrix3f_TransformV3f(&parent_transform_absolute->,transform_child_from_parent);

		transform_absolute->translate=Vector3f_Add(transform_absolute->translate,transform_child_from_parent);
		transform_absolute->quaternion=Quaternion_Mul(local_quaternion,parent_transform_absolute->quaternion);

	}
	else { // Is the root, then add origin on their initial values ...

		Vector3f origin=ViewPort_GetProjectionOrigin();
		if(_this->transform->transform_properties & TRANSFORM_PROPERTY_POSITION_RELATIVE_X){ //  add x offset origin according opengl
			transform_absolute->translate.x+=origin.x;
		}

		if(_this->transform->transform_properties & TRANSFORM_PROPERTY_POSITION_RELATIVE_Y){ //  add x offset origin according opengl
			transform_absolute->translate.y+=origin.y;
		}

		if((data->transform_attributes & SGNODE_TRANSFORM_ROTATE)){
			transform_absolute->quaternion=local_quaternion;
		}
	}

	// save transformation matrix ...
	//transform_absolute->matrix3f=Quaternion_ToMatrix3f(transform_absolute->quaternion);

}

void SGNode_Update(SGNode *_this) {

	SGNodeData *data = _this->data;

	// update coord3d  scene graph...
	if(data->scene != NULL){

		SGNode_UpdateSceneGraph(_this);

		// if screen object not transparent ...
		if(_this->appearance!= NULL && _this->appearance->material->color.a > 0.0f){
			SGRender_AddNodeToDraw(data->scene->sg_render,_this);
		}

		SGNode_PostUpdate(_this);

	}
}


Transform *SGNode_GetTransform(SGNode *_this, SGNodeTransformType sgtransform){
	SGNodeData *data=_this->data;
	if(sgtransform == SGNODE_TRANSFORM_TYPE_WORLD){
		return data->transform_absolute;
	}

	return _this->transform;
}

void SGNode_SetOpacity(SGNode *_this,float alpha){
	_this->appearance->material->color.a=alpha;
}



void   SGNode_SetVisible(SGNode *_this,bool visible){

	if(visible){
		_this->appearance->material->color.a=ALPHA_VALUE_SOLID;
	}else{
		_this->appearance->material->color.a=ALPHA_VALUE_TRANSPARENT;
	}
}


void SGNode_SetColor3i(SGNode *_this,uint8_t r, uint8_t  b, uint8_t  g){
	Material_SetColor3i(_this->appearance->material,r,b,g);
}

float SGNode_GetOpacity(SGNode * _this){
	return _this->appearance->material->color.a;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------v

void 	 SGNode_Delete(SGNode *_this){
	if(_this==NULL) return;
	SGNodeData *_data = _this->data;

	SGNode_ClearNodes(_this);
	List_Delete(_data->child_nodes);

	Transform_Delete(_data->transform_default);
	Transform_Delete(_data->transform_absolute);
	Appearance_Delete(_data->appearance_default);


	FREE(_data);
	FREE(_this);
}
