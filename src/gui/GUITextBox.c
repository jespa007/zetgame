#include "zg_gui.h"



static void GUITextBox_Draw(void *gui_textbox);

GUITextBox *GUITextBox_New(int x, int y, uint16_t width, uint16_t height){
	GUITextBox *textbox = ZG_NEW(GUITextBox);

	textbox->widget=GUIWidget_New( x,  y,  width,  height);
	textbox->widget->type=WIDGET_TYPE_TEXTBOX;
	textbox->widget->gui_ptr=textbox;

	textbox->textbox=TextBox_New();

	GUIWidget_SetDrawFunction(textbox->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUITextBox_Draw
				,.calling_widget=textbox
			}
	);

	// SETUP LABEL

	return textbox;
}


static void GUITextBox_Draw(void *gui_textbox){
	GUITextBox *_this=gui_textbox;
	Transform	transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);

	Transform_SetPosition2i(&transform,position.x,position.y);
	TextBox_Draw(_this->textbox,&transform,&_this->widget->color);

}


void GUITextBox_Delete(GUITextBox *_this){
	if(_this == NULL) return;

	GUIWidget_Delete(_this->widget);
	TextBox_Delete(_this->textbox);

	ZG_FREE(_this);
}
