#include "_zg_animation_.h"

ZG_Tween *ZG_Tween_New(Uint8 n_channels){
	ZG_Tween *tween=ZG_NEW(ZG_Tween);
	tween->channels_info=ZG_ChannelsInfo_New(n_channels);
	tween->tween_channels=(ZG_TweenChannel *)malloc(sizeof(ZG_TweenChannel)*n_channels);
	memset(tween->tween_channels,0,sizeof(ZG_TweenChannel)*n_channels);

	return tween;
}

void ZG_Tween_Start(ZG_Tween *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	ZG_TweenChannel *tc=NULL;

	if(_idx_channel > _this->channels_info->n_channels){
		ZG_LOG_ERROR("reached max channels (%i > %i)",_idx_channel,_this->channels_info->n_channels);
		return;
	}

	tc=_this->tween_channels+_idx_channel;
	tc->duration=_duration;
	tc->ease=_ease;
	tc->from=_from;
	tc->to=_to;
	tc->start_time=_start_time;
	tc->repeat = _repeat;
	tc->one_over_duration=1.0f/_duration;

	// mark channel active
	_this->channels_info->msk_active_channels|=0x1<<_idx_channel;
}

void ZG_Tween_Update(ZG_Tween *_this, uint32_t *_msk_active_channels, uint32_t _time){
	if(_this->channels_info->msk_active_channels){
		for(unsigned i = 0; i < _this->channels_info->n_channels; i++){
			if(_this->channels_info->msk_active_channels & (0x1<<i)){ // update
				ZG_TweenChannel *tween_channel=_this->tween_channels+i;
				uint32_t diff_time=_time-tween_channel->start_time;
				if(diff_time >= tween_channel->duration &&  tween_channel->repeat < 0){
					diff_time=(_time-tween_channel->start_time)%tween_channel->duration;
				}
				float t = diff_time*tween_channel->one_over_duration;
				float value=tween_channel->from;

				if((diff_time) < tween_channel->duration){ // interpolate
					value=tween_channel->from+ZG_Ease_Linear(t)*(tween_channel->to - tween_channel->from);
				}else{ // set last and delete active msk
					value=tween_channel->to;
					*_msk_active_channels&=~(0x1<<i);
				}
				_this->channels_info->channels[i]=value;
			}
		}
	}
}

void ZG_Tween_Delete(ZG_Tween *_this){
	ZG_ChannelsInfo_Delete(_this->channels_info);

	free(_this->tween_channels);
	free(_this);
}
