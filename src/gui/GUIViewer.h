#ifndef __GUI_VIEWER_H__
#define __GUI_VIEWER_H__

typedef struct GUIViewer GUIViewer;

struct GUIViewer{
	GUIWidget 	*widget;
	void 		*data;
};

GUIViewer *		GUIViewer_New(int x, int y, uint16_t width, uint16_t height);
void	 		GUIViewer_SetWidth(GUIViewer *_this, uint16_t _width);
void	 		GUIViewer_SetHeight(GUIViewer *_this, uint16_t _height);
void			GUIViewer_SetTexture(GUIViewer *_this, const char *_texture_id);
void 			GUIViewer_SetText(GUIViewer *_this, const char *_text_in,...);
void 			GUIViewer_SetFontFile(GUIViewer *_this, const char *_font_name);
void 			GUIViewer_SetText(GUIViewer *_this, const char *_text,...);
void			GUIViewer_SetHorizontalAlignment(GUIViewer *_this,HorizontalAlignment _text_align);
void			GUIViewer_SetVerticalAlignment(GUIViewer *_this,VerticalAlignment _vertical_align);
void 			GUIViewer_SetBorderThickness(GUIViewer *_this,int _thickness);
void 			GUIViewer_SetBorderColorHtml(GUIViewer *_this,const char *_color_html);
void 			GUIViewer_SetFontSize(GUIViewer *_this, uint8_t _font_size);
uint16_t		GUIViewer_GetFontSize(GUIViewer *_this);
void 			GUIViewer_Delete(GUIViewer *_this);


#endif
