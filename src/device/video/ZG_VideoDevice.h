#ifndef __ZG_VIDEO_DEVICE_H__
#define __ZG_VIDEO_DEVICE_H__

typedef struct ZG_VideoDevice ZG_VideoDevice;

struct ZG_VideoDevice{
	void *data;
};

bool ZG_VideoDevice_Init(void);
ZG_VideoDevice *ZG_VideoDevice_GetDevice(void);
void ZG_VideoDevice_DeInit(void);

#endif
