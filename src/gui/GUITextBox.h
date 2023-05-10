#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUITextBox GUITextBox;

struct GUITextBox{
	GUIWidget 	*	widget;
	TextBox 	*	textbox;
	void *data;
};


GUITextBox *	GUITextBox_New(int x, int y, uint16_t width, uint16_t height);
void 			GUITextBox_SetWidth(GUITextBox *_this,uint16_t _width);
void 			GUITextBox_SetHeight(GUITextBox *_this,uint16_t _height);
void 			GUITextBox_Delete(GUITextBox *_this);

#endif
