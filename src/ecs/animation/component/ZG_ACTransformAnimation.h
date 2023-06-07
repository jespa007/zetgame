#ifndef _AC_TRANSFORM_ANIMATION_H__
#define _AC_TRANSFORM_ANIMATION_H__

struct ACTransformAnimation{
	AComponentHeader header;
	void 			*data;
};

void			ACTransformAnimation_Setup(void *_this, ComponentId _idx);
void			ACTransformAnimation_Init(void *_this);

void			ACTransformAnimation_StartAction(
	void *_this
	, ZG_TransformAction *action
	, int repeat
);


void 			ACTransformAnimation_StartTween(
	void *_this
	, ZG_TransformComponent _transform_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void 			ACTransformAnimation_Update(void *_this);
void 			ACTransformAnimation_Destroy(void *_this);

#endif
