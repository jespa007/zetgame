#ifndef __GUI_VIEWER_H__
#define __GUI_VIEWER_H__

typedef struct GUIViewer GUIViewer;

struct GUIViewer{
	GUIWidget *widget;
	void *data;
};

GUIViewer *	GUIViewer_New(int x, int y, uint16_t width, uint16_t height);
void 		GUIViewer_Delete(GUIViewer *_this);


#endif
