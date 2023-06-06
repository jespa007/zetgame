#include "zg_gui.h"


static void GUITexture_DrawWidget(void *gui_texture);

typedef struct{
	ZG_Texture 	*texture;
}GUITextureData;


static void GUITexture_DrawWidget(void *gui_texture){
	GUITexture *_this=gui_texture;
	GUITextureData *data=_this->data;
	ZG_Transform transform=ZG_Transform_DefaultValues();
	ZG_Color4f color=GUIWidget_GetColor4f(_this->widget);

	ZG_Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	if(data->texture!=NULL){
		Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,ZG_COLOR4F_WHITE,data->texture,NULL);
	}

	ZG_Transform_SetPosition2i(&transform,position.x,position.y);
	ZG_TextBox_Draw(_this->textbox,&transform,&color);
}

GUITexture *GUITexture_New(int x, int y, uint16_t width, uint16_t height){
	GUITexture *viewer = ZG_NEW(GUITexture);

	// SETUP WIDGET
	viewer->widget=GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;
	viewer->textbox=ZG_TextBox_New();


	// SETUP DATA
	GUITextureData *data = ZG_NEW(GUITextureData);
	data->texture=NULL;
	viewer->data=data;

	ZG_TextBox_SetHorizontalAlignment(viewer->textbox,ZG_HORIZONTAL_ALIGNMENT_CENTER);
	ZG_TextBox_SetVerticalAlignment(viewer->textbox,ZG_VERTICAL_ALIGNMENT_CENTER);

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
	ZG_TextureManager *texture_manager=GUIWidget_GetTextureManager(_this->widget);

	if(texture_manager!=NULL){
		data->texture=ZG_TextureManager_Get(texture_manager,_source);
	}
}


ZG_Texture			*GUITexture_GetTexture(GUITexture *_this){
	GUITextureData *data=_this->data;
	return data->texture;
}


void GUITexture_Delete(GUITexture *_this){
	if(_this == NULL) return;
	GUITextureData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	ZG_TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
