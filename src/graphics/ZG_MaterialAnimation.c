#include "@zg_graphics.h"

typedef struct{
	ZG_Animation *animation;
}ZG_MaterialAnimationData;



ZG_MaterialAnimation *ZG_MaterialAnimation_New(void){
	ZG_MaterialAnimation *material_animation=ZG_NEW(ZG_MaterialAnimation);
	ZG_MaterialAnimationData *data=ZG_NEW(ZG_MaterialAnimationData);
	data->animation=ZG_Animation_New(ZG_MATERIAL_COMPONENT_MAX);

	material_animation->data=data;

	return material_animation;
}

void ZG_MaterialAnimation_Update(ZG_MaterialAnimation *_this, ZG_Material *_material){
	ZG_MaterialAnimationData *data=_this->data;
	ZG_Animation_Update(data->animation,ZG_System_GetTicks());
	ZG_Animation_CopyChannelValues(data->animation,&_material->color.r);

}

void					ZG_MaterialAnimation_StartAction(
		ZG_MaterialAnimation *_this
		, ZG_MaterialAction *_action
		, int _repeat
){
	ZG_MaterialAnimationData *data=_this->data;

	ZG_Animation_StartAction(
			data->animation
			,ZG_MaterialAction_GetAction(_action)
			,ZG_System_GetTicks()
			,_repeat
	);
}

void ZG_MaterialAnimation_StartTween(
		ZG_MaterialAnimation *_this
		, ZG_MaterialComponent _material_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	ZG_MaterialAnimationData *data=_this->data;
	ZG_Animation_StartTween(
		data->animation
		, ZG_System_GetTicks()
		, _material_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void ZG_MaterialAnimation_Delete(ZG_MaterialAnimation *_this){
	ZG_MaterialAnimationData *data=_this->data;

	ZG_Animation_Delete(data->animation);
	ZG_FREE(data);
	ZG_FREE(_this);
}


