#include "@zg_gui.h"


static void ZG_GUITexture_DrawWidget(void *gui_texture);

typedef struct{
	ZG_Texture 	*texture;
}ZG_GUITextureData;


static void ZG_GUITexture_DrawWidget(void *gui_texture){
	ZG_GUITexture *_this=gui_texture;
	ZG_GUITextureData *data=_this->data;
	ZG_Transform transform=ZG_Transform_DefaultValues();
	ZG_Color4f color=ZG_GUIWidget_GetColor4f(_this->widget);

	ZG_Vector2i position=ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=ZG_GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	if(data->texture!=NULL){
		ZG_Graphics_DrawTexturedRectangle4i(position.x,position.y,dimensions.x,dimensions.y,ZG_COLOR4F_WHITE,data->texture,NULL);
	}

	ZG_Transform_SetPosition2i(&transform,position.x,position.y);
	ZG_TextBox_Draw(_this->textbox,&transform,&color);
}

ZG_GUITexture *ZG_GUITexture_New(int x, int y, uint16_t width, uint16_t height){
	ZG_GUITexture *viewer = ZG_NEW(ZG_GUITexture);

	// SETUP WIDGET
	viewer->widget=ZG_GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=ZG_GUI_WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;
	viewer->textbox=ZG_TextBox_New();


	// SETUP DATA
	ZG_GUITextureData *data = ZG_NEW(ZG_GUITextureData);
	data->texture=NULL;
	viewer->data=data;

	ZG_TextBox_SetHorizontalAlignment(viewer->textbox,ZG_HORIZONTAL_ALIGNMENT_CENTER);
	ZG_TextBox_SetVerticalAlignment(viewer->textbox,ZG_VERTICAL_ALIGNMENT_CENTER);

	ZG_GUIWidget_SetDrawFunction(viewer->widget
			,(ZG_CallbackWidgetUpdate){
				.ptr_function=ZG_GUITexture_DrawWidget
				,.calling_widget=viewer
			}
	);

	return viewer;
}

void			ZG_GUITexture_SetTexture(ZG_GUITexture *_this, const char *_source){
	ZG_GUITextureData *data=_this->data;
	ZG_TextureManager *texture_manager=ZG_GUIWidget_GetTextureManager(_this->widget);

	if(texture_manager!=NULL){
		data->texture=ZG_TextureManager_Get(texture_manager,_source);
	}
}


ZG_Texture			*ZG_GUITexture_GetTexture(ZG_GUITexture *_this){
	ZG_GUITextureData *data=_this->data;
	return data->texture;
}


void ZG_GUITexture_Delete(ZG_GUITexture *_this){
	if(_this == NULL) return;
	ZG_GUITextureData *data=_this->data;

	ZG_GUIWidget_Delete(_this->widget);
	ZG_TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
