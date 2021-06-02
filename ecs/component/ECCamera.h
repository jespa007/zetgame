#ifndef __EC_CAMERA_H__
#define __EC_CAMERA_H__

typedef struct ECCamera ECCamera;
typedef struct Entity Entity;

struct ECCamera{
	Entity *entity;
	PROJECTION_MODE projection_mode;
	void *data;
};



#endif
