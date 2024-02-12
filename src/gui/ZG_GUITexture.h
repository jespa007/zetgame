#ifndef __ZG_GUI_VIEWER_H__
#define __ZG_GUI_VIEWER_H__

typedef struct ZG_GUITexture ZG_GUITexture;

struct ZG_GUITexture{
	ZG_GUIWidget 	*widget;
	ZG_TextBox 	*textbox;
	void 		*data;
};

ZG_GUITexture *	ZG_GUITexture_New(int x, int y, uint16_t width, uint16_t height);
void			ZG_GUITexture_SetTexture(ZG_GUITexture *_this, ZG_Texture *_texture);
ZG_Texture	   *	ZG_GUITexture_GetTexture(ZG_GUITexture *_this);
void 			ZG_GUITexture_Delete(ZG_GUITexture *_this);


#endif
