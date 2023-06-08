#include "ecs/zg_ecs.h"

typedef struct{
	TransformAnimation *ani_transform;
}ACTransformAnimationData;



void	ACTransformAnimation_Setup(void *_this, ZG_ComponentId _id){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ac_transform_animation->header.id=_id;

	ACTransformAnimationData *data=ZG_NEW(ACTransformAnimationData);
	data->ani_transform=TransformAnimation_New();

	ac_transform_animation->data=data;
}

void			ACTransformAnimation_StartAction(void *_this, ZG_TransformAction *action, int repeat){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ACTransformAnimationData *data=ac_transform_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	TransformAnimation_StartAction(
			data->ani_transform
			,action
			,_start_time
			,repeat
	);
}

void ZG_ACTransformAnimation_StartTween(
		  void *_this
		, ZG_TransformComponent _transform_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ACTransformAnimationData *ac_transform_animation_data=ac_transform_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	TransformAnimation_StartTween(
		  ac_transform_animation_data->ani_transform
		, _start_time
		, _transform_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}


void 	ACTransformAnimation_Update(void *_this){
	ZG_ACTransformAnimation *ac_transform_animation =_this;
	ACTransformAnimationData *data=ac_transform_animation->data;

	ZG_ECTransform *ac_transform=ac_transform_animation->header.entity->components[EC_TRANSFORM];
	if(ac_transform != NULL){
		ZG_Transform *local=ZG_ECTransform_GetTransform(ac_transform,ZG_ECTRANSFORM_TYPE_LOCAL);
		TransformAnimation_Update(data->ani_transform,local);
	}

}


void	ACTransformAnimation_Destroy(void *_this){
	ZG_ACTransformAnimation *ac_transform_animation =_this;
	ACTransformAnimationData *data=ac_transform_animation->data;
	TransformAnimation_Delete(data->ani_transform);
	ZG_FREE(data);
}
