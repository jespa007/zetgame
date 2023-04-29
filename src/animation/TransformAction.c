
#include "zg_animation.h"

typedef struct{
	Action *action;
}TransformActionData;



// PUBLIC
TransformAction 	*	TransformAction_New(void){
	TransformAction *transform_action=ZG_NEW(TransformAction);
	TransformActionData *data=ZG_NEW(TransformActionData);

	Action *action=Action_New(TRANSFORM_COMPONENT_MAX);
	data->action=action;

	transform_action->data=data;

	return transform_action;
}
void					TransformAction_SetKeyframesTrack(
		TransformAction *_this
		, uint8_t _idx_channel
		, Ease _ease
		,float *_keyframe_points
		, size_t _keyframe_points_count
){
	TransformActionData *data=_this->data;
	Action_SetKeyframesTrack(
		data->action
		,_idx_channel
		,_ease
		,_keyframe_points
		,_keyframe_points_count
	);

}

Action *TransformAction_GetAction(TransformAction 	*_this){
	TransformActionData *data=_this->data;

	return data->action;
}

void 					TransformAction_Delete(TransformAction *_this){
	TransformActionData *data=_this->data;
	Action_Delete(data->action);

	ZG_FREE(data);
	ZG_FREE(_this);
}
