#ifndef __TWEEN_CHANNEL_H__
#define __TWEEN_CHANNEL_H__

typedef struct TweenChannel TweenChannel;

struct TweenChannel{
	float to,from;
	uint32_t start_time;
	Ease ease;
	int repeat;
	uint32_t duration;
	float one_over_duration;
};


#endif
