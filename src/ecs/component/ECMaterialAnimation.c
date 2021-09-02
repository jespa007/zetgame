#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_material;
}ECMaterialAnimationData;

static EComponent g_ec_material_animation_required_components[]={
		EC_MATERIAL
};


EComponentList ECMaterialAnimation_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_material_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_material_animation_required_components);

	return cl;
}

void	ECMaterialAnimation_Setup(void *_this, Entity *_entity){
	ECMaterialAnimation *ec_material_animation =_this;
	ec_material_animation->header.entity=_entity;
	ec_material_animation->header.id=EC_MATERIAL_ANIMATION;
	_entity->components[EC_MATERIAL_ANIMATION]=_this;

	ECMaterialAnimationData *data=NEW(ECMaterialAnimationData);
	data->ani_material=Animation_New(MATERIAL_CHANNEL_MAX);

	ec_material_animation->data=data;
}

void	ECMaterialAnimation_Init(void *_this,Entity *_entity){
	ECMaterialAnimation *ec_material_animation=_this;
	ec_material_animation->header.entity=_entity;
}

void			ECMaterialAnimation_StartAction(void *_this, Action *action, int repeat){
	ECMaterialAnimation *ec_material_animation=_this;
	ECMaterialAnimationData *data=ec_material_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	Animation_StartAction(data->ani_material,action,_start_time,repeat);
}

void 	ECMaterialAnimation_Update(void *_this){
	ECMaterialAnimation *ec_material_animation =_this;
	ECMaterialAnimationData *data=ec_material_animation->data;

	if(Animation_Update(data->ani_material,SDL_GetTicks())){ // let animation do the move...
		ECMaterial *ec_material=ec_material_animation->header.entity->components[EC_MATERIAL];
		if(ec_material != NULL){
			Animation_CopyChannelValues(data->ani_material,&ec_material->material->color.r);
		}
	}
}

void	ECMaterialAnimation_Destroy(void *_this){
	ECMaterialAnimation *ec_material_animation =_this;
	ECMaterialAnimationData *data=ec_material_animation->data;
	Animation_Delete(data->ani_material);
	FREE(data);
}
