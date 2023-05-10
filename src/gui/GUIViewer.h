#ifndef __GUI_VIEWER_H__
#define __GUI_VIEWER_H__

typedef struct GUIViewer GUIViewer;

struct GUIViewer{
	GUIWidget 	*widget;
	TextBox 	*textbox;
	void 		*data;
};

GUIViewer *		GUIViewer_New(int x, int y, uint16_t width, uint16_t height);
void	 		GUIViewer_SetWidth(GUIViewer *_this, uint16_t _width);
void	 		GUIViewer_SetHeight(GUIViewer *_this, uint16_t _height);
void			GUIViewer_SetTexture(GUIViewer *_this, const char *_texture_id);
void 			GUIViewer_Delete(GUIViewer *_this);


#endif
