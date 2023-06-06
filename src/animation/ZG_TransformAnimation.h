#ifndef __ZG_TRANSFORM_ANIMATION_H__
#define __ZG_TRANSFORM_ANIMATION_H__


typedef struct ZG_TransformAnimation ZG_TransformAnimation;

struct ZG_TransformAnimation{

	void *data;
};

// STATIC
Action 				*	ZG_TransformAnimation_NewAction(void);


// PUBLIC
ZG_TransformAnimation 	*	ZG_TransformAnimation_New(void);
void					ZG_TransformAnimation_StartAction(
		ZG_TransformAnimation *_this
		, TransformAction *_action
		,uint32_t _start_time
		, int _repeat
);
void					ZG_TransformAnimation_StartTween(
	ZG_TransformAnimation *_this
	, uint32_t _start_time
	, uint8_t _idx_channel
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);
void 					ZG_TransformAnimation_Update(ZG_TransformAnimation *_this, ZG_Transform *_transform);
void 					ZG_TransformAnimation_Delete(ZG_TransformAnimation *_this);

#endif
