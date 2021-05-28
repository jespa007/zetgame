#ifndef __TWEEN_H__
#define __TWEEN_H__

typedef struct Tween Tween;
typedef struct TweenChannel TweenChannel;

struct Tween{
	ChannelsInfo	*channels_info;
	TweenChannel	*tween_channels;
};

Tween *Tween_New(Uint8 n_channels);

void Tween_Start(Tween *_this, uint8_t _idx_channel,uint32_t _start_time, uint32_t _duration, Ease _ease, float _from, float _to, int _repeat);

void Tween_Update(Tween *_this, uint32_t _time);

void Tween_Delete(Tween *_this);

#endif
