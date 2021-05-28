#ifndef __ACTION_H__
#define __ACTION_H__




typedef struct Action Action;
typedef struct KeyframeTrack KeyframeTrack;

struct Action{
	ChannelsInfo		*channels_info;
	KeyframeTrack 		**channel_keyframe_tracks;
	uint32_t 	   		total_time_ms;
	uint32_t 			min_time_ms;
	uint32_t 			max_time_ms;
};



// member
Action * 		Action_New(uint8_t n_channels);
void 			Action_Update(Action *action, uint32_t current_ms, uint32_t start_time_ms, bool loop);
void 			Action_SetKeyFramesTrack(Action *_this, uint8_t idx_channel, Ease ease,float *keyframe_points, size_t keyframe_points_count);
void 			Action_SetKeyframesTrackGroup(Action *_this, uint8_t idx_channel_first ,uint32_t msk_keyframe_track_group, Ease _ease,const float *points, size_t points_count);
void			Action_Unload(Action *action);
void			Action_Delete(Action *action);

#endif
