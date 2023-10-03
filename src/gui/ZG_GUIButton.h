#ifndef __ZG_BUTTON_GUI_H__
#define __ZG_BUTTON_GUI_H__

typedef struct ZG_GUIButton ZG_GUIButton;

struct ZG_GUIButton{
	ZG_GUIWidget	*widget;
	void 		*data;
};

void ZG_GUIButton_Init(void);
void ZG_GUIButton_DeInit(void);

ZG_GUIButton * 	ZG_GUIButton_New(int x, int y, uint16_t width, uint16_t height);
void 			ZG_GUIButton_SetIcon(ZG_GUIButton *_this, Icon icon_info);
void 			ZG_GUIButton_SetText(ZG_GUIButton *_this, const char *_text_in,...);
void 			ZG_GUIButton_SetupClickOnOver(ZG_GUIButton *_this, uint32_t time_ms, ZG_Color4f color);
void 			ZG_GUIButton_AddCallbackOnClick(ZG_GUIButton *_this, ZG_MouseEventCallback on_click);
void 			ZG_GUIButton_Delete(ZG_GUIButton *_this);


#endif
