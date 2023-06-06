#include "zg_animation.h"

typedef struct{
	Animation *animation;
}ZG_TransformActionData;



ZG_TransformAction *ZG_TransformAnimation_New(void){
	ZG_TransformAction *transform_animation=ZG_NEW(ZG_TransformAction);
	ZG_TransformActionData *data=ZG_NEW(ZG_TransformActionData);
	data->animation=ZG_Animation_New(TRANSFORM_COMPONENT_MAX);

	transform_animation->data=data;

	return transform_animation;
}

void ZG_TransformAnimation_Update(ZG_TransformAction *_this, Transform *_transform){
	ZG_TransformActionData *data=_this->data;
	ZG_Animation_Update(data->animation,SDL_GetTicks());
	ZG_Animation_CopyChannelValues(data->animation,&_transform->translate.x);

}

void					ZG_TransformAnimation_StartAction(
		ZG_TransformAction *_this
		, TransformAction *_action
		,uint32_t _start_time
		, int _repeat
){
	ZG_TransformActionData *data=_this->data;

	ZG_Animation_StartAction(
			data->animation
			,TransformAction_GetAction(_action)
			,_start_time
			,_repeat
	);
}

void ZG_TransformAnimation_StartTween(
		ZG_TransformAction *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	ZG_TransformActionData *data=_this->data;
	ZG_Animation_StartTween(
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

void ZG_TransformAnimation_Delete(ZG_TransformAction *_this){
	ZG_TransformActionData *data=_this->data;

	ZG_Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


