#ifndef __ZG_ECMATERIAL_H__
#define __ZG_ECMATERIAL_H__

typedef struct ZG_ECMaterial ZG_ECMaterial;

struct ZG_ECMaterial{
	ZG_EComponentHeader header;
	ZG_Material 		*material;
	void			*data;
};

void ZG_ECMaterial_Setup(void *_this, ZG_ComponentId _id);
void ZG_ECMaterial_SetAlpha(void *_this, float alpha);
void ZG_ECMaterial_SetMaterialAnimation(ZG_ECMaterial *_this, ZG_ACMaterialAnimation * _material_animation_component);
ZG_ACMaterialAnimation * ZG_ECMaterial_GetMaterialAnimation(ZG_ECMaterial *_this);
void ZG_ECMaterial_Destroy(void *_this);


#endif
