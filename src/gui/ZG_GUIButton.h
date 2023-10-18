#ifndef __ZG_BUTTON_GUI_H__
#define __ZG_BUTTON_GUI_H__

typedef struct ZG_GUIButton ZG_GUIButton;

struct ZG_GUIButton{
	ZG_GUIWidget	*widget;
	void 		*data;
};

void ZG_GUIButton_Init(void);
void ZG_GUIButton_DeInit(void);

ZG_GUIButton * 	ZG_GUIButton_New(int _x, int _y, uint16_t _width, uint16_t _height);
void 			ZG_GUIButton_SetIcon(ZG_GUIButton *_this, Icon icon_info);
void 			ZG_GUIButton_SetText(ZG_GUIButton *_this, const char *_text_in,...);
void 			ZG_GUIButton_SetupClickOnOver(ZG_GUIButton *_this, uint32_t _time_ms, ZG_Color4f _color);
void 			ZG_GUIButton_AddCallbackOnClick(ZG_GUIButton *_this, ZG_MouseEventCallback _on_click);
void 			ZG_GUIButton_Delete(ZG_GUIButton *_this);


#endif
