#include "zg_animation.h"

Tween *Tween_New(Uint8 n_channels){
	Tween *tween=ZG_NEW(Tween);
	tween->channels_info=ChannelsInfo_New(n_channels);
	tween->tween_channels=(TweenChannel *)malloc(sizeof(TweenChannel)*n_channels);
	memset(tween->tween_channels,0,sizeof(TweenChannel)*n_channels);

	return tween;
}

void Tween_Start(Tween *_this
		, uint32_t _start_time
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat){
	TweenChannel *tc=NULL;

	if(_idx_channel > _this->channels_info->n_channels){
		Log_Error("reached max channels (%i > %i)",_idx_channel,_this->channels_info->n_channels);
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

void Tween_Update(Tween *_this, uint32_t _time){
	if(_this->channels_info->msk_active_channels){
		for(unsigned i = 0; i < _this->channels_info->n_channels; i++){
			if(_this->channels_info->msk_active_channels & (0x1<<i)){ // update
				TweenChannel *tween_channel=_this->tween_channels+i;
				uint32_t diff_time=_time-tween_channel->start_time;
				if(diff_time >= tween_channel->duration &&  tween_channel->repeat < 0){
					diff_time=(_time-tween_channel->start_time)%tween_channel->duration;
				}
				float t = diff_time*tween_channel->one_over_duration;
				float value=tween_channel->from;

				if((diff_time) < tween_channel->duration){ // interpolate
					value=tween_channel->from+ease_linear(t)*(tween_channel->to - tween_channel->from);
				}else{ // set last and delete active msk
					value=tween_channel->to;
					_this->channels_info->msk_active_channels&=~(0x1<<i);
				}

				_this->channels_info->channels[i]=value;

			}
		}
	}
}

void Tween_Delete(Tween *_this){
	ChannelsInfo_Delete(_this->channels_info);

	free(_this->tween_channels);
	free(_this);
}
