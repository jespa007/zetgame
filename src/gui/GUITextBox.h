#ifndef __GUI_TEXTBOX_H__
#define __GUI_TEXTBOX_H__

typedef struct GUITextBox GUITextBox;

struct GUITextBox{
	GUIWidget 	*widget;
	TextBox 	*textbox;
	void 		*data;
};

GUITextBox *GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void GUITextBox_Delete(GUITextBox *_this);

#endif
