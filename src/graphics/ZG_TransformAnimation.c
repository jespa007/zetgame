#include "../animation/@zg_animation.h"

typedef struct{
	ZG_Animation *animation;
}ZG_TransformAnimationData;



ZG_TransformAnimation *ZG_TransformAnimation_New(void){
	ZG_TransformAnimation *transform_animation=ZG_NEW(ZG_TransformAnimation);
	ZG_TransformAnimationData *data=ZG_NEW(ZG_TransformAnimationData);
	data->animation=ZG_Animation_New(ZG_TRANSFORM_COMPONENT_MAX);

	transform_animation->data=data;

	return transform_animation;
}

void ZG_TransformAnimation_Update(ZG_TransformAnimation *_this, ZG_Transform *_transform){
	ZG_TransformAnimationData *data=_this->data;
	ZG_Animation_Update(data->animation,SDL_GetTicks());
	ZG_Animation_CopyChannelValues(data->animation,&_transform->translate.x);

}

void					ZG_TransformAnimation_StartAction(
		ZG_TransformAnimation *_this
		, ZG_TransformAction *_action
		, int _repeat
){
	ZG_TransformAnimationData *data=_this->data;

	ZG_Animation_StartAction(
			data->animation
			,ZG_TransformAction_GetAction(_action)
			,SDL_GetTicks()
			,_repeat
	);
}

void ZG_TransformAnimation_StartTween(
		ZG_TransformAnimation *_this
		, uint8_t _idx_channel
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	ZG_TransformAnimationData *data=_this->data;
	ZG_Animation_StartTween(
		data->animation
		, SDL_GetTicks()
		, _idx_channel
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void ZG_TransformAnimation_Delete(ZG_TransformAnimation *_this){
	ZG_TransformAnimationData *data=_this->data;

	ZG_Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


