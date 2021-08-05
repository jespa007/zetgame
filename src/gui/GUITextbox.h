#ifndef __GUI_TEXTBOX_H__
#define __GUI_TEXTBOX_H__

typedef struct GUITextbox GUITextbox;

struct GUITextbox{
	GUIWidget *widget;
	void *data;
};


GUITextbox *GUITextbox_New(int x, int y, uint16_t width, uint16_t height);
void GUITextbox_Delete(GUITextbox *_this);

#endif
