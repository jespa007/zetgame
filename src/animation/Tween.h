#ifndef __TWEEN_H__
#define __TWEEN_H__

typedef struct Tween Tween;

struct Tween{
	ChannelsInfo	*channels_info;
	TweenChannel	*tween_channels;
};

Tween *Tween_New(Uint8 n_channels);

void Tween_Start(Tween *_this
		,uint32_t _start_time
		, uint8_t _idx_channel
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
		);

void Tween_Update(Tween *_this,uint32_t *_msk_active_channels,uint32_t _time);

void Tween_Delete(Tween *_this);

#endif
