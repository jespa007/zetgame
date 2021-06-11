#ifndef _EC_MATERIAL_ANIMATION_H__
#define _EC_MATERIAL_ANIMATION_H__

typedef struct ECMaterialAnimation ECMaterialAnimation;

struct ECMaterialAnimation{
	EComponentHeader;
	void 		*data;
};

EComponentList	ECMaterialAnimation_RequiredComponents(void);
void			ECMaterialAnimation_Setup(void *_this, Entity *_entity);
void			ECMaterialAnimation_Init(void *_this,Entity *_entity);
void			ECMaterialAnimation_StartAction(void *_this, Action *action, int repeat);
void 			ECMaterialAnimation_Update(void *_this);
void 			ECMaterialAnimation_Destroy(void *_this);

#endif
