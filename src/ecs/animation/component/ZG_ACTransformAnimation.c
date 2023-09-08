#include "ecs/@zg_ecs.h"

typedef struct{
	ZG_TransformAnimation *ani_transform;
}ZG_ACTransformAnimationData;



void	ZG_ACTransformAnimation_Setup(ZG_AComponentHeader *_this, ZG_ComponentId _id){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ac_transform_animation->header.id=_id;

	ZG_ACTransformAnimationData *data=ZG_NEW(ZG_ACTransformAnimationData);
	data->ani_transform=TransformAnimation_New();

	ac_transform_animation->data=data;
}

void	ZG_ACTransformAnimation_Destroy(void *_this){
	ZG_ACTransformAnimation *ac_transform_animation =_this;
	ZG_ACTransformAnimationData *data=ac_transform_animation->data;
	ZG_TransformAnimation_Delete(data->ani_transform);
	ZG_FREE(data);
}

void			ZG_ACTransformAnimation_StartAction(ZG_AComponentHeader *_this, ZG_TransformAction *action, int repeat){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ZG_ACTransformAnimationData *data=ac_transform_animation->data;

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
		ZG_ACTransformAnimation *_this
		, ZG_TransformComponent _transform_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ZG_ACTransformAnimation *ac_transform_animation=_this;
	ZG_ACTransformAnimationData *ac_transform_animation_data=ac_transform_animation->data;

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


void 	ZG_ACTransformAnimation_Update(
		ZG_ACTransformAnimation *_this
){
	ZG_ACTransformAnimation *ac_transform_animation =_this;
	ZG_ACTransformAnimationData *data=ac_transform_animation->data;

	ZG_ECTransform *ac_transform=ac_transform_animation->header.entity->components[EC_TRANSFORM];
	if(ac_transform != NULL){
		ZG_Transform *local=ZG_ECTransform_GetTransform(ac_transform,ZG_ECTRANSFORM_TYPE_LOCAL);
		ZG_TransformAnimation_Update(data->ani_transform,local);
	}

}




void	ZG_ACTransformAnimation_UpdateECTransform(
		ZG_AComponentHeader *_this
	, ZG_EComponentHeader *_ec_transform
){

}
