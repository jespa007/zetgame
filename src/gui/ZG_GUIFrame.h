#ifndef __ZG_GUI_FRAME_H__
#define __ZG_GUI_FRAME_H__

typedef struct ZG_GUIFrame ZG_GUIFrame;

struct ZG_GUIFrame{
	ZG_GUIWidget	*widget;
};

ZG_GUIFrame *	ZG_GUIFrame_New(int x, int y, uint16_t width, uint16_t height);
void		ZG_GUIFrame_Delete(ZG_GUIFrame *_this);

#endif
