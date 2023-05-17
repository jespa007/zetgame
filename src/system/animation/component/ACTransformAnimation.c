#include "system/zg_system.h"

typedef struct{
	TransformAnimation *ani_transform;
}ACTransformAnimationData;

static AComponent g_ec_transform_animation_required_components[]={
		AC_TRANSFORM
};

AComponentList ACTransformAnimation_RequiredComponents(void){
	AComponentList cl;
	cl.components=g_ec_transform_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_transform_animation_required_components);

	return cl;
}

void	ACTransformAnimation_Setup(void *_this, Entity *_entity){
	ACTransformAnimation *ec_transform_animation=_this;
	ec_transform_animation->header.entity=_entity;
	ec_transform_animation->header.id=AC_TRANSFORM_ANIMATION;
	_entity->components[AC_TRANSFORM_ANIMATION]=_this;


	ACTransformAnimationData *data=ZG_NEW(ACTransformAnimationData);
	data->ani_transform=TransformAnimation_New();

	ec_transform_animation->data=data;
}

void			ACTransformAnimation_StartAction(void *_this, TransformAction *action, int repeat){
	ACTransformAnimation *ec_transform_animation=_this;
	ACTransformAnimationData *data=ec_transform_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	TransformAnimation_StartAction(
			data->ani_transform
			,action
			,_start_time
			,repeat
	);
}

void ACTransformAnimation_StartTween(
		  void *_this
		, TransformComponent _transform_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ACTransformAnimation *ec_transform_animation=_this;
	ACTransformAnimationData *ec_transform_animation_data=ec_transform_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	TransformAnimation_StartTween(
		  ec_transform_animation_data->ani_transform
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
	ACTransformAnimation *ec_transform_animation =_this;
	ACTransformAnimationData *data=ec_transform_animation->data;

	ACTransform *ec_transform=ec_transform_animation->header.entity->components[AC_TRANSFORM];
	if(ec_transform != NULL){
		Transform *local=ACTransform_GetTransform(ec_transform,AC_TRANSFORM_TYPE_LOCAL);
		TransformAnimation_Update(data->ani_transform,local);
	}

}


void	ACTransformAnimation_Destroy(void *_this){
	ACTransformAnimation *ec_transform_animation =_this;
	ACTransformAnimationData *data=ec_transform_animation->data;
	TransformAnimation_Delete(data->ani_transform);
	ZG_FREE(data);
}
