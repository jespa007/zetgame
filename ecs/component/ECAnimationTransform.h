#ifndef _EC_TWEEN_TRANSFORM_H__
#define _EC_TWEEN_TRANSFORM_H__

typedef struct ECAnimationTransform ECAnimationTransform;

struct ECAnimationTransform{
	Entity *entity;
	void *data;
};

ECAnimationTransform *	ECAnimationTransform_Ini(void *_this,Entity *_entity);
void 					ECAnimationTransform_Delete(void *_this);

#endif
