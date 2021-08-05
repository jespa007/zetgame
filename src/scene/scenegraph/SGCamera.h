#ifndef __SGCAMERA_H__
#define __SGCAMERA_H__

typedef struct SGCamera SGCamera;

struct SGCamera{
	PROJECTION_MODE projection_mode;
	SGNode			*node;
};

SGCamera * 	SGCamera_New(void);
void 		SGCamera_Delete(SGCamera *_this);




#endif
