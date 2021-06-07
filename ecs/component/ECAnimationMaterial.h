#ifndef _EC_ANIMATION_MATERIAL_H__
#define _EC_ANIMATION_MATERIAL_H__

typedef struct ECAnimationMaterial ECAnimationMaterial;

struct ECAnimationMaterial{
	Entity *entity;
	void *data;
};

void	ECAnimationMaterial_Setup(void *_this);
void	ECAnimationMaterial_Init(void *_this,Entity *_entity);
void 	ECAnimationMaterial_Update(void *_this);
void 	ECAnimationMaterial_Destroy(void *_this);

#endif
