#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUITextBox GUITextBox;

struct GUITextBox{
	GUIWidget 	*	widget;
	ZG_TextBox 	*	textbox;
	void *data;
};


GUITextBox *	GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void 			GUITextBox_Delete(GUITextBox *_this);

#endif
