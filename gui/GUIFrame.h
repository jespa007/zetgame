#ifndef __GUI_FRAME_H__
#define __GUI_FRAME_H__

typedef struct GUIFrame GUIFrame;

struct GUIFrame{
	GUIWidget	*widget;
};

GUIFrame *	GUIFrame_New(int x, int y, uint16_t width, uint16_t height);
void		GUIFrame_Delete(GUIFrame *_this);

#endif
