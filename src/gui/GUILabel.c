#include "zg_gui.h"



static void GUILabel_Draw(void *gui_label);

GUILabel *GUILabel_New(int x, int y, uint16_t width, uint16_t height){
	GUILabel *label = ZG_NEW(GUILabel);

	label->widget=GUIWidget_New( x,  y,  width,  height);
	label->textbox=Textbox_New();

	GUIWidget_SetDrawFunction(label->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUILabel_Draw
				,.calling_widget=label
			}
	);

	// SETUP LABEL

	return label;
}


static void GUILabel_Draw(void *gui_label){
	GUILabel *_this=gui_label;
	Transform	transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);

	Transform_SetPosition2i(&transform,position.x,position.y);
	Textbox_Draw(_this->textbox,&transform,&_this->widget->color);



}


void GUILabel_Delete(GUILabel *_this){
	if(_this == NULL) return;

	GUIWidget_Delete(_this->widget);
	Textbox_Delete(_this->textbox);

	FREE(_this);
}
