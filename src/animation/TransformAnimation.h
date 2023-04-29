#ifndef __TRANSFORM_ANIMATION_H__
#define __TRANSFORM_ANIMATION_H__


typedef struct TransformAnimation TransformAnimation;

struct TransformAnimation{

	void *data;
};

// STATIC
Action 				*	TransformAnimation_NewAction(void);


// PUBLIC
TransformAnimation 	*	TransformAnimation_New(void);
void					TransformAnimation_StartAction(
		TransformAnimation *_this
		, TransformAction *_action
		,uint32_t _start_time
		, int _repeat
);
void					TransformAnimation_StartTween(
	TransformAnimation *_this
	, uint32_t _start_time
	, uint8_t _idx_channel
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);
void 					TransformAnimation_Update(TransformAnimation *_this, Transform *_transform);
void 					TransformAnimation_Delete(TransformAnimation *_this);

#endif
