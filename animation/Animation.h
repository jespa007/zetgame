#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#define ANIMATION_LOOP_REPEAT_INFINITE	-1

typedef struct Animation Animation;
/*
typedef struct {

	uint32_t 	end_time_ms;
	uint32_t 	start_time_ms; // only for triggered actions...
	bool		loop;
	ChannelOperator channel_operator;

	Action *actions;
	Tween  *tweens;
}InfoAction;*/

struct Animation{

	//ChannelsInfo 	*	channels;

	// ani controlled components...
	//List 			*	ani_controlled_components;

	// two types of actions (actions and triggered)

	//List			* 	info_ani_triggered_actions;
	//InfoAction *info_ani_action;
	// Tween

	void 			* data;



	//uint32_t duration_time_ms;
};


typedef struct{
	uint32_t (* ptr_function)(void * user_data);
	void * user_data;
} CallbackAnimationTime;

Animation * Animation_New(uint8_t n_channels);

void Animation_SetCallbackAnimationTime(Animation * _this,CallbackAnimationTime cf);
void Animation_AddChannel(Animation * _this, ChannelsInfo *ac);

void Animation_OnResetChannels(Animation * _this,void (*)(ChannelsInfo *_channels_info,void * _user_data), void *_user_data);
void Animation_OnSetChannels(Animation * _this,void (*)(ChannelsInfo *_channels_info, void *_user_data), void *_user_data);

/**
 * Erase component
 */
void Animation_EraseChannel(Animation *_this, ChannelsInfo *ac);

/**
 * Add action to time line
 */
void Animation_StartAction(Animation *_this, Action *_action,uint32_t _start_time, int _repeat);

/**
 * Add action to time line
 */
void Animation_ClearAction(Animation *_this, Action *_action);

/**
 * Trigger action (independent to time line)
 */
void Animation_StartTween(Animation *_this
		, uint32_t _start_time
		, uint8_t  _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat);


void Animation_Clear(Animation *_this);
void Animation_CopyChannelValues(Animation * _this, float *_array);
bool Animation_Update(Animation *_this,uint32_t _time);


void Animation_Delete(Animation *_this);


#endif
