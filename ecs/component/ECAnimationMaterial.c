#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_material;
}ECAnimationMaterialData;

void	ECAnimationMaterial_Setup(void *_this){
	ECAnimationMaterial *ec_ani_material =_this;
	ECAnimationMaterialData *data=NEW(ECAnimationMaterialData);
	data->ani_material=Animation_New(MATERIAL_CHANNEL_MAX);

	ec_ani_material->data=data;
}

void	ECAnimationMaterial_Init(void *_this,Entity *_entity){

}

void 	ECAnimationMaterial_Update(void *_this){
	ECAnimationMaterial *ec_ani_material =_this;
	ECAnimationMaterialData *data=ec_ani_material->data;

	if(Animation_Update(data->ani_material,SDL_GetTicks())){ // let animation do the move...
		ECMaterial *ec_material=Entity_GetComponent(ec_ani_material->entity,ENTITY_COMPONENT_MATERIAL);
		if(ec_material != NULL){
			Animation_CopyChannelValues(data->ani_material,&ec_material->material->color.r);
		}
	}
}

void	ECAnimationMaterial_Destroy(void *_this){
	ECAnimationMaterial *ec_ani_material =_this;
	ECAnimationMaterialData *data=ec_ani_material->data;
	Animation_Delete(data->ani_material);
	FREE(data);
}
