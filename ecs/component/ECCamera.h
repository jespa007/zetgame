#ifndef _EC_CAMERA_H__
#define _EC_CAMERA_H__

typedef struct ECCamera ECCamera;
typedef struct Entity Entity;

struct ECCamera{
	EComponent 		id;
	Entity 			*entity;
	PROJECTION_MODE projection_mode;
	void 			*data;
};



#endif
