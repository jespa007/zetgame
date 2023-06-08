#ifndef _ZG_ECCAMERA_H__
#define _ZG_ECCAMERA_H__

typedef struct ZG_ECCamera ZG_ECCamera;
typedef struct ZG_Entity ZG_Entity;

struct ZG_ECCamera{
	ZG_EComponentHeader;
	ZG_ProjectionMode projection_mode;
	void 			*data;
};



#endif
