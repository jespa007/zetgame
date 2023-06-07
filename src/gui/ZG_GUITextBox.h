#ifndef __ZG_GUI_LABEL_H__
#define __ZG_GUI_LABEL_H__

typedef struct ZG_GUITextBox ZG_GUITextBox;

struct ZG_GUITextBox{
	ZG_GUIWidget 	*	widget;
	ZG_TextBox 	*	textbox;
	void *data;
};


ZG_GUITextBox *	ZG_GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void 			ZG_GUITextBox_Delete(ZG_GUITextBox *_this);

#endif
