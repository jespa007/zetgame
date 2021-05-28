#include "zg_graphics.h"
/*

static void  *	Shape2dAnimation_OnAfterSetChannels(void *_channels, void *_shape2d){
	// update all controlled transform3d

	Shape2d   *shape2d= _shape2d;
	ChannelsInfo *channels=_channels;

	ChannelsInfo_CopyVarChannels(channels,&shape2d->selected_index);

	return NULL;
}


Shape2dAnimation * 	Shape2dAnimation_New(){
	Shape2dAnimation * s2d_animation=NEW(Shape2dAnimation);
	memset(s2d_animation,0,sizeof(Shape2dAnimation));
	s2d_animation->animation=Animation_New(SHAPE2D_CHANNEL_MAX);
	s2d_animation->animation->channels->reset=Shape2dChannel_Reset;
	s2d_animation->animation->channels->on_after_set_components.ptr_function=Shape2dAnimation_OnAfterSetChannels;
	return s2d_animation;
}

void 					Shape2dAnimation_AddShape2d(Shape2dAnimation *_this,Shape2d *s2d){
	Animation_AddControlledObject(_this->animation,s2d);
}

void 					Shape2dAnimation_EraseShape2d(Shape2dAnimation *_this,Shape2d *s2d){
	Animation_AddControlledObject(_this->animation,s2d);
}

void 					Shape2dAnimation_AddAction(Shape2dAnimation *_this,Shape2dAction * transform_action, bool _loop){
		if(_this==NULL)return;
		Animation_AddAction(_this->animation,transform_action->action,  _loop);
}

void 					Shape2dAnimation_TriggerAction(Shape2dAnimation *_this,Shape2dAction * transform_action, bool _loop, ChannelOperator _ani_operator_component){
	if(_this==NULL)return;
	Animation_TriggerAction(_this->animation,transform_action->action,  _loop,  _ani_operator_component);
}


void 					Shape2dAnimation_Delete(Shape2dAnimation * _this){
	if(_this != NULL){
		Animation_Delete(_this->animation);
		FREE(_this);
	}
}
*/
