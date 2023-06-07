#include "_zg_gui_.h"

static void ZG_GUITextBox_Draw(void *gui_label);

typedef struct{
	ZG_TTFont *	font;
}ZG_GUITextBoxData;

ZG_GUITextBox *ZG_GUITextBox_New(int x, int y, uint16_t width, uint16_t height){
	ZG_GUITextBox *label = ZG_NEW(ZG_GUITextBox);

	label->widget=ZG_GUIWidget_New( x,  y,  width,  height);
	label->widget->type=ZG_GUI_WIDGET_TYPE_LABEL;
	label->widget->gui_ptr=label;
	label->textbox=ZG_TextBox_New();

	ZG_GUIWidget_SetDrawFunction(label->widget
		,(ZG_CallbackWidgetUpdate){
			.ptr_function=ZG_GUITextBox_Draw
			,.calling_widget=label
		}
	);

	// SETUP DATA
	ZG_GUITextBoxData *data = ZG_NEW(ZG_GUITextBoxData);
	label->data=data;

	return label;
}


static void ZG_GUITextBox_Draw(void *gui_label){
	ZG_GUITextBox *_this=gui_label;
	ZG_Transform	transform=ZG_Transform_DefaultValues();
	ZG_Color4f color=ZG_GUIWidget_GetColor4f(_this->widget);

	ZG_Vector2i position=ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=ZG_GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	ZG_Transform_SetPosition2i(&transform,position.x,position.y);
	ZG_TextBox_Draw(_this->textbox,&transform,&color);
}

void ZG_GUITextBox_Delete(ZG_GUITextBox *_this){
	if(_this == NULL) return;
	ZG_GUITextBoxData *data=_this->data;

	ZG_GUIWidget_Delete(_this->widget);
	ZG_TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
