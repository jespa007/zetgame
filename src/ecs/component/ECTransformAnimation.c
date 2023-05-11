#include "ecs/zg_ecs.h"

typedef struct{
	TransformAnimation *ani_transform;
}ECTransformAnimationData;

static EComponent g_ec_transform_animation_required_components[]={
		EC_TRANSFORM
};

EComponentList ECTransformAnimation_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_transform_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_transform_animation_required_components);

	return cl;
}

void	ECTransformAnimation_Setup(void *_this, Entity *_entity){
	ECTransformAnimation *ec_transform_animation=_this;
	ec_transform_animation->header.entity=_entity;
	ec_transform_animation->header.id=EC_TRANSFORM_ANIMATION;
	_entity->components[EC_TRANSFORM_ANIMATION]=_this;


	ECTransformAnimationData *data=ZG_NEW(ECTransformAnimationData);
	data->ani_transform=TransformAnimation_New();

	ec_transform_animation->data=data;
}

void			ECTransformAnimation_StartAction(void *_this, TransformAction *action, int repeat){
	ECTransformAnimation *ec_transform_animation=_this;
	ECTransformAnimationData *data=ec_transform_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	TransformAnimation_StartAction(
			data->ani_transform
			,action
			,_start_time
			,repeat
	);
}

void ECTransformAnimation_StartTween(
		  void *_this
		, TransformComponent _transform_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ECTransformAnimation *ec_transform_animation=_this;
	ECTransformAnimationData *ec_transform_animation_data=ec_transform_animation->data;

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


void 	ECTransformAnimation_Update(void *_this){
	ECTransformAnimation *ec_transform_animation =_this;
	ECTransformAnimationData *data=ec_transform_animation->data;

	ECTransform *ec_transform=ec_transform_animation->header.entity->components[EC_TRANSFORM];
	if(ec_transform != NULL){
		Transform *local=ECTransform_GetTransform(ec_transform,EC_TRANSFORM_TYPE_LOCAL);
		TransformAnimation_Update(data->ani_transform,local);
	}

}


void	ECTransformAnimation_Destroy(void *_this){
	ECTransformAnimation *ec_transform_animation =_this;
	ECTransformAnimationData *data=ec_transform_animation->data;
	TransformAnimation_Delete(data->ani_transform);
	ZG_FREE(data);
}
