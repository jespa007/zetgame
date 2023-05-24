#include "../../zg_component_system.h"

typedef struct{
	ACMaterialAnimation 	*	material_animation_component; // in case entity it has associated material animation component
}ECMaterialData;

void ECMaterial_Setup(void *_this, Entity *_entity){
	ECMaterial *ec_material=_this;
	ec_material->header.entity=_entity;
	ec_material->header.id=EC_MATERIAL;
	_entity->components[EC_MATERIAL]=_this;


	_entity->components[EC_MATERIAL]=_this;
	ec_material->material=NULL;//Material_New();
}

void ECMaterial_SetAlpha(void *_this, float alpha){
	ECMaterial *ec_material=_this;
	Material_SetAlpha(ec_material->material,alpha);
}

/**
 * Material animation
 */
void ECMaterial_SetMaterialAnimation(ECMaterial *_this, ACMaterialAnimation * _material_animation_component){
	ECMaterialData *data=_this->data;
	data->material_animation_component = _material_animation_component;
}

ACMaterialAnimation * ECMaterial_GetMaterialAnimation(ECMaterial *_this){
	ECMaterialData *data=_this->data;
	return data->material_animation_component;

}


void ECMaterial_Destroy(void *_this){
	ECMaterial *ec_material=_this;
	if(ec_material->material!=NULL){
		Material_Delete(ec_material->material);
	}
}
