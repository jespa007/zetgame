#include "zg_gui.h"



static void GUITextbox_Draw(void *gui_textbox);

GUITextbox *GUITextbox_New(int x, int y, uint16_t width, uint16_t height){
	GUITextbox *label = NEW(GUITextbox);

	label->widget=GUIWidget_New( x,  y,  width,  height);
	label->widget->textbox=Textbox_New();

	GUIWidget_SetDrawFunction(label->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUITextbox_Draw
				,.calling_widget=label
			}
	);

	// SETUP LABEL

	return label;
}


static void GUITextbox_Draw(void *gui_textbox){
	GUITextbox *_this=gui_textbox;
	Transform	transform=Transform_ResetValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);

	Transform_SetPosition2i(&transform,position.x,position.y);
	Textbox_Draw(_this->widget->textbox,&transform,&_this->widget->color);

}


void GUITextbox_Delete(GUITextbox *_this){
	if(_this == NULL) return;

	GUIWidget_Delete(_this->widget);


	FREE(_this);
}
