#ifndef __EC_MATERIAL_H__
#define __EC_MATERIAL_H__

typedef struct ECMaterial ECMaterial;

struct ECMaterial{
	EComponentHeader header;
	Material 		*material;
	void			*data;
};

void ECMaterial_Setup(void *_this, ComponentId _id);
void ECMaterial_SetAlpha(void *_this, float alpha);
void ECMaterial_SetMaterialAnimation(ECMaterial *_this, ACMaterialAnimation * _material_animation_component);
ACMaterialAnimation * ECMaterial_GetMaterialAnimation(ECMaterial *_this);
void ECMaterial_Destroy(void *_this);


#endif
