#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_transform;
}ECTransformAnimationData;

static unsigned g_ec_transform_animation_required_components[]={
		ENTITY_COMPONENT_TRANSFORM
};

ComponentList ECTransformAnimation_RequiredComponents(void){
	ComponentList cl;
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

void 	ECTransformAnimation_Update(void *_this){
	ECTransformAnimation *ec_ani_transform =_this;
	ECTransformAnimationData *data=ec_ani_transform->data;

	if(Animation_Update(data->ani_transform,SDL_GetTicks())){ // let animation do the move...
		ECTransform *ec_transform=ec_ani_transform->entity->components[ENTITY_COMPONENT_TRANSFORM];
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
