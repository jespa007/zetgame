#include "ecs/zg_ecs.h"

typedef struct{
	MaterialAnimation *ani_material;
}ACMaterialAnimationData;


void	ACMaterialAnimation_Setup(void *_this, ComponentId _id){
	ACMaterialAnimation *ac_material_animation =_this;
	ac_material_animation->header.id=_id;

	ACMaterialAnimationData *data=ZG_NEW(ACMaterialAnimationData);
	data->ani_material=MaterialAnimation_New();

	ac_material_animation->data=data;
}

void			ACMaterialAnimation_StartAction(
	void *_this
	, ZG_MaterialAction *action
	, int repeat
){
	ACMaterialAnimation *ec_material_animation=_this;
	ACMaterialAnimationData *data=ec_material_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	MaterialAnimation_StartAction(
			data->ani_material
			,action
			,_start_time
			,repeat
	);
}

void ACMaterialAnimation_StartTween(
		  void *_this
		, MaterialComponent _material_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ACMaterialAnimation *ec_material_animation=_this;
	ACMaterialAnimationData *ec_material_animation_data=ec_material_animation->data;

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

void 	ACMaterialAnimation_Update(void *_this){
	ACMaterialAnimation *ec_material_animation =_this;
	ACMaterialAnimationData *data=ec_material_animation->data;


	ECMaterial *ec_material=ec_material_animation->header.entity->components[EC_MATERIAL];
	if(ec_material != NULL){
		MaterialAnimation_Update(data->ani_material,ec_material->material);
	}
}

void	ACMaterialAnimation_Destroy(void *_this){
	ACMaterialAnimation *ec_material_animation =_this;
	ACMaterialAnimationData *data=ec_material_animation->data;
	MaterialAnimation_Delete(data->ani_material);
	ZG_FREE(data);
}
