#include "zg_animation.h"

ZG_Action * 			ZG_Action_New(uint8_t n_channels){
	ZG_Action *action=ZG_NEW(ZG_Action);
	memset(action,0,sizeof(ZG_Action));

	action->channels_info=ChannelsInfo_New(n_channels);

	action->channel_keyframe_tracks=malloc(sizeof(KeyframeTrack *)*n_channels);

	for(unsigned i=0; i < action->channels_info->n_channels; i++){
		action->channel_keyframe_tracks[i]=KeyframeTrack_New();
	}

	return action;
}


void 	ZG_Action_SetKeyframesTrack(ZG_Action *_this
		, uint8_t idx_channel
		, Ease ease
		,float *keyframe_points
		, size_t keyframe_points_count){

	if(keyframe_points == NULL) return;

	if(idx_channel >= _this->channels_info->n_channels){
		ZG_Log_ErrorF("Internal error index reached max channels");
		return;
	}

	KeyframeTrack_SetEase(_this->channel_keyframe_tracks[idx_channel],ease);
	KeyframeTrack_SetKeyframes(_this->channel_keyframe_tracks[idx_channel],keyframe_points,keyframe_points_count);

	_this->min_time_ms=MIN(_this->min_time_ms,_this->channel_keyframe_tracks[idx_channel]->minx_interval);
	_this->max_time_ms=MAX(_this->max_time_ms,_this->channel_keyframe_tracks[idx_channel]->maxx_interval);

	_this->duration = _this->max_time_ms - _this->min_time_ms;

	_this->channels_info->msk_active_channels|=(0x1<<idx_channel);
}

/*
void 				ZG_Action_SetKeyframesTrackGroup(ZG_Action *_this, uint8_t idx_channel_first ,uint32_t msk_keyframe_track_group, Ease _ease,const float *keyframe_points, size_t keyframe_points_count){
	KeyframeTrackGroup *x=NULL, *y=NULL, *z=NULL, *w=NULL;

	if(_this == NULL) return;

	switch(msk_keyframe_track_group){
	case KEYFRAME_TRACK_GROUP_COMPONENT_X:
		x=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_Y:
		y=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_Z:
		z=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_W:
		w=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_X | KEYFRAME_TRACK_GROUP_COMPONENT_Y:
		x=KeyframeTrackGroup_New();
		y=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_X | KEYFRAME_TRACK_GROUP_COMPONENT_Y | KEYFRAME_TRACK_GROUP_COMPONENT_Z:
		x=KeyframeTrackGroup_New();
		y=KeyframeTrackGroup_New();
		z=KeyframeTrackGroup_New();
		break;
	case KEYFRAME_TRACK_GROUP_COMPONENT_X | KEYFRAME_TRACK_GROUP_COMPONENT_Y | KEYFRAME_TRACK_GROUP_COMPONENT_Z | KEYFRAME_TRACK_GROUP_COMPONENT_W:
		x=KeyframeTrackGroup_New();
		y=KeyframeTrackGroup_New();
		z=KeyframeTrackGroup_New();
		w=KeyframeTrackGroup_New();
		break;
	default:
		ZG_Log_ErrorF("ZG_Action_SetKeyframes: invalid mask");
		break;
	}

	if(KeyframeTrackGroup_DecomposeXYZW(keyframe_points,keyframe_points_count,x,y,z,w)){

		if(x != NULL){
			ZG_Action_SetKeyframesTrack(_this,idx_channel_first+0,_ease,x->keyframe_points,x->keyframe_points_count);
		}

		if(y!=NULL){
			ZG_Action_SetKeyframesTrack(_this,idx_channel_first+1,_ease,y->keyframe_points,y->keyframe_points_count);
		}

		if(z!=NULL){
			ZG_Action_SetKeyframesTrack(_this,idx_channel_first+2,_ease,z->keyframe_points,z->keyframe_points_count);
		}

		if(w!=NULL){
			ZG_Action_SetKeyframesTrack(_this,idx_channel_first+3,_ease,w->keyframe_points,w->keyframe_points_count);
		}
	}

	KeyframeTrackGroup_FreeXYZW(&x,&y,&z,&w);
}*/

bool ZG_Action_Update(ZG_Action *_this, uint32_t current_time_ms, uint32_t *start_time_ms, int * repeat){

	float point=0;
	bool update=true;

	if(_this->channels_info == NULL || _this->duration == 0){
		return false;
	}

	// reset components to unit...
	if(_this->channels_info->on_reset_channels.ptr_function!=NULL){
		_this->channels_info->on_reset_channels.ptr_function(_this->channels_info,_this->channels_info->on_reset_channels.user_data);
	}

	uint32_t time_ms=current_time_ms-*start_time_ms;//[MAX_ZG_MATERIAL_COMPONENTS];

	if(
		(time_ms < _this->duration ) || ((time_ms >= _this->duration)
		&& (((*repeat) > 0) || ((*repeat) < 0)))
	){

		if(time_ms >= _this->duration ) {// clamp start time
			*start_time_ms=current_time_ms;// set start time

			if(*repeat > 0){
				(*repeat)--;
			}

			// clamp
			time_ms %= _this->duration;
		}
	}else{ //update last
		update=false;
	}

	// update all channels...
	for(int i = 0; i < _this->channels_info->n_channels; i++){
		if(KeyframeTrack_Interpolate(_this->channel_keyframe_tracks[i],time_ms,&point)){
				_this->channels_info->channels[i]  = point;
		}
	}

	return update;
}

void ZG_Action_Unload(ZG_Action *_this){
	if(_this->channel_keyframe_tracks !=NULL){
		for(unsigned i = 0; i < _this->channels_info->n_channels; i++){
			if(_this->channel_keyframe_tracks[i] != NULL){
				KeyframeTrack_Delete(_this->channel_keyframe_tracks[i]);
			}
		}

		ZG_FREE(_this->channel_keyframe_tracks);
		_this->channel_keyframe_tracks=NULL;

	}
}

void ZG_Action_Delete(ZG_Action * _this){
	if(_this != NULL){
		for(unsigned i=0; i < _this->channels_info->n_channels; i++){
			KeyframeTrack_Delete(_this->channel_keyframe_tracks[i]);
		}

		ZG_FREE(_this->channel_keyframe_tracks);

		ChannelsInfo_Delete(_this->channels_info);
		ZG_FREE(_this);

	}


}


