#ifndef __GUI_VIEWER_H__
#define __GUI_VIEWER_H__

typedef struct GUITexture GUITexture;

struct GUITexture{
	GUIWidget 	*widget;
	TextBox 	*textbox;
	void 		*data;
};

GUITexture *	GUITexture_New(int x, int y, uint16_t width, uint16_t height);
void			GUITexture_SetTexture(GUITexture *_this, const char *_source);
Texture	   *	GUITexture_GetTexture(GUITexture *_this);
void 			GUITexture_Delete(GUITexture *_this);


#endif
