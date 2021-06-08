#ifndef _EC_TRANSFORM_ANIMATION_H__
#define _EC_TRANSFORM_ANIMATION_H__

typedef struct ECTransformAnimation ECTransformAnimation;

struct ECTransformAnimation{
	Entity *entity;
	void *data;
};

ComponentList ECTransformAnimation_RequirementsComponents();
void	ECTransformAnimation_Setup(void *_this);
void	ECTransformAnimation_Init(void *_this,Entity *_entity);
void 	ECTransformAnimation_Update(void *_this);
void 	ECTransformAnimation_Destroy(void *_this);

#endif
