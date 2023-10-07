#ifndef __ZG_TWEEN_H__
#define __ZG_TWEEN_H__

typedef struct ZG_Tween ZG_Tween;

struct ZG_Tween{
	ZG_ChannelsInfo	*channels_info;
	ZG_TweenChannel	*tween_channels;
};

ZG_Tween *ZG_Tween_New(Uint8 n_channels);

void ZG_Tween_Start(ZG_Tween *_this
		,uint32_t _start_time
		, uint8_t _idx_channel
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat
		);

void ZG_Tween_Update(ZG_Tween *_this,uint32_t _time);

void ZG_Tween_Delete(ZG_Tween *_this);

#endif
