#include "zg_scene.h"

typedef enum{
	SCENE_STATUS_STOP=0,
	SCENE_STATUS_RUNNING
}SceneStatus;

#define MAX_SG_NODES		100
#define MAX_SG_VIEWERS_2D	100
#define MAX_SG_TEXTS_2D		100


typedef struct{

	uint32_t			start_time;
	ZG_List 			* 	animations;
	ZG_List 			* 	movie_players;
	ZG_List 			* 	sprite2d_emitters;
	//TransformNode		*	node_root;
	//SceneState  	* 	current_state;
	//ZG_List			*	scene_states;
	SceneStatus 		scene_status;
	ZG_EntitySystem	*	entity_system;
	ZG_AnimationSystem *	animation_system;
	ZG_CollisionSystem *	collision_system;

	ZG_List			* 	sg_nodes;
	ZG_List			* 	sg_textures;
	ZG_List			* 	sg_textboxes;

	ZG_EntityManager	* 	em_sg_nodes;
	ZG_EntityManager	* 	em_sg_textures;
	ZG_EntityManager	* 	em_sg_textboxes;
	ZG_List			* 	entity_managers2d;

/*	ZG_List * appearances;
	ZG_List * transforms3d;
	ZG_List * transform_actions;
	ZG_List * transform_animations;
	ZG_List * sprite_emitters;*/
}SceneData;


Scene * Scene_New(void){
	Scene *scene=ZG_NEW(Scene);
	SceneData *data=ZG_NEW(SceneData);
	scene->data=data;

	data->entity_system=ZG_EntitySystem_New();
	data->animation_system=ZG_AnimationSystem_New(data->entity_system);
	data->collision_system=ZG_CollisionSystem_New(data->entity_system);
	data->sg_nodes=ZG_List_New();
	data->sg_textures=ZG_List_New();
	data->sg_textboxes=ZG_List_New();
	//scene->sg_render=SGRender_New();
	//data->node_root=TransformNode_New();

	//TransformNode_SetScene(data->node_root,scene);

	data->animations=ZG_List_New();
	//data->scene_states=ZG_List_New();
	data->scene_status=SCENE_STATUS_STOP;

	ZG_EComponent sg_node_entity_components[]={
			EC_TRANSFORM,
	};

	data->em_sg_nodes=ZG_EntitySystem_NewEntityManager(
		data->entity_system
		,"sg_nodes"
		,MAX_SG_NODES
		,sg_node_entity_components
		,ZG_ARRAY_SIZE(sg_node_entity_components)
	);//TextureNode_New());

	ZG_EComponent sg_textures_entity_components[]={
			EC_SPRITE_RENDERER
			,EC_TRANSFORM
	};

	data->em_sg_textures=ZG_EntitySystem_NewEntityManager(
		data->entity_system,"sg_textures"
		,MAX_SG_VIEWERS_2D
		,sg_textures_entity_components
		,ZG_ARRAY_SIZE(sg_textures_entity_components)
	);//TextureNode_New());


	ZG_EComponent sg_textboxes_entity_components[]={
			EC_TEXTBOX_RENDERER
			,EC_TRANSFORM
	};

	data->em_sg_textboxes=ZG_EntitySystem_NewEntityManager(
		data->entity_system,"sg_textboxes"
		,MAX_SG_TEXTS_2D
		,sg_textboxes_entity_components
		,ZG_ARRAY_SIZE(sg_textboxes_entity_components)
	);//TextureNode_New());


/*	data->sgnodes=ZG_List_New();
	data->sgnodes=ZG_List_New();
	data->sgnodes=ZG_List_New();
	data->sgnodes=ZG_List_New();
	data->sgnodes=ZG_List_New();*/

	return scene;
}
/*
void Scene_AddSceneState(Scene *_this
		, const char *name
		, ZG_Callback init
		, ZG_Callback update){
	SceneData *data = _this->data;
	SceneState *state=SceneState_New(_this,name,init,update);


	if(data->current_state==NULL){ // set current state
		data->current_state=state;
	}

	ZG_List_Add(data->scene_states,state);
}


void Scene_AttachNode(Scene *_this,TransformNode *node){

	SceneData *data = _this->data;
	TransformNode_AttachNode(data->node_root,node);
}
*/

bool Scene_IsRunning(Scene *_this){
	SceneData *data = _this->data;
	return data->scene_status == SCENE_STATUS_RUNNING;
}

uint32_t Scene_CallbackGetTimeScene(void *_this){
	return ((Scene*)(_this))->current_time;
}
/*
void Scene_AttachAnimation(Scene *_this,Animation *animation){

	if(animation == NULL) {
		ZG_LOG_ERROR("Animation null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->animations,animation)){
		ZG_LOG_ERROR("animation already exist in scene");
	}

	Animation_SetCallbackAnimationTime(animation,(CallbackAnimationTime){
		 .ptr_function=Scene_CallbackGetTimeScene
		,.user_data=_this
	});

	ZG_List_Add(data->animations,animation);
}

void Scene_AttachMoviePlayer(Scene *_this,MoviePlayer *movie_player){
	if(movie_player == NULL) {
		ZG_LOG_ERROR("movie_player null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->movie_players,movie_player)){
		ZG_LOG_ERROR("movie_player already exist in scene");
	}

	ZG_List_Add(data->movie_players,movie_player);
}*/


