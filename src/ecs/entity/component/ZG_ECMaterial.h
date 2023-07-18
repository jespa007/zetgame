#ifndef __ZG_ECMATERIAL_H__
#define __ZG_ECMATERIAL_H__

typedef struct ZG_ECMaterial ZG_ECMaterial;

struct ZG_ECMaterial{
	ZG_Material 				material;
	int 						ac_idx_material_animation; // in case entity it has associated material animation component
};

//---
void ZG_ECMaterial_OnCreate(void *_this, ZG_Entity _entity);
void ZG_ECMaterial_OnDestroy(void *_this, ZG_Entity _entity);
//---

void ZG_ECMaterial_SetAlpha(void *_this, float alpha);
void ZG_ECMaterial_SetMaterialAnimation(ZG_ECMaterial *_this, ZG_ACMaterialAnimation * _material_animation_component);
ZG_ACMaterialAnimation * ZG_ECMaterial_GetMaterialAnimation(ZG_ECMaterial *_this);



#endif
