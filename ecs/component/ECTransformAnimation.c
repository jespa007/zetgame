#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_transform;
}ECTransformAnimationData;

static EComponent g_ec_transform_animation_required_components[]={
		ECOMPONENT_TRANSFORM
};

EComponentList ECTransformAnimation_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_transform_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_transform_animation_required_components);

	return cl;
}

void	ECTransformAnimation_Setup(void *_this){
	ECTransformAnimation *ec_ani_transform=_this;
	ECTransformAnimationData *data=NEW(ECTransformAnimationData);
	data->ani_transform=Animation_New(TRANSFORM_CHANNEL_MAX);

	ec_ani_transform->data=data;
}

void	ECTransformAnimation_Init(void *_this,Entity *_entity){

}

void ECTransformAnimation_StartTween(
		  void *_this
		, uint8_t _idx_channel
		, uint32_t _duration
		, Ease _ease
		, float _from
		, float _to
		, int _repeat){
	ECTransformAnimation *ec_transform_animation=_this;
	ECTransformAnimationData *ec_transform_animation_data=ec_transform_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	Animation_StartTween(
		  ec_transform_animation_data->ani_transform
		, _idx_channel
		, _start_time
		, _duration
		, _ease
		, _from
		, _to
		, _repeat
	);

}


void 	ECTransformAnimation_Update(void *_this){
	ECTransformAnimation *ec_ani_transform =_this;
	ECTransformAnimationData *data=ec_ani_transform->data;

	if(Animation_Update(data->ani_transform,SDL_GetTicks())){ // let animation do the move...
		ECTransform *ec_transform=ec_ani_transform->entity->components[ECOMPONENT_TRANSFORM];
		if(ec_transform != NULL){
			Animation_CopyChannelValues(data->ani_transform,&ec_transform->transform.translate.x);
		}
	}
}


void	ECTransformAnimation_Destroy(void *_this){
	ECTransformAnimation *ec_ani_transform =_this;
	ECTransformAnimationData *data=ec_ani_transform->data;
	Animation_Delete(data->ani_transform);
	FREE(data);
}
