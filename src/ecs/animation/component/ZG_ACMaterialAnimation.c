#include "ecs/_zg_ecs_.h"

typedef struct{
	ZG_MaterialAnimation *ani_material;
}ZG_ACMaterialAnimationData;

//----
void	ZG_ACMaterialAnimation_Setup(void *_this, ZG_ComponentId _id){
	ZG_ACMaterialAnimation *ac_material_animation =_this;
	ac_material_animation->header.id=_id;

	ZG_ACMaterialAnimationData *data=ZG_NEW(ZG_ACMaterialAnimationData);
	data->ani_material=ZG_MaterialAnimation_New();

	ac_material_animation->data=data;
}

void 	ZG_ACMaterialAnimation_Update(void *_this){
	ZG_ACMaterialAnimation *ec_material_animation =_this;
	ZG_ACMaterialAnimationData *data=ec_material_animation->data;


	ZG_ECMaterial *ec_material=ec_material_animation->header.entity->components[EC_MATERIAL];
	if(ec_material != NULL){
		MaterialAnimation_Update(data->ani_material,ec_material->material);
	}
}


void	ZG_ACMaterialAnimation_Destroy(void *_this){
	ZG_ACMaterialAnimation *ec_material_animation =_this;
	ZG_ACMaterialAnimationData *data=ec_material_animation->data;
	ZG_MaterialAnimation_Delete(data->ani_material);
	ZG_FREE(data);
}
//----

void			ZG_ACMaterialAnimation_StartAction(
		ZG_ACMaterialAnimation *_this
	, ZG_MaterialAction *action
	, int repeat
){
	ZG_ACMaterialAnimation *ec_material_animation=_this;
	ZG_ACMaterialAnimationData *data=ec_material_animation->data;

	// own custom time
	uint32_t _start_time=SDL_GetTicks();

	ZG_MaterialAnimation_StartAction(
			data->ani_material
			,action
			,_start_time
			,repeat
	);
}

void ZG_ACMaterialAnimation_StartTween(
		ZG_ACMaterialAnimation *_this
		, ZG_MaterialComponent _material_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ZG_ACMaterialAnimation *ec_material_animation=_this;
	ZG_ACMaterialAnimationData *ec_material_animation_data=ec_material_animation->data;

	// define custom time
	uint32_t _start_time=SDL_GetTicks();

	ZG_MaterialAnimation_StartTween(
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

void	ZG_ACMaterialAnimation_UpdateECMaterial(
	ZG_AComponentHeader *_this
	, ZG_EComponentHeader *_material
){
	// in the animation system will get the material animation and a set material components and it will update
}


