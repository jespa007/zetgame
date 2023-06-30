#include "ecs/_zg_ecs_.h"

typedef struct{
	ZG_ComponentId 	material_animation_component; // in case entity it has associated material animation component
}ZG_ECMaterialData;

void ZG_ECMaterial_OnCreate(void *_this, ZG_Archetype *_archetype, int _component_offset){
	ZG_ECMaterial *ec_material=_this;
	ec_material->material=NULL;//ZG_Material_New();
}

void ZG_ECMaterial_OnDestroy(void *_this, ZG_Archetype *_archetype, int _component_offset){
	ZG_ECMaterial *ec_material=_this;
	if(ec_material->material!=NULL){
		ZG_Material_Delete(ec_material->material);
	}
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



