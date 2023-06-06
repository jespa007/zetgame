
#include "zg_animation.h"

typedef struct{
	Action *action;
}ZG_MaterialActionData;


// PUBLIC
ZG_MaterialAction 	*	ZG_MaterialAction_New(void){
	ZG_MaterialAction *transform_action=ZG_NEW(ZG_MaterialAction);
	ZG_MaterialActionData *data=ZG_NEW(ZG_MaterialActionData);

	Action *action=Action_New(TRANSFORM_COMPONENT_MAX);
	data->action=action;

	transform_action->data=data;

	return transform_action;
}

void					ZG_MaterialAction_SetKeyframesTrack(
		ZG_MaterialAction *_this
		, MaterialComponent _material_component
		, Ease _ease
		,float *_keyframe_points
		, size_t _keyframe_points_count
){
	ZG_MaterialActionData *data=_this->data;
	Action_SetKeyframesTrack(
		data->action
		,_material_component
		,_ease
		,_keyframe_points
		,_keyframe_points_count
	);

}

Action *ZG_MaterialAction_GetAction(ZG_MaterialAction 	*_this){
	ZG_MaterialActionData *data=_this->data;

	return data->action;
}

void 					ZG_MaterialAction_Delete(ZG_MaterialAction *_this){
	ZG_MaterialActionData *data=_this->data;
	Action_Delete(data->action);

	ZG_FREE(data);
	ZG_FREE(_this);
}
