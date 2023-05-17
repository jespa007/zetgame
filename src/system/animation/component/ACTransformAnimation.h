#ifndef _AC_TRANSFORM_ANIMATION_H__
#define _AC_TRANSFORM_ANIMATION_H__

typedef struct ACTransformAnimation ACTransformAnimation;

struct ACTransformAnimation{
	AComponentHeader header;
	void 			*data;
};

AComponentList 	ACTransformAnimation_RequiredComponents(void);
void			ACTransformAnimation_Setup(void *_this, Entity *_entity);
void			ACTransformAnimation_Init(void *_this,Entity *_entity);

void			ACTransformAnimation_StartAction(
	void *_this
	, TransformAction *action
	, int repeat
);


void 			ACTransformAnimation_StartTween(
	void *_this
	, TransformComponent _transform_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void 			ACTransformAnimation_Update(void *_this);
void 			ACTransformAnimation_Destroy(void *_this);

#endif
