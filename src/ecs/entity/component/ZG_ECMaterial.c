#include "ecs/_zg_ecs_.h"

typedef struct{
	ZG_Material 				*material;

}ZG_ECMaterialData;

void ZG_ECMaterial_OnCreate(void *_this, ZG_Entity _entity){
	ZG_ECMaterial *ec_material=_this;
	ec_material->material=ZG_Material_DefaultValues();
}

void ZG_ECMaterial_OnDestroy(void *_this, ZG_Entity _entity){
	ZG_ECMaterial *ec_material=_this;
	ZG_Material_Delete(ec_material->material);
}

//-----

void ZG_ECMaterial_SetAlpha(void *_this, float alpha){
	ZG_ECMaterial *ec_material=_this;
	ZG_Material_SetAlpha(ec_material->material,alpha);
}

/**
 * ZG_Material animation
 */
void ZG_ECMaterial_SetMaterialAnimation(ZG_ECMaterial *_this, ZG_ACMaterialAnimation * _material_animation_component){
	ZG_ECMaterialData *data=_this->data;
	data->material_animation_component = _material_animation_component;
}

ZG_ACMaterialAnimation * ZG_ECMaterial_GetMaterialAnimation(ZG_ECMaterial *_this){
	ZG_ECMaterialData *data=_this->data;
	return data->material_animation_component;

}



