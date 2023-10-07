#include "@zg_animation.h"

typedef struct{
	ZG_Action *action;
	uint32_t start_time;
	int repeat;
}InfoAction;


typedef struct{
	ZG_List *info_actions;
	ZG_Tween  *tween;
	ZG_ChannelsInfo 	*channels_info;
	ZG_Callback on_after_set_components;

}ZG_AnimationData;


InfoAction * InfoAniAction_New(ZG_Action *_action,uint32_t _start_time, int  _repeat){
	InfoAction * action_info = ZG_NEW(InfoAction);
	action_info->start_time=_start_time; // only for triggered info_actions...
	action_info->repeat=_repeat;
	action_info->action = _action;

	return action_info;
}

void ZG_Animation_OnResetChannels(ZG_Animation * _this,void (*_on_reset_channels)(ZG_ChannelsInfo *_channels_info, void *_user_data), void *_user_data){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	data->channels_info->on_set_channels.ptr_function=_on_reset_channels;
	data->channels_info->on_set_channels.user_data=_user_data;
}

void ZG_Animation_OnSetChannels(ZG_Animation * _this,void (*_on_set_channels)(ZG_ChannelsInfo *_channels_info, void *user_data), void *_user_data){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	data->channels_info->on_set_channels.ptr_function=_on_set_channels;
	data->channels_info->on_set_channels.user_data=_user_data;
}



void InfoAniAction_Delete(InfoAction *info_ani){
	ZG_FREE(info_ani);
}


//--------------------------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTIONS

uint32_t ZG_Animation_GetDefaultStartTimeTriggerAction(void *user_data){
	ZG_UNUSUED_PARAM(user_data);
	return SDL_GetTicks();
}


ZG_Animation *ZG_Animation_New(uint8_t n_channels){
	ZG_Animation *ani=ZG_NEW(ZG_Animation);
	ZG_AnimationData *data=ZG_NEW(ZG_AnimationData);
	//memset(ani,0,sizeof(ZG_Animation));
	//ZG_AnimationData *data = ZG_NEW(ZG_AnimationData);
	//memset(data,0,sizeof(ZG_AnimationData));

	data->channels_info=ZG_ChannelsInfo_New(n_channels);
	//ani->ani_controlled_objects=ZG_List_New();
	data->info_actions=ZG_List_New();
	data->tween=ZG_Tween_New(n_channels);//(ZG_Tween *)malloc(sizeof(ZG_Tween)*n_channels);
	//memset(data->tweens,0,sizeof(ZG_Tween)*n_channels);

	ani->data=data;

	return ani;
}

/**
 * Trigger action (independent to time line)
 */
void ZG_Animation_StartAction(ZG_Animation *_this, ZG_Action *_action,uint32_t _start_time, int _repeat){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	if(data->channels_info->n_channels != _action->channels_info->n_channels){
		ZG_LOG_ERROR("Internal error animation components (%i) != action components (%i)"
				,data->channels_info->n_channels
				,data->channels_info->n_channels);
		return;
	}

	ZG_List_Add(data->info_actions,InfoAniAction_New(
			_action
			,_start_time
			,_repeat
			)
	);

}

/**
 * Remove action to time line
 */
void ZG_Animation_ClearAction(ZG_Animation *_this,ZG_Action *_action){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	for(unsigned i = 0; i < data->info_actions->count; i++){
		if(((InfoAction *)data->info_actions->items[i])->action == _action){
			InfoAniAction_Delete((InfoAction *)data->info_actions->items[i]);
			ZG_List_Erase(data->info_actions,i);
			return;
		}
	}
	ZG_LOG_WARNINGF("action not exist");
}

void ZG_Animation_StartTween(
		ZG_Animation *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	ZG_Tween_Start(data->tween, _start_time,_idx_channel, _ease, _from, _to, _duration,_repeat);

}

void ZG_Animation_Clear(ZG_Animation * _this){
	ZG_AnimationData *data = (ZG_AnimationData *)_this->data;
	// TODO:
	for(unsigned i=0; i < data->info_actions->count; i++){
		InfoAniAction_Delete((InfoAction *)data->info_actions->items[i]);
	}

	ZG_List_Clear(data->info_actions);

}

void ZG_Animation_CopyChannelValues(ZG_Animation * _this, float *_array){
	ZG_AnimationData *data=_this->data;
	ZG_ChannelsInfo_CopyToFloatArray(data->channels_info,_array);
}

bool ZG_Animation_Update(ZG_Animation * _this, uint32_t _time){
	ZG_AnimationData *data=_this->data;
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
		if(ZG_Action_Update(
				info_action->action
				,_time
				,&info_action->start_time
				,&info_action->repeat
		)==false){ //remove action...
			update_action=false;
		}

		// update last channels...
		ZG_ChannelsInfo_Copy(data->channels_info,info_action->action->channels_info);
		data->channels_info->msk_active_channels |= info_action->action->channels_info->msk_active_channels;


		if(update_action){
			i++;
		}else{ // remove
			InfoAniAction_Delete(info_action);
			ZG_List_Erase(data->info_actions,i);
		}
	}

	if(data->tween->channels_info->msk_active_channels != 0){
		// save current channels
		//uint32_t msk_active_channels=data->tween->channels_info->msk_active_channels;
		ZG_Tween_Update(data->tween,_time);
		ZG_ChannelsInfo_Copy(data->channels_info,data->tween->channels_info);
		//data->tween->channels_info->msk_active_channels=msk_active_channels;
	}

	return updated;
}


void ZG_Animation_Delete(ZG_Animation * _this){

	if(_this==NULL) return;
	ZG_AnimationData *_data=_this->data;

	ZG_Animation_Clear(_this);

	ZG_List_Delete(_data->info_actions);
	ZG_ChannelsInfo_Delete(_data->channels_info);
	ZG_Tween_Delete(_data->tween);


	ZG_FREE(_data);
	ZG_FREE(_this);

}


