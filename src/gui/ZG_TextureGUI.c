#include "zg_gui.h"


static void GUITexture_DrawWidget(void *gui_texture);

typedef struct{
	Texture 	*texture;
}GUITextureData;


static void GUITexture_DrawWidget(void *gui_texture){
	GUITexture *_this=gui_texture;
	GUITextureData *data=_this->data;
	Transform transform=Transform_DefaultValues();
	Color4f color=GUIWidget_GetColor4f(_this->widget);

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	if(data->texture!=NULL){
		Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,COLOR4F_WHITE,data->texture,NULL);
	}

	Transform_SetPosition2i(&transform,position.x,position.y);
	TextBox_Draw(_this->textbox,&transform,&color);
}

GUITexture *GUITexture_New(int x, int y, uint16_t width, uint16_t height){
	GUITexture *viewer = ZG_NEW(GUITexture);

	// SETUP WIDGET
	viewer->widget=GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;
	viewer->textbox=TextBox_New();


	// SETUP DATA
	GUITextureData *data = ZG_NEW(GUITextureData);
	data->texture=NULL;
	viewer->data=data;

	TextBox_SetHorizontalAlignment(viewer->textbox,HORIZONTAL_ALIGNMENT_CENTER);
	TextBox_SetVerticalAlignment(viewer->textbox,VERTICAL_ALIGNMENT_CENTER);

	GUIWidget_SetDrawFunction(viewer->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUITexture_DrawWidget
				,.calling_widget=viewer
			}
	);

	return viewer;
}

void			GUITexture_SetTexture(GUITexture *_this, const char *_source){
	GUITextureData *data=_this->data;
	TextureManager *texture_manager=GUIWidget_GetTextureManager(_this->widget);

	if(texture_manager!=NULL){
		data->texture=TextureManager_Get(texture_manager,_source);
	}
}


Texture			*GUITexture_GetTexture(GUITexture *_this){
	GUITextureData *data=_this->data;
	return data->texture;
}


void GUITexture_Delete(GUITexture *_this){
	if(_this == NULL) return;
	GUITextureData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
