#include "_zg_animation_.h"

typedef struct{
	ZG_Animation *animation;
}ZG_MaterialAnimationData;



ZG_MaterialAnimation *ZG_MaterialAnimation_New(void){
	ZG_MaterialAnimation *transform_animation=ZG_NEW(ZG_MaterialAnimation);
	ZG_MaterialAnimationData *data=ZG_NEW(ZG_MaterialAnimationData);
	data->animation=ZG_Animation_New(ZG_TRANSFORM_COMPONENT_MAX);

	transform_animation->data=data;

	return transform_animation;
}

void ZG_MaterialAnimation_Update(ZG_MaterialAnimation *_this, ZG_Material *_material){
	ZG_MaterialAnimationData *data=_this->data;
	ZG_Animation_Update(data->animation,SDL_GetTicks());
	ZG_Animation_CopyChannelValues(data->animation,&_material->color.r);

}

void					ZG_MaterialAnimation_StartAction(
		ZG_MaterialAnimation *_this
		, ZG_MaterialAction *_action
		,uint32_t _start_time
		, int _repeat
){
	ZG_MaterialAnimationData *data=_this->data;

	ZG_Animation_StartAction(
			data->animation
			,ZG_MaterialAction_GetAction(_action)
			,_start_time
			,_repeat
	);
}

void ZG_MaterialAnimation_StartTween(
		ZG_MaterialAnimation *_this
		, uint32_t _start_time
		, MaterialComponent _material_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	ZG_MaterialAnimationData *data=_this->data;
	ZG_Animation_StartTween(
		data->animation
		, _start_time
		, _material_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void ZG_MaterialAnimation_Delete(ZG_MaterialAnimation *_this){
	ZG_MaterialAnimationData *data=_this->data;

	ZG_Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


