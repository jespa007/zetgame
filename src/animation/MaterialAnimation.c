#include "zg_animation.h"

typedef struct{
	Animation *animation;
}MaterialAnimationData;



MaterialAnimation *MaterialAnimation_New(void){
	MaterialAnimation *transform_animation=ZG_NEW(MaterialAnimation);
	MaterialAnimationData *data=ZG_NEW(MaterialAnimationData);
	data->animation=Animation_New(TRANSFORM_COMPONENT_MAX);

	transform_animation->data=data;

	return transform_animation;
}

void MaterialAnimation_Update(MaterialAnimation *_this, Material *_material){
	MaterialAnimationData *data=_this->data;
	Animation_Update(data->animation,SDL_GetTicks());
	Animation_CopyChannelValues(data->animation,&_material->color.r);

}

void					MaterialAnimation_StartAction(
		MaterialAnimation *_this
		, MaterialAction *_action
		,uint32_t _start_time
		, int _repeat
){
	MaterialAnimationData *data=_this->data;

	Animation_StartAction(
			data->animation
			,MaterialAction_GetAction(_action)
			,_start_time
			,_repeat
	);
}

void MaterialAnimation_StartTween(
		MaterialAnimation *_this
		, uint32_t _start_time
		, MaterialComponent _material_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	MaterialAnimationData *data=_this->data;
	Animation_StartTween(
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

void MaterialAnimation_Delete(MaterialAnimation *_this){
	MaterialAnimationData *data=_this->data;

	Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


