#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

typedef struct GUILabel GUILabel;

struct GUILabel{
	GUIWidget *widget;
	void *data;
};


GUILabel *GUILabel_New(int x, int y, uint16_t width, uint16_t height);
void GUILabel_Delete(GUILabel *_this);

#endif
