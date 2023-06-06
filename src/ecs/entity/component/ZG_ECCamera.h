#ifndef _EC_CAMERA_H__
#define _EC_CAMERA_H__

typedef struct ECCamera ECCamera;
typedef struct Entity Entity;

struct ECCamera{
	EComponentHeader;
	ZG_ProjectionMode projection_mode;
	void 			*data;
};



#endif
