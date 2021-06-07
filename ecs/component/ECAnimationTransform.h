#ifndef _EC_ANIMATION_TRANSFORM_H__
#define _EC_ANIMATION_TRANSFORM_H__

typedef struct ECAnimationTransform ECAnimationTransform;

struct ECAnimationTransform{
	Entity *entity;
	void *data;
};

void	ECAnimationTransform_Setup(void *_this);
void	ECAnimationTransform_Init(void *_this,Entity *_entity);
void 	ECAnimationTransform_Update(void *_this);
void 	ECAnimationTransform_Destroy(void *_this);

#endif
