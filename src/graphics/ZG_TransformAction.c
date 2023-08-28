
#include "_zg_animation_.h"

typedef struct{
	ZG_Action *action;
}ZG_TransformActionData;



// PUBLIC
ZG_TransformAction 	*	ZG_TransformAction_New(void){
	ZG_TransformAction *transform_action=ZG_NEW(ZG_TransformAction);
	ZG_TransformActionData *data=ZG_NEW(ZG_TransformActionData);

	ZG_Action *action=ZG_Action_New(ZG_TRANSFORM_COMPONENT_MAX);
	data->action=action;

	transform_action->data=data;

	return transform_action;
}
void					ZG_TransformAction_SetKeyframesTrack(
		ZG_TransformAction *_this
		, ZG_TransformComponent _transform_component
		, ZG_Ease _ease
		,float *_keyframe_points
		, size_t _keyframe_points_count
){
	ZG_TransformActionData *data=_this->data;
	ZG_Action_SetKeyframesTrack(
		data->action
		,_transform_component
		,_ease
		,_keyframe_points
		,_keyframe_points_count
	);

}

ZG_Action *ZG_TransformAction_GetAction(ZG_TransformAction 	*_this){
	ZG_TransformActionData *data=_this->data;

	return data->action;
}

void 					ZG_TransformAction_Delete(ZG_TransformAction *_this){
	ZG_TransformActionData *data=_this->data;
	ZG_Action_Delete(data->action);

	ZG_FREE(data);
	ZG_FREE(_this);
}
