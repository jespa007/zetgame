#include "zg_scene.h"

typedef struct{
	char *name;
	uint32_t start_time;
	Callback on_start,on_update;
	void *user_data;
	Scene *scene;
}SceneStateData;


SceneState *SceneState_New(
		Scene *scene
		,const char *name
		,Callback on_start
		,Callback on_update){

	SceneState *state_state=ZG_NEW(SceneState);
	SceneStateData *data = ZG_NEW(SceneStateData);

	data->name=StrUtils_StrClone(name);
	data->on_start=on_start;
	data->on_update=on_update;
	data->scene=scene;

	return state_state;

}

void SceneState_Start(SceneState *_this){
	SceneStateData *data = _this->data;
	data->start_time=SDL_GetTicks();
	_this->current_time=0;
	if(data->on_start.ptr_function != NULL){
		data->on_start.ptr_function(_this,data->on_start.user_data);
	}

}

void SceneState_Update(SceneState *_this){
	SceneStateData *data = _this->data;
	_this->current_time=SDL_GetTicks()-data->start_time;
	if(data->on_update.ptr_function != NULL){
		data->on_update.ptr_function(_this,data->on_update.user_data);
	}

}

void SceneState_Delete(SceneState *_this){
	SceneStateData *data=_this->data;

	free(data->name);

	FREE(data);
	FREE(_this);
}
