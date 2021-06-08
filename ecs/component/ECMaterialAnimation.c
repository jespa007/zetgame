#include "ecs/zg_ecs.h"

typedef struct{
	Animation *ani_material;
}ECMaterialAnimationData;

static unsigned g_ec_material_animation_required_components[]={
		ENTITY_COMPONENT_MATERIAL
};


ComponentList ECMaterialAnimation_RequiredComponents(void){
	ComponentList cl;
	cl.components=g_ec_material_animation_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_material_animation_required_components);

	return cl;
}

void	ECMaterialAnimation_Setup(void *_this){
	ECMaterialAnimation *ec_ani_material =_this;
	ECMaterialAnimationData *data=NEW(ECMaterialAnimationData);
	data->ani_material=Animation_New(MATERIAL_CHANNEL_MAX);

	ec_ani_material->data=data;
}

void	ECMaterialAnimation_Init(void *_this,Entity *_entity){

}

void 	ECMaterialAnimation_Update(void *_this){
	ECMaterialAnimation *ec_ani_material =_this;
	ECMaterialAnimationData *data=ec_ani_material->data;

	if(Animation_Update(data->ani_material,SDL_GetTicks())){ // let animation do the move...
		ECMaterial *ec_material=ec_ani_material->entity->components[ENTITY_COMPONENT_MATERIAL];
		if(ec_material != NULL){
			Animation_CopyChannelValues(data->ani_material,&ec_material->material->color.r);
		}
	}
}

void	ECMaterialAnimation_Destroy(void *_this){
	ECMaterialAnimation *ec_ani_material =_this;
	ECMaterialAnimationData *data=ec_ani_material->data;
	Animation_Delete(data->ani_material);
	FREE(data);
}
