#ifndef __ZG_TWEEN_CHANNEL_H__
#define __ZG_TWEEN_CHANNEL_H__

typedef struct ZG_TweenChannel ZG_TweenChannel;

struct ZG_TweenChannel{
	float to,from;
	uint32_t start_time;
	Ease ease;
	int repeat;
	uint32_t duration;
	float one_over_duration;
};


#endif
