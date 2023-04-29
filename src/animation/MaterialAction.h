#ifndef __MATERIAL_ACTION_H__
#define __MATERIAL_ACTION_H__

typedef struct MaterialAction MaterialAction;

struct MaterialAction{

	void *data;
};

// STATIC
Action 				*	MaterialAction_NewAction(void);


// PUBLIC
MaterialAction 	*	MaterialAction_New(void);

void					MaterialAction_SetKeyframesTrack(
		MaterialAction *_this
		, uint8_t idx_channel
		, Ease ease
		,float *keyframe_points
		, size_t keyframe_points_count

);

Action *MaterialAction_GetAction(MaterialAction 	*_this);


void 	MaterialAction_Delete(MaterialAction 	*_this);


#endif
