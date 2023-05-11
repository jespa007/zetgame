#include "ecs/zg_ecs.h"

typedef struct{
	MaterialAnimation *ani_material;
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

	ECMaterialAnimationData *data=ZG_NEW(ECMaterialAnimationData);
	data->ani_material=MaterialAnimation_New();

	ec_material_animation->data=data;
}

void	ECMaterialAnimation_Init(void *_this,Entity *_entity){
	ECMaterialAnimation *ec_material_animation=_this;
	ec_material_animation->header.entity=_entity;
}

void			ECMaterialAnimation_StartAction(
	void *_this
	, MaterialAction *action
	, int repeat
){
	ECMaterialAnimation *ec_material_animation=_this;
	ECMaterialAnimationData *data=ec_material_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	MaterialAnimation_StartAction(
			data->ani_material
			,action
			,_start_time
			,repeat
	);
}

void ECMaterialAnimation_StartTween(
		  void *_this
		, MaterialComponent _material_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ECMaterialAnimation *ec_material_animation=_this;
	ECMaterialAnimationData *ec_material_animation_data=ec_material_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	MaterialAnimation_StartTween(
		  ec_material_animation_data->ani_material
		, _start_time
		, _material_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void 	ECMaterialAnimation_Update(void *_this){
	ECMaterialAnimation *ec_material_animation =_this;
	ECMaterialAnimationData *data=ec_material_animation->data;


	ECMaterial *ec_material=ec_material_animation->header.entity->components[EC_MATERIAL];
	if(ec_material != NULL){
		MaterialAnimation_Update(data->ani_material,ec_material->material);
	}
}

void	ECMaterialAnimation_Destroy(void *_this){
	ECMaterialAnimation *ec_material_animation =_this;
	ECMaterialAnimationData *data=ec_material_animation->data;
	MaterialAnimation_Delete(data->ani_material);
	ZG_FREE(data);
}
