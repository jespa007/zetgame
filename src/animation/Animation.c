#include "zg_animation.h"

typedef struct{
	Action *action;
	uint32_t start_time;
	int repeat;
}InfoAction;


typedef struct{
	List *info_actions;
	Tween  *tween;
	ChannelsInfo 	*channels_info;
	Callback on_after_set_components;

}AnimationData;


InfoAction * InfoAniAction_New(Action *_action,uint32_t _start_time, int  _repeat){
	InfoAction * action_info = ZG_NEW(InfoAction);
	action_info->start_time=_start_time; // only for triggered info_actions...
	action_info->repeat=_repeat;
	action_info->action = _action;

	return action_info;
}

void Animation_OnResetChannels(Animation * _this,void (*_on_reset_channels)(ChannelsInfo *_channels_info, void *_user_data), void *_user_data){
	AnimationData *data = (AnimationData *)_this->data;
	data->channels_info->on_set_channels.ptr_function=_on_reset_channels;
	data->channels_info->on_set_channels.user_data=_user_data;
}

void Animation_OnSetChannels(Animation * _this,void (*_on_set_channels)(ChannelsInfo *_channels_info, void *user_data), void *_user_data){
	AnimationData *data = (AnimationData *)_this->data;
	data->channels_info->on_set_channels.ptr_function=_on_set_channels;
	data->channels_info->on_set_channels.user_data=_user_data;
}



void InfoAniAction_Delete(InfoAction *info_ani){
	ZG_FREE(info_ani);
}


//--------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTIONS

uint32_t Animation_GetDefaultStartTimeTriggerAction(void *user_data){
	UNUSUED_PARAM(user_data);
	return SDL_GetTicks();
}


Animation *Animation_New(uint8_t n_channels){
	Animation *ani=ZG_NEW(Animation);
	AnimationData *data=ZG_NEW(AnimationData);
	//memset(ani,0,sizeof(Animation));
	//AnimationData *data = ZG_NEW(AnimationData);
	//memset(data,0,sizeof(AnimationData));

	data->channels_info=ChannelsInfo_New(n_channels);
	//ani->ani_controlled_objects=List_New();
	data->info_actions=List_New();
	data->tween=Tween_New(n_channels);//(Tween *)malloc(sizeof(Tween)*n_channels);
	//memset(data->tweens,0,sizeof(Tween)*n_channels);

	ani->data=data;

	return ani;
}

/**
 * Trigger action (independent to time line)
 */
void Animation_StartAction(Animation *_this, Action *_action,uint32_t _start_time, int _repeat){
	AnimationData *data = (AnimationData *)_this->data;
	if(data->channels_info->n_channels != _action->channels_info->n_channels){
		Log_Error("Internal error animation components (%i) != action components (%i)"
				,data->channels_info->n_channels
				,data->channels_info->n_channels);
		return;
	}

	List_Add(data->info_actions,InfoAniAction_New(
			_action
			,_start_time
			,_repeat
			)
	);

}

/**
 * Remove action to time line
 */
void Animation_ClearAction(Animation *_this,Action *_action){
	AnimationData *data = (AnimationData *)_this->data;
	for(unsigned i = 0; i < data->info_actions->count; i++){
		if(((InfoAction *)data->info_actions->items[i])->action == _action){
			InfoAniAction_Delete((InfoAction *)data->info_actions->items[i]);
			List_Erase(data->info_actions,i);
			return;
		}
	}
	Log_WarningF("action not exist");
}

void Animation_StartTween(
		Animation *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	AnimationData *data = (AnimationData *)_this->data;
	Tween_Start(data->tween, _start_time,_idx_channel, _ease, _from, _to, _duration,_repeat);

}

void Animation_Clear(Animation * _this){
	AnimationData *data = (AnimationData *)_this->data;
	// TODO:
	for(unsigned i=0; i < data->info_actions->count; i++){
		InfoAniAction_Delete((InfoAction *)data->info_actions->items[i]);
	}

	List_Clear(data->info_actions);

}

void Animation_CopyChannelValues(Animation * _this, float *_array){
	AnimationData *data=_this->data;
	ChannelsInfo_CopyToFloatArray(data->channels_info,_array);
}

bool Animation_Update(Animation * _this, uint32_t _time){
	AnimationData *data=_this->data;
	bool updated=(data->info_actions->count>0) || (data->tween->channels_info->msk_active_channels != 0);
	unsigned i=0;
	/*if(data->channels_info->on_reset_channels.ptr_function!=NULL){
		data->channels_info->on_reset_channels.ptr_function(data->channels_info,data->channels_info->on_reset_channels.user_data);
	}*/

	// for all info_actions... make ops.
	// update triggered info_actions
	while(i< data->info_actions->count){

		InfoAction * info_action=(InfoAction *)data->info_actions->items[i];
		bool update_action = true;

		// render frame...
		if(Action_Update(
				info_action->action
				,_time
				,&info_action->start_time
				,&info_action->repeat
		)==false){ //remove action...
			update_action=false;
		}

		// update last channels...
		ChannelsInfo_Copy(data->channels_info,info_action->action->channels_info);
		data->channels_info->msk_active_channels |= info_action->action->channels_info->msk_active_channels;


		if(update_action){
			i++;
		}else{ // remove
			InfoAniAction_Delete(info_action);
			List_Erase(data->info_actions,i);
		}
	}

	if(data->tween->channels_info->msk_active_channels != 0){
		// save current channels
		uint32_t msk_active_channels=data->tween->channels_info->msk_active_channels;
		Tween_Update(data->tween,&msk_active_channels,_time);
		ChannelsInfo_Copy(data->channels_info,data->tween->channels_info);
		data->tween->channels_info->msk_active_channels=msk_active_channels;
	}

	return updated;
}


void Animation_Delete(Animation * _this){

	if(_this==NULL) return;
	AnimationData *_data=_this->data;

	Animation_Clear(_this);

	List_Delete(_data->info_actions);
	ChannelsInfo_Delete(_data->channels_info);
	Tween_Delete(_data->tween);


	ZG_FREE(_data);
	ZG_FREE(_this);

}


