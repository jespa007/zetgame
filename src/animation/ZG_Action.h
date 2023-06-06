#ifndef __ZG_ACTION_H__
#define __ZG_ACTION_H__




typedef struct ZG_Action ZG_Action;
//typedef struct KeyframeTrack KeyframeTrack;

struct ZG_Action{
	ChannelsInfo		*channels_info;
	KeyframeTrack 		**channel_keyframe_tracks;
	uint32_t 	   		duration;
	uint32_t 			min_time_ms;
	uint32_t 			max_time_ms;
};



// member
ZG_Action * 		ZG_Action_New(uint8_t n_channels);
bool 			ZG_Action_Update(ZG_Action *action, uint32_t current_ms, uint32_t *start_time_ms, int *repeat);
void 			ZG_Action_SetKeyframesTrack(ZG_Action *_this, uint8_t idx_channel, Ease ease,float *keyframe_points, size_t keyframe_points_count);
//void 			ZG_Action_SetKeyframesTrackGroup(ZG_Action *_this, uint8_t idx_channel_first ,uint32_t msk_keyframe_track_group, Ease _ease,const float *points, size_t points_count);
void			ZG_Action_Unload(ZG_Action *action);
void			ZG_Action_Delete(ZG_Action *action);

#endif
