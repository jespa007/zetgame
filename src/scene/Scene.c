#include "zg_scene.h"

typedef enum{
	SCENE_STATUS_STOP=0,
	SCENE_STATUS_RUNNING
}SceneStatus;


typedef struct{

	uint32_t			start_time;
	List 		* 	animations;
	List 		* 	movie_players;
	List 		* 	sprite2d_emitters;
	//SGNode		*	node_root;
	//SceneState  * 	current_state;
	//List		*	scene_states;
	SceneStatus 	scene_status;
	ESSystem		*es_system;

/*	List * appearances;
	List * transforms3d;
	List * transform_actions;
	List * transform_animations;
	List * sprite_emitters;*/
}SceneData;


Scene * Scene_New(void){
	Scene *scene=ZG_NEW(Scene);
	SceneData *data=ZG_NEW(SceneData);
	scene->data=data;

	data->es_system=ESSystem_New();
	//scene->sg_render=SGRender_New();
	//data->node_root=SGNode_New();

	//SGNode_SetScene(data->node_root,scene);

	data->animations=List_New();
	//data->scene_states=List_New();
	data->scene_status=SCENE_STATUS_STOP;

/*	data->sgnodes=List_New();
	data->sgnodes=List_New();
	data->sgnodes=List_New();
	data->sgnodes=List_New();
	data->sgnodes=List_New();*/

	return scene;
}
/*
void Scene_AddSceneState(Scene *_this
		, const char *name
		, Callback init
		, Callback update){
	SceneData *data = _this->data;
	SceneState *state=SceneState_New(_this,name,init,update);


	if(data->current_state==NULL){ // set current state
		data->current_state=state;
	}

	List_Add(data->scene_states,state);
}


void Scene_AttachNode(Scene *_this,SGNode *node){

	SceneData *data = _this->data;
	SGNode_AttachNode(data->node_root,node);
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
		Log_Error("Animation null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->animations,animation)){
		Log_Error("animation already exist in scene");
	}

	Animation_SetCallbackAnimationTime(animation,(CallbackAnimationTime){
		 .ptr_function=Scene_CallbackGetTimeScene
		,.user_data=_this
	});

	List_Add(data->animations,animation);
}

void Scene_AttachMoviePlayer(Scene *_this,MoviePlayer *movie_player){
	if(movie_player == NULL) {
		Log_Error("movie_player null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->movie_players,movie_player)){
		Log_Error("movie_player already exist in scene");
	}

	List_Add(data->movie_players,movie_player);
}*/

void Scene_NewEntityManager(Scene *_this, const char *_id,uint16_t max_entities, unsigned * entity_components, size_t entity_components_len){
	SceneData *data=_this->data;
	ESSystem_NewEntityManager(data->es_system,_id,max_entities,entity_components,entity_components_len);
}

Entity * Scene_NewEntity(Scene *_this, EComponent * entity_components, size_t entity_components_len){
	SceneData *data=_this->data;

	return ESSystem_NewEntity(data->es_system,entity_components,entity_components_len);
}

Entity * Scene_NewEntityFromManager(Scene *_this, const char *_str_entity_manager){
	SceneData *data=_this->data;
	return ESSystem_NewEntityFromManager(data->es_system,_str_entity_manager);
}


/*
void Scene_AttachSprite2dEmitter(Scene *_this,Sprite2dEmitter *sprite2d_emitter){
	if(sprite2d_emitter == NULL) {
		Log_Error("sprite2d_emitter null");
		return;
	}

	SceneData *data = _this->data;
	if(List_Exist(data->sprite2d_emitters,sprite2d_emitter)){
		Log_Error("sprite2d_emitter already exist in scene");
	}

	List_Add(data->sprite2d_emitters,sprite2d_emitter);
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

void Scene_OnPostRender(Scene *_this,Callback cf){

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

	ESSystem_Update(data->es_system);
	//SGNode_Update(data->node_root);

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
	//SGNode_Delete(_data->node_root);


	/*for(unsigned i=0; i < _data->scene_states->count; i++){
		SceneState_Delete(_data->scene_states->items[i]);
	}

	List_Delete(_data->scene_states);*/
	ESSystem_Delete(data->es_system);

	List_Delete(data->animations);
	List_Delete(data->movie_players);
	List_Delete(data->sprite2d_emitters);


	ZG_FREE(data);
	ZG_FREE(_this);
}
