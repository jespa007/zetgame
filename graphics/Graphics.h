#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define MSK_GRAPHIC_PROPERTY_NO_SHOW_ICON_TASKBAR	(0x1<<0x0)
#define MSK_GRAPHIC_PROPERTY_DESKTOP				(0x1<<0x1)


typedef enum{
	GRAPHICS_API_GL=0
}GraphicsApi;

typedef enum{
	 PROJECTION_MODE_ORTHO=0
	,PROJECTION_MODE_PERSPECTIVE
}PROJECTION_MODE;


bool 	Graphics_Init(int start_posx
		,int start_posy
		, uint16_t _width	// game resolution resolution
		, uint16_t _height // game resolution resolution
		,GraphicsApi video_context
		,const char *caption
		,uint16_t properties);

void 	Graphics_SetProjectionMode(PROJECTION_MODE);
void 	Graphics_SetCameraTransform(Transform *transform);
void 	Graphics_SetResolutionDesign(int _width, int _height);

Vector2f Graphics_GetScale(void);

GraphicsApi Graphics_GetGraphicsApi(void);
unsigned Graphics_GetNumMonitors(void);
bool Graphics_GetScreenBounds(int num_display, SDL_Rect * rect );
void Graphics_ToggleFullscreen(void);

uint16_t Graphics_GetWidth(void);
uint16_t Graphics_GetHeight(void);
float Graphics_GetOneOverWidth(void);
float Graphics_GetOneOverHeight(void);

uint8_t  Graphics_GetBytesPerPixel(void);

float Graphics_GetAspectRatio(void);
float Graphics_GetOneOverAspectRatio(void);

void Graphics_PrintGraphicsInfo(void);
void Graphics_SetColorBackground(Color4f color);

void Graphics_MakeScreenShoot(void);

void Graphics_ClearScreen(Color4f color);

void Graphics_BeginRender(void);
void Graphics_SetBackgroundColor(Color4f color);
void Graphics_EndRender(void);



/*****
 * 2D Drawing functions
 */
void Graphics_Draw(Transform *transform, Geometry *geometry, Appearance *appearance);

void Graphics_DrawRectangle(int x, int y, uint16_t width, uint16_t height,  uint8_t thickness, Color4f color);
void Graphics_DrawRectangle4f(float x1, float y1, float x2, float y2, uint8_t thickness, Color4f color);

void Graphics_DrawRectangleFilled(int x, int y, uint16_t width, uint16_t height, Color4f color);
void Graphics_DrawRectangleFilled4f(float x1, float y1, float x2, float y2, Color4f color);

void Graphics_DrawRectangleTextured(int x, int y, uint16_t width, uint16_t height,  Color4f color,Texture *text, Rectanglef * text_crop);
void Graphics_DrawRectangleTextured4f(float x1, float y1, float x2, float y2, Color4f color, Texture *text, Rectanglef * text_crop);


//void Graphics_DrawShape(Shape *shape, Transform *transform);
//void Graphics_DrawShape2d(Shape2d *shape);
void Graphics_Print(int x, int y,Color4f color, const char *in,...);
void Graphics_WPrint(int x, int y,Color4f color, const wchar_t *in,...);


void Graphics_AddCaptureScreenCallback(Callback fun);

//void  Graphics_RenderText(float x, float y, Color4f color, const char *str, ...);
float Graphics_GetFPS(void);

void Graphics_DeInit(void);
#endif
