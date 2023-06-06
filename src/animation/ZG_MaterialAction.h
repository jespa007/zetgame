#ifndef __ZG_MATERIAL_ACTION_H__
#define __ZG_MATERIAL_ACTION_H__

typedef struct ZG_MaterialAction ZG_MaterialAction;

struct ZG_MaterialAction{

	void *data;
};

// STATIC
Action 				*	ZG_MaterialAction_NewAction(void);


// PUBLIC
ZG_MaterialAction 	*	ZG_MaterialAction_New(void);

void					ZG_MaterialAction_SetKeyframesTrack(
		ZG_MaterialAction *_this
		, MaterialComponent _material_component
		, Ease ease
		,float *keyframe_points
		, size_t keyframe_points_count

);

Action *ZG_MaterialAction_GetAction(ZG_MaterialAction 	*_this);


void 	ZG_MaterialAction_Delete(ZG_MaterialAction 	*_this);


#endif
