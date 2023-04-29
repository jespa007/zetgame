#include "zg_animation.h"

typedef struct{
	Animation *animation;
}TransformAnimationData;



TransformAnimation *TransformAnimation_New(void){
	TransformAnimation *transform_animation=ZG_NEW(TransformAnimation);
	TransformAnimationData *data=ZG_NEW(TransformAnimationData);
	data->animation=Animation_New(TRANSFORM_COMPONENT_MAX);

	transform_animation->data=data;

	return transform_animation;
}

void TransformAnimation_Update(TransformAnimation *_this, Transform *_transform){
	TransformAnimationData *data=_this->data;
	Animation_Update(data->animation,SDL_GetTicks());
	Animation_CopyChannelValues(data->animation,&_transform->translate.x);

}

void					TransformAnimation_StartAction(
		TransformAnimation *_this
		, TransformAction *_action
		,uint32_t _start_time
		, int _repeat
){
	TransformAnimationData *data=_this->data;

	Animation_StartAction(
			data->animation
			,TransformAction_GetAction(_action)
			,_start_time
			,_repeat
	);
}

void TransformAnimation_StartTween(
		TransformAnimation *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	TransformAnimationData *data=_this->data;
	Animation_StartTween(
		data->animation
		, _start_time
		, _idx_channel
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void TransformAnimation_Delete(TransformAnimation *_this){
	TransformAnimationData *data=_this->data;

	Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


