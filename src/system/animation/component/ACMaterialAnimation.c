#include "system/zg_system.h"

typedef struct{
	MaterialAnimation *ani_material;
}ACMaterialAnimationData;

static AComponent g_ec_material_animation_required_components[]={
		AC_MATERIAL
};


AComponentList ACMaterialAnimation_RequiredComponents(void){
	AComponentList cl;
	cl.components=g_ec_material_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_material_animation_required_components);

	return cl;
}

void	ACMaterialAnimation_Setup(void *_this, Entity *_entity){
	ACMaterialAnimation *ec_material_animation =_this;
	ec_material_animation->header.entity=_entity;
	ec_material_animation->header.id=AC_MATERIAL_ANIMATION;
	_entity->components[AC_MATERIAL_ANIMATION]=_this;

	ACMaterialAnimationData *data=ZG_NEW(ACMaterialAnimationData);
	data->ani_material=MaterialAnimation_New();

	ec_material_animation->data=data;
}

void	ACMaterialAnimation_Init(void *_this,Entity *_entity){
	ACMaterialAnimation *ec_material_animation=_this;
	ec_material_animation->header.entity=_entity;
}

void			ACMaterialAnimation_StartAction(
	void *_this
	, MaterialAction *action
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


	ACMaterial *ec_material=ec_material_animation->header.entity->components[AC_MATERIAL];
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
