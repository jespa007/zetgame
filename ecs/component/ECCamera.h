#ifndef __EC_CAMERA_H__
#define __EC_CAMERA_H__

typedef struct ECCamera ECCamera;

struct ECCamera{
	Entity *entity;
	PROJECTION_MODE projection_mode;
	void *data;
};



#endif
