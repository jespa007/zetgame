#ifndef __TRANSFORM_ACTION_H__
#define __TRANSFORM_ACTION_H__

typedef struct TransformAction TransformAction;

struct TransformAction{

	void *data;
};

// STATIC
Action 				*	TransformAction_NewAction(void);


// PUBLIC
TransformAction 	*	TransformAction_New(void);

void					TransformAction_SetKeyframesTrack(
		TransformAction *_this
		, uint8_t idx_channel
		, Ease ease
		,float *keyframe_points
		, size_t keyframe_points_count

);

Action *TransformAction_GetAction(TransformAction 	*_this);


void 	TransformAction_Delete(TransformAction 	*_this);


#endif
