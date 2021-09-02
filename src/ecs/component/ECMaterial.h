#ifndef __EC_MATERIAL_H__
#define __EC_MATERIAL_H__

typedef struct ECMaterial ECMaterial;

struct ECMaterial{
	EComponentHeader header;
	Material 		*material;
};

void ECMaterial_Setup(void *_this, Entity *_entity);
void ECMaterial_SetAlpha(void *_this, float alpha);
void ECMaterial_Destroy(void *_this);


#endif
