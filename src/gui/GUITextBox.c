#include "zg_gui.h"

static void GUITextBox_Draw(void *gui_label);

typedef struct{
	TTFont *	font;
	TextBox *	textbox;
	char		font_name[50];
}GUITextBoxData;

GUITextBox *GUITextBox_New(int x, int y, uint16_t width, uint16_t height){
	GUITextBox *label = ZG_NEW(GUITextBox);

	label->widget=GUIWidget_New( x,  y,  width,  height);
	label->widget->type=WIDGET_TYPE_LABEL;
	label->widget->gui_ptr=label;

	GUIWidget_SetDrawFunction(label->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUITextBox_Draw
				,.calling_widget=label
			}
	);

	// SETUP DATA
	GUITextBoxData *data = ZG_NEW(GUITextBoxData);
	//TTFontInfo font_info=TTFontManager_GetEmbeddedFontInfo();
	data->textbox=TextBox_New();
	label->data=data;

	return label;
}

void 			GUITextBox_SetText(GUITextBox *_this, const char *_text_in,...){
	GUITextBoxData *data=_this->data;
	char text_out[STR_MAX];
	STR_CAPTURE_VARGS(text_out,_text_in);

	TextBox_SetText(data->textbox,text_out);
}

void 			GUITextBox_SetHeight(GUITextBox *_this,uint16_t _height){
	GUITextBoxData *data=_this->data;

	GUIWidget_SetHeight(_this->widget,_height);
	TextBox_SetHeight(data->textbox,_height);
}

void 			GUITextBox_SetWidth(GUITextBox *_this,uint16_t _width){
	GUITextBoxData *data=_this->data;

	GUIWidget_SetWidth(_this->widget,_width);
	TextBox_SetWidth(data->textbox,_width);
}

void 			GUITextBox_SetFontFile(GUITextBox *_this, const char *_font_file){
	GUITextBoxData *data=_this->data;
	TextBox_SetFontFile(data->textbox,_font_file);
}

const char *	GUITextBox_GetFontName(GUITextBox *_this){
	GUITextBoxData *data=_this->data;
	return data->font_name;

}


void 			GUITextBox_SetFontSize(GUITextBox *_this, uint8_t _font_size){
	GUITextBoxData *data=_this->data;
	TextBox_SetFontSize(data->textbox,_font_size);
}

uint16_t			GUITextBox_GetFontSize(GUITextBox *_this){
	GUITextBoxData *data=_this->data;

	return TextBox_GetFontSize(data->textbox);
}

void			GUITextBox_SetTextAlign(GUITextBox *_this,TextAlign _text_align){
	GUITextBoxData *data=_this->data;
	TextBox_SetTextAlign(data->textbox,_text_align);
}

void			GUITextBox_SetVerticalAlign(GUITextBox *_this,VerticalAlign _vertical_align){
	GUITextBoxData *data=_this->data;
	TextBox_SetVerticalAlign(data->textbox,_vertical_align);
}

void GUITextBox_SetBorderThickness(GUITextBox *_this,int _thickness){
	GUITextBoxData *data=_this->data;
	TextBox_SetBorderThickness(data->textbox,_thickness);
}

void GUITextBox_SetBorderColorHtml(GUITextBox *_this,const char *_color_html){
	GUITextBoxData *data=_this->data;
	TextBox_SetBorderColor4f(data->textbox,Color4f_FromHtml(_color_html));
}

static void GUITextBox_Draw(void *gui_label){
	GUITextBox *_this=gui_label;
	GUITextBoxData *data=_this->data;
	Transform	transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	Transform_SetPosition2i(&transform,position.x,position.y);
	TextBox_Draw(data->textbox,&transform,&_this->widget->color);
}



void GUITextBox_Delete(GUITextBox *_this){
	if(_this == NULL) return;
	GUITextBoxData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	TextBox_Delete(data->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);

}
