#ifndef __ZG_GRAPHICS_H__
#define __ZG_GRAPHICS_H__

#define ZG_MSK_GRAPHIC_PROPERTY_NO_SHOW_ICON_TASKBAR	(0x1<<0x0)
#define ZG_MSK_GRAPHIC_PROPERTY_FULLSCREEN				(0x1<<0x1)


typedef enum{
	ZG_GRAPHICS_API_GL=0
}ZG_GraphicsApi;

typedef enum{
	ZG_PROJECTION_MODE_ORTHO=0
	,ZG_PROJECTION_MODE_PERSPECTIVE
}ZG_ProjectionMode;


bool 	ZG_Graphics_Init(int start_posx
		,int start_posy
		, uint16_t _width	// game resolution resolution
		, uint16_t _height // game resolution resolution
		,ZG_GraphicsApi video_context
		,const char *caption
		,uint16_t properties);

void 	ZG_Graphics_SetProjectionMode(ZG_ProjectionMode );
void 	ZG_Graphics_SetCameraTransform(ZG_Transform *transform);
void 	ZG_Graphics_SetResolutionDesign(int _width, int _height);

ZG_Vector2f ZG_Graphics_GetScale(void);

ZG_GraphicsApi ZG_Graphics_GetGraphicsApi(void);
unsigned ZG_Graphics_GetNumMonitors(void);
bool ZG_Graphics_GetScreenBounds(int num_display, SDL_Rect * rect );
bool ZG_Graphics_IsFullscreen(void);
void ZG_Graphics_ToggleFullscreen(void);
void ZG_Graphics_SetFullscreen(bool _fullscreen);

void ZG_Graphics_SetLineThickness(uint8_t _thickness);
void ZG_Graphics_SetPointSize(uint8_t _point_size);
void ZG_Graphics_SetColor4f(float _r, float _g, float _b, float _a);

uint16_t ZG_Graphics_GetWindowWidth(void);
uint16_t ZG_Graphics_GetWindowHeight(void);
uint16_t ZG_Graphics_GetWidth(void);
uint16_t ZG_Graphics_GetHeight(void);
float ZG_Graphics_GetOneOverWidth(void);
float ZG_Graphics_GetOneOverHeight(void);

uint8_t  ZG_Graphics_GetBytesPerPixel(void);

float ZG_Graphics_GetAspectRatio(void);
float ZG_Graphics_GetOneOverAspectRatio(void);

void ZG_Graphics_PrintGraphicsInfo(void);
void ZG_Graphics_SetBackgroundColor(ZG_Color4f color);

void ZG_Graphics_MakeScreenShoot(void);

void ZG_Graphics_ClearScreen(ZG_Color4f color);

void ZG_Graphics_BeginRender(void);
void ZG_Graphics_SetBackgroundColor(ZG_Color4f color);
void ZG_Graphics_EndRender(void);



/*****
 * 2D Drawing functions
 */
void ZG_Graphics_Draw(ZG_Transform *transform, ZG_Geometry *geometry, ZG_Appearance *appearance);

void ZG_Graphics_DrawPoint2f(float _x, float _y, ZG_Color4f _color, uint8_t _point_size);
void ZG_Graphics_DrawPoint2i(int _x, int _y, ZG_Color4f _color, uint8_t _point_size);

void ZG_Graphics_DrawRectangle2i(uint16_t width, uint16_t height, ZG_Color4f color, uint8_t thickness);
void ZG_Graphics_DrawRectangle4i(int _x_center, int _y_center, int _width, int _height, ZG_Color4f color,  uint8_t thickness);
void ZG_Graphics_DrawRectangle4f(float _x_center, float _y_center, float _scalex, float _scaley, ZG_Color4f color, uint8_t thickness);

void ZG_Graphics_DrawRectangleFilled4i(int x, int y, uint16_t width, uint16_t height, ZG_Color4f color);
void ZG_Graphics_DrawRectangleFilled4f(float x1, float y1, float x2, float y2, ZG_Color4f color);

void ZG_Graphics_DrawRectangleTextured4i(int _x, int _y, uint16_t width, uint16_t height,  ZG_Color4f color,ZG_Texture *text, ZG_TextureRect * text_crop);
void ZG_Graphics_DrawRectangleTextured4f(float _x1, float _y1, float _x2, float _y2, ZG_Color4f color, ZG_Texture *text, ZG_TextureRect * text_crop);

void ZG_Graphics_DrawCircle3f(float _x, float _y, float _r, ZG_Color4f _color, uint8_t _thickness);
void ZG_Graphics_DrawCircle3i(int _x, int _y, int _r, ZG_Color4f _color, uint8_t _thickness);


//void ZG_Graphics_DrawShape(Shape *shape, ZG_Transform *transform);
//void ZG_Graphics_DrawShape2d(Shape2d *shape);
void ZG_Graphics_Print(int x, int y,ZG_Color4f color, const char *in,...);
void ZG_Graphics_WPrint(int x, int y,ZG_Color4f color, const wchar_t *in,...);


void ZG_Graphics_AddCaptureScreenCallback(ZG_Callback fun);

//void  ZG_Graphics_RenderText(float x, float y, ZG_Color4f color, const char *str, ...);
float ZG_Graphics_GetFPS(void);

void ZG_Graphics_DeInit(void);
#endif
