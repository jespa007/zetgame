#ifndef __VIEW_PORT_H__
#define __VIEW_PORT_H__

typedef struct ZG_ViewPort ZG_ViewPort;

struct ZG_ViewPort{
		int x,y;
		uint16_t 	width
	 	 	 	,height;

	 float one_over_width
		  ,one_over_height;

	ZG_Vector3f		projection_origin;

	ZG_ProjectionMode	projection_mode;

};

//-----
// STATIC
void 		ZG_ViewPort_Init(uint16_t _width, uint16_t _height);
ZG_ViewPort * 	ZG_ViewPort_Push(int x, int y,uint16_t width,uint16_t height, ZG_ProjectionMode projection_mode);
ZG_ViewPort * 	ZG_ViewPort_Pop(void);
ZG_ViewPort * 	ZG_ViewPort_GetCurrent(void);
void 		ZG_ViewPort_DeInit(void);

//-----
// PUBLIC

// it converts 3d to 2d. It suposses camera at origin ( tr(0,0,-1) rot(0,0,0))
int ZG_ViewPort_WorldToScreenPosX(float x3d);
int ZG_ViewPort_WorldToScreenPosY(float y3d);
ZG_Vector2i ZG_ViewPort_WorldToScreen(float x3d, float y3d);

// it converts 2d to 3d. It supposes camera at origin ( tr(0,0,-1) rot(0,0,0))
float 		ZG_ViewPort_ScreenToWorldPositionX(int x2d);
float 		ZG_ViewPort_ScreenToWorldPositionY(int y2d);
ZG_Vector3f 	ZG_ViewPort_ScreenToWorld(int x2d, int y2d);

uint16_t ZG_ViewPort_CurrentWidth(void);
uint16_t ZG_ViewPort_CurrentHeight(void);

float 		ZG_ViewPort_ScreenToWorldWidth(int width);
float 		ZG_ViewPort_ScreenToWorldHeight(int  height);
ZG_Vector3f 	ZG_ViewPort_ScreenToWorldDimension2i(int width, int height);

ZG_Vector3f ZG_ViewPort_GetProjectionOrigin(void);


#endif
