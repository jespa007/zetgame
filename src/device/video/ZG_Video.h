#ifndef __ZG_VIDEO_H__
#define __ZG_VIDEO_H__

typedef struct ZG_Video ZG_Video;

struct ZG_Video{
	void *data;
};

bool 			ZG_Video_Init(void);
ZG_Video 	*	ZG_Video_Get(void);
void 			ZG_Video_DeInit(void);

#endif
