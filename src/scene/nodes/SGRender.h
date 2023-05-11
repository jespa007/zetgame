#ifndef __RENDER__
#define __RENDER__



typedef struct SGRender SGRender;

struct SGRender{
	void *data;
};

//-------

void SGRender_Init(void);
void SGRender_DeInit(void);

//-------

SGRender * SGRender_New(void);

/**
 * camera: Pass the camera that will render objects. If null render will get default camera.
 */

void SGRender_Start(SGRender *_this,SGCamera * camera);
void SGRender_SetBackground3i(SGRender *_this,uint8_t r,uint8_t g,uint8_t b);
void SGRender_AddNodeToDraw(SGRender *_this, TransformNode *node);
void SGRender_End(SGRender *_this);
void SGRender_Delete(SGRender *_this);

#endif


