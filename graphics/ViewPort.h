#ifndef __VIEW_PORT_H__
#define __VIEW_PORT_H__

typedef struct ViewPort ViewPort;

struct ViewPort{
		int x,y;
		uint16_t 	width
	 	 	 	,height;

	 float one_over_width
		  ,one_over_height;

	Vector3f		projection_origin;

	PROJECTION_MODE	projection_mode;

};

//-----
// STATIC
void 		ViewPort_Init(uint16_t _width, uint16_t _height);
ViewPort * 	ViewPort_Push(int x, int y,uint16_t width,uint16_t height, PROJECTION_MODE projection_mode);
ViewPort * 	ViewPort_Pop(void);
ViewPort * 	ViewPort_GetCurrent(void);
void 		ViewPort_DeInit(void);

//-----
// PUBLIC

// it converts 3d to 2d. It suposses camera at origin ( tr(0,0,-1) rot(0,0,0))
int ViewPort_WorldToScreenPosX(float x3d);
int ViewPort_WorldToScreenPosY(float y3d);
Vector2i ViewPort_WorldToScreen(float x3d, float y3d);

// it converts 2d to 3d. It supposes camera at origin ( tr(0,0,-1) rot(0,0,0))
float 		ViewPort_ScreenToWorldPosX(int x2d);
float 		ViewPort_ScreenToWorldPosY(int y2d);
Vector3f 	ViewPort_ScreenToWorld(int x2d, int y2d);

uint16_t ViewPort_CurrentWidth(void);
uint16_t ViewPort_CurrentHeight(void);

float 		ViewPort_ScreenToWorldWidth(int width);
float 		ViewPort_ScreenToWorldHeight(int  height);
Vector3f 	ViewPort_ScreenToWorldDim2i(int width, int height);

Vector3f ViewPort_GetProjectionOrigin(void);


#endif
