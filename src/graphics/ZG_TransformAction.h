#ifndef __ZG_TRANSFORM_ACTION_H__
#define __ZG_TRANSFORM_ACTION_H__

typedef struct ZG_TransformAction ZG_TransformAction;

struct ZG_TransformAction{

	void *data;
};

// STATIC
ZG_Action 				*	ZG_TransformAction_NewAction(void);


// PUBLIC
ZG_TransformAction 	*	ZG_TransformAction_New(void);

void					ZG_TransformAction_SetKeyframesTrack(
		ZG_TransformAction *_this
		, ZG_TransformComponent _transform_component
		, ZG_Ease ease
		,float *keyframe_points
		, size_t keyframe_points_count

);

ZG_Action *ZG_TransformAction_GetAction(ZG_TransformAction 	*_this);


void 	ZG_TransformAction_Delete(ZG_TransformAction 	*_this);


#endif
