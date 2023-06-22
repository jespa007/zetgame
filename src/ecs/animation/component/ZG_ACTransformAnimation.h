#ifndef _ZG_ACTRANSFORM_ANIMATION_H__
#define _ZG_ACTRANSFORM_ANIMATION_H__

struct ZG_ACTransformAnimation{
	ZG_AComponentHeader header;
	void 			*data;
};

void			ZG_ACTransformAnimation_Setup(ZG_AComponentHeader *_this, ZG_ComponentId _idx);
void			ZG_ACTransformAnimation_Init(ZG_AComponentHeader *_this);
void 			ZG_ACTransformAnimation_Update(ZG_AComponentHeader *_this);
void 			ZG_ACTransformAnimation_Destroy(ZG_AComponentHeader *_this);

void			ZG_ACTransformAnimation_StartAction(
		ZG_ACTransformAnimation *_this
	, ZG_TransformAction *action
	, int repeat
);


void 			ZG_ACTransformAnimation_StartTween(
		ZG_ACTransformAnimation *_this
	, ZG_TransformComponent _transform_component
	, ZG_Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void	ZG_ACTransformAnimation_UpdateECTransform(
		ZG_AComponentHeader *_this
	, ZG_EComponentHeader *_ec_transform
);


#endif
