#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

typedef struct GUIButton GUIButton;

struct GUIButton{
	GUIWidget	*widget;
	Texture 	*texture;
	TextBox 	*textbox;

	void 		*data;
};

void GUIButton_Init(void);
void GUIButton_DeInit(void);

GUIButton * GUIButton_New(int x, int y, uint16_t width, uint16_t height);
void 		GUIButton_SetIcon(GUIButton *_this, Icon icon_info);
void 		GUIButton_SetupClickOnOver(GUIButton *_this, uint32_t time_ms, Color4f color);
void 		GUIButton_AddEventOnClick(GUIButton *_this, CallbackMouseEvent on_click);
void 		GUIButton_Delete(GUIButton *_this);


#endif
