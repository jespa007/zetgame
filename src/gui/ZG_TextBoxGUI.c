#include "zg_gui.h"

static void GUITextBox_Draw(void *gui_label);

typedef struct{
	ZG_TTFont *	font;
}GUITextBoxData;

GUITextBox *GUITextBox_New(int x, int y, uint16_t width, uint16_t height){
	GUITextBox *label = ZG_NEW(GUITextBox);

	label->widget=GUIWidget_New( x,  y,  width,  height);
	label->widget->type=WIDGET_TYPE_LABEL;
	label->widget->gui_ptr=label;
	label->textbox=ZG_TextBox_New();

	GUIWidget_SetDrawFunction(label->widget
		,(CallbackWidgetUpdate){
			.ptr_function=GUITextBox_Draw
			,.calling_widget=label
		}
	);

	// SETUP DATA
	GUITextBoxData *data = ZG_NEW(GUITextBoxData);
	label->data=data;

	return label;
}


static void GUITextBox_Draw(void *gui_label){
	GUITextBox *_this=gui_label;
	ZG_Transform	transform=ZG_Transform_DefaultValues();
	ZG_Color4f color=GUIWidget_GetColor4f(_this->widget);

	ZG_Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	ZG_Transform_SetPosition2i(&transform,position.x,position.y);
	ZG_TextBox_Draw(_this->textbox,&transform,&color);
}

void GUITextBox_Delete(GUITextBox *_this){
	if(_this == NULL) return;
	GUITextBoxData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	ZG_TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
