#ifndef __ZG_ANIMATION_H__
#define __ZG_ANIMATION_H__

#define ANIMATION_LOOP_REPEAT_INFINITE	-1

typedef struct ZG_Animation ZG_Animation;
/*
typedef struct {

	uint32_t 	end_time_ms;
	uint32_t 	start_time_ms; // only for triggered actions...
	bool		loop;
	ChannelOperator channel_operator;

	Action *actions;
	Tween  *tweens;
}InfoAction;*/

struct ZG_Animation{

	//ChannelsInfo 	*	channels;

	// ani controlled components...
	//ZG_List 			*	ani_controlled_components;

	// two types of actions (actions and triggered)

	//ZG_List			* 	info_ani_triggered_actions;
	//InfoAction *info_ani_action;
	// Tween

	void 			* data;



	//uint32_t duration_time_ms;
};


typedef struct{
	uint32_t (* ptr_function)(void * user_data);
	void * user_data;
} CallbackZG_AnimationTime;

ZG_Animation * ZG_Animation_New(uint8_t n_channels);

void ZG_Animation_SetCallbackZG_AnimationTime(ZG_Animation * _this,CallbackZG_AnimationTime cf);
void ZG_Animation_AddChannel(ZG_Animation * _this, ChannelsInfo *ac);

void ZG_Animation_OnResetChannels(ZG_Animation * _this,void (*)(ChannelsInfo *_channels_info,void * _user_data), void *_user_data);
void ZG_Animation_OnSetChannels(ZG_Animation * _this,void (*)(ChannelsInfo *_channels_info, void *_user_data), void *_user_data);

/**
 * Erase component
 */
void ZG_Animation_EraseChannel(ZG_Animation *_this, ChannelsInfo *ac);

/**
 * Add action to time line
 */
void ZG_Animation_StartAction(ZG_Animation *_this, Action *_action,uint32_t _start_time, int _repeat);

/**
 * Add action to time line
 */
void ZG_Animation_ClearAction(ZG_Animation *_this, Action *_action);

/**
 * Trigger action (independent to time line)
 */
void ZG_Animation_StartTween(ZG_Animation *_this
		, uint32_t _start_time
		, uint8_t  _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat);


void ZG_Animation_Clear(ZG_Animation *_this);
void ZG_Animation_CopyChannelValues(ZG_Animation * _this, float *_array);
bool ZG_Animation_Update(ZG_Animation *_this,uint32_t _time);


void ZG_Animation_Delete(ZG_Animation *_this);


#endif
