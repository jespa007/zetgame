#ifndef _EC_MATERIAL_ANIMATION_H__
#define _EC_MATERIAL_ANIMATION_H__

typedef struct ECMaterialAnimation ECMaterialAnimation;

struct ECMaterialAnimation{
	Entity *entity;
	void *data;
};

ComponentList	ECMaterialAnimation_RequiredComponents(void);
void			ECMaterialAnimation_Setup(void *_this);
void			ECMaterialAnimation_Init(void *_this,Entity *_entity);
void 			ECMaterialAnimation_Update(void *_this);
void 			ECMaterialAnimation_Destroy(void *_this);

#endif
