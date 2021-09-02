#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_transform;
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


	ECTransformAnimationData *data=NEW(ECTransformAnimationData);
	data->ani_transform=Animation_New(TRANSFORM_CHANNEL_MAX);

	ec_transform_animation->data=data;
}

void ECTransformAnimation_StartTween(
		  void *_this
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ECTransformAnimation *ec_transform_animation=_this;
	ECTransformAnimationData *ec_transform_animation_data=ec_transform_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	Animation_StartTween(
		  ec_transform_animation_data->ani_transform
		, _start_time
		, _idx_channel
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

	if(Animation_Update(data->ani_transform,SDL_GetTicks())){ // let animation do the move...
		ECTransform *ec_transform=ec_transform_animation->header.entity->components[EC_TRANSFORM];
		if(ec_transform != NULL){
			Transform *local=ECTransform_GetTransform(ec_transform,EC_TRANSFORM_TYPE_LOCAL);
			Animation_CopyChannelValues(data->ani_transform,&local->translate.x);
		}
	}
}


void	ECTransformAnimation_Destroy(void *_this){
	ECTransformAnimation *ec_transform_animation =_this;
	ECTransformAnimationData *data=ec_transform_animation->data;
	Animation_Delete(data->ani_transform);
	FREE(data);
}
