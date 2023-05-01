
#include "zg_animation.h"

typedef struct{
	Action *action;
}MaterialActionData;


// PUBLIC
MaterialAction 	*	MaterialAction_New(void){
	MaterialAction *transform_action=ZG_NEW(MaterialAction);
	MaterialActionData *data=ZG_NEW(MaterialActionData);

	Action *action=Action_New(TRANSFORM_COMPONENT_MAX);
	data->action=action;

	transform_action->data=data;

	return transform_action;
}

void					MaterialAction_SetKeyframesTrack(
		MaterialAction *_this
		, uint8_t _idx_channel
		, Ease _ease
		,float *_keyframe_points
		, size_t _keyframe_points_count
){
	MaterialActionData *data=_this->data;
	Action_SetKeyframesTrack(
		data->action
		,_idx_channel
		,_ease
		,_keyframe_points
		,_keyframe_points_count
	);

}

Action *MaterialAction_GetAction(MaterialAction 	*_this){
	MaterialActionData *data=_this->data;

	return data->action;
}

void 					MaterialAction_Delete(MaterialAction *_this){
	MaterialActionData *data=_this->data;
	Action_Delete(data->action);

	ZG_FREE(data);
	ZG_FREE(_this);
}
