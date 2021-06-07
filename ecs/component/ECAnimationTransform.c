#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_transform;
}ECAnimationTransformData;

void	ECAnimationTransform_Setup(void *_this){
	ECAnimationTransform *ec_ani_transform=_this;
	ECAnimationTransformData *data=NEW(ECAnimationTransformData);
	data->ani_transform=Animation_New(TRANSFORM_CHANNEL_MAX);

	ec_ani_transform->data=data;
}

void	ECAnimationTransform_Init(void *_this,Entity *_entity){

}

void 	ECAnimationTransform_Update(void *_this){
	ECAnimationTransform *ec_ani_transform =_this;
	ECAnimationTransformData *data=ec_ani_transform->data;

	if(Animation_Update(data->ani_transform,SDL_GetTicks())){ // let animation do the move...
		ECTransform *ec_transform=Entity_GetComponent(ec_ani_transform->entity,ENTITY_COMPONENT_TRANSFORM);
		if(ec_transform != NULL){
			Animation_CopyChannelValues(data->ani_transform,&ec_transform->transform.translate.x);
		}
	}
}


void	ECAnimationTransform_Destroy(void *_this){
	ECAnimationTransform *ec_ani_transform =_this;
	ECAnimationTransformData *data=ec_ani_transform->data;
	Animation_Delete(data->ani_transform);
	FREE(data);
}
