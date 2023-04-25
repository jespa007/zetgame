#include "zg_gui.h"

static void GUILabel_Draw(void *gui_label);

typedef struct{
	TTFont *	font;
	TextBox *	textbox;
	char		font_name[50];
}GUILabelData;

GUILabel *GUILabel_New(int x, int y, uint16_t width, uint16_t height){
	GUILabel *label = ZG_NEW(GUILabel);

	label->widget=GUIWidget_New( x,  y,  width,  height);
	label->widget->type=WIDGET_TYPE_LABEL;

	GUIWidget_SetDrawFunction(label->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUILabel_Draw
				,.calling_widget=label
			}
	);

	// SETUP DATA
	GUILabelData *data = ZG_NEW(GUILabelData);
	TTFontInfo font_info=TTFontManager_GetEmbeddedFontInfo();
	data->textbox=Textbox_New();
	strcpy(data->font_name,font_info.font_name);
	label->data=data;

	return label;
}

void 			GUILabel_SetText(GUILabel *_this, const char *_text_in,...){
	GUILabelData *data=_this->data;
	char text_out[STR_MAX];
	STR_CAPTURE_VARGS(text_out,_text_in);

	Textbox_SetText(data->textbox,text_out);

}

void 			GUILabel_SetHeight(GUILabel *_this,uint16_t _height){
	GUILabelData *data=_this->data;

	GUIWidget_SetHeight(_this->widget,_height);
	Textbox_SetHeight(data->textbox,_height);
}

void 			GUILabel_SetWidth(GUILabel *_this,uint16_t _width){
	GUILabelData *data=_this->data;

	GUIWidget_SetWidth(_this->widget,_width);
	Textbox_SetWidth(data->textbox,_width);
}

void 			GUILabel_SetFontName(GUILabel *_this, const char *_font_name){
	GUILabelData *data=_this->data;
	TTFontManager *tffont_manager=GUIWidget_GetTTFontManager(_this->widget);

	if(tffont_manager != NULL){
		TTFont *font=Textbox_GetFont(data->textbox);
		if((font=TTFontManager_GetFont(tffont_manager,_font_name,font->font_size))!=NULL){
			// if font manager was able to create the font, update font name
			Textbox_SetFont(data->textbox,font);
			//strcpy(data->font_name,_font_name);
		}
	}
}

const char *	GUILabel_GetFontName(GUILabel *_this){
	GUILabelData *data=_this->data;
	return data->font_name;

}


void 			GUILabel_SetFontSize(GUILabel *_this, uint8_t _font_size){
	GUILabelData *data=_this->data;
	TTFontManager *tffont_manager=GUIWidget_GetTTFontManager(_this->widget);

	if(tffont_manager != NULL){
		TTFont *font=NULL;

		if((font=TTFontManager_GetFont(tffont_manager,data->font_name,_font_size))!=NULL){
			// if font manager was able to create the font, update font size
			Textbox_SetFont(data->textbox,font);
		}
	}
}

uint8_t			GUILabel_GetFontSize(GUILabel *_this){
	GUILabelData *data=_this->data;

	TTFont *font=Textbox_GetFont(data->textbox);
	return font->font_size;
}

static void GUILabel_Draw(void *gui_label){
	GUILabel *_this=gui_label;
	GUILabelData *data=_this->data;
	Transform	transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);

	Transform_SetPosition2i(&transform,position.x,position.y);
	Textbox_Draw(data->textbox,&transform,&_this->widget->color);
}



void GUILabel_Delete(GUILabel *_this){
	if(_this == NULL) return;
	GUILabelData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	Textbox_Delete(data->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);

}