ZG_EntityManager * Scene_NewEntityManager(
		Scene *_this
		, const char *_id
		,uint16_t max_entities
		, unsigned * entity_components
		, size_t entity_components_len
){
	SceneData *data=_this->data;
	return ZG_EntitySystem_NewEntityManager(data->entity_system,_id,max_entities,entity_components,entity_components_len);
}

TransformNode *Scene_NewTransformNode(Scene *_this){
	SceneData *data=_this->data;
	TransformNode *sg_node=TransformNode_New(_this,ZG_ZG_EntityManager_NewEntity(data->em_sg_nodes));
	ZG_List_Add(data->sg_nodes,sg_node);
	return sg_node;
}

TextureNode *Scene_NewTextureNode(Scene *_this){
	SceneData *data=_this->data;
	TextureNode *sg_texture=TextureNode_New(_this,ZG_ZG_EntityManager_NewEntity(data->em_sg_textures));
	ZG_List_Add(data->sg_textures,sg_texture);
	return sg_texture;
}

TextBoxNode *Scene_NewTextBoxNode(Scene *_this){
	SceneData *data=_this->data;
	TextBoxNode *sg_textbox=TextBoxNode_New(_this,ZG_ZG_EntityManager_NewEntity(data->em_sg_textboxes));
	ZG_List_Add(data->sg_textboxes,sg_textbox);
	return sg_textbox;
}
/*
ZG_Entity * Scene_NewEntity(Scene *_this, ZG_EComponent * entity_components, size_t entity_components_len){
	SceneData *data=_this->data;

	return EntitySystem_NewEntity(data->entity_system,entity_components,entity_components_len);
}

ZG_Entity * Scene_NewEntityFromManager(Scene *_this, const char *_str_entity_manager){
	SceneData *data=_this->data;
	return EntitySystem_NewEntityFromManager(data->entity_system,_str_entity_manager);
}
*/

/*
void Scene_AttachSprite2dEmitter(Scene *_this,Sprite2dEmitter *sprite2d_emitter){
	if(sprite2d_emitter == NULL) {
		ZG_LOG_ERROR("sprite2d_emitter null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->sprite2d_emitters,sprite2d_emitter)){
		ZG_LOG_ERROR("sprite2d_emitter already exist in scene");
	}

	ZG_List_Add(data->sprite2d_emitters,sprite2d_emitter);
}

void Scene_SetBackgroundColor3i(Scene *_this,uint8_t r,uint8_t g,uint8_t b){

}*/

void Scene_Start(Scene *_this){
	SceneData *data = _this->data;
	data->start_time = SDL_GetTicks();


/*
	if(data->scene_states->count > 0){
		data->current_state = data->scene_states->items[0];

		if(data->current_state != NULL){
			SceneState_Start(data->current_state);
		}
	}*/

	if(_this->on_start.ptr_function!=NULL){
		_this->on_start.ptr_function(_this,_this->on_start.user_data);
	}

	data->scene_status=SCENE_STATUS_RUNNING;

}
/*
void Scene_Resume(Scene *_this){

}
void Scene_Pause(Scene *_this){

}

void Scene_OnPostRender(Scene *_this,ZG_Callback cf){

}
*/

void Scene_Stop(Scene *_this){
	SceneData *data = _this->data;
	data->scene_status=SCENE_STATUS_STOP;
}



void Scene_Update(Scene *_this){
	SceneData *data = _this->data;

	if(data->scene_status != SCENE_STATUS_RUNNING) return;

	//SGRender_Begin(_this->sg_render,NULL);

	_this->current_time = SDL_GetTicks() - data->start_time;

	for(unsigned i=0; i < data->animations->count;i++){
		Animation_Update(data->animations->items[i],_this->current_time);
	}

	ZG_EntitySystem_Update(data->entity_system);
	//TransformNode_Update(data->node_root);

	/*if(data->current_state != NULL){
		SceneState_Update(data->current_state);
	}*/

	if(_this->on_update.ptr_function!=NULL){
		_this->on_update.ptr_function(_this,_this->on_update.user_data);
	}


	//SGRender_End(_this->sg_render);
}


void Scene_Delete(Scene *_this){
	if(_this == NULL) return;
	SceneData *data = _this->data;

	//SGRender_Delete(_this->sg_render);
	//TransformNode_Delete(_data->node_root);


	/*for(unsigned i=0; i < _data->scene_states->count; i++){
		SceneState_Delete(_data->scene_states->items[i]);
	}

	ZG_List_Delete(_data->scene_states);*/
	ZG_EntitySystem_Delete(data->entity_system);
	ZG_AnimationSystem_Delete(data->animation_system);
	ZG_CollisionSystem_Delete(data->collision_system);

	ZG_List_Delete(data->sg_nodes);
	ZG_List_Delete(data->sg_textures);
	ZG_List_Delete(data->sg_textboxes);
	ZG_List_Delete(data->animations);
	ZG_List_Delete(data->movie_players);
	ZG_List_Delete(data->sprite2d_emitters);


	ZG_FREE(data);
	ZG_FREE(_this);
}
