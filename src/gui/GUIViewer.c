#include "zg_gui.h"


static void GUIViewer_DrawWidget(void *gui_texture);

typedef struct{
	Texture 	*texture;
}GUIViewerData;


static void GUIViewer_DrawWidget(void *gui_texture){
	GUIViewer *_this=gui_texture;
	GUIViewerData *data=_this->data;
	Transform transform=Transform_DefaultValues();
	Color4f color=GUIWidget_GetColor4f(_this->widget);

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,COLOR4F_WHITE,data->texture,NULL);

	Transform_SetPosition2i(&transform,position.x,position.y);
	TextBox_Draw(_this->textbox,&transform,&color);
}

GUIViewer *GUIViewer_New(int x, int y, uint16_t width, uint16_t height){
	GUIViewer *viewer = ZG_NEW(GUIViewer);

	// SETUP WIDGET
	viewer->widget=GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;
	viewer->textbox=TextBox_New();


	// SETUP DATA
	GUIViewerData *data = ZG_NEW(GUIViewerData);
	data->texture = NULL;
	viewer->data=data;

	TextBox_SetHorizontalAlignment(viewer->textbox,HORIZONTAL_ALIGNMENT_CENTER);
	TextBox_SetVerticalAlignment(viewer->textbox,VERTICAL_ALIGNMENT_CENTER);

	GUIWidget_SetDrawFunction(viewer->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUIViewer_DrawWidget
				,.calling_widget=viewer
			}
	);

	return viewer;
}

void 			GUIViewer_SetHeight(GUIViewer *_this,uint16_t _height){
	GUIWidget_SetHeight(_this->widget,_height);
	TextBox_SetHeight(_this->textbox,_height);
}

void 			GUIViewer_SetWidth(GUIViewer *_this,uint16_t _width){
	GUIWidget_SetWidth(_this->widget,_width);
	TextBox_SetWidth(_this->textbox,_width);
}

void			GUIViewer_SetTexture(GUIViewer *_this, const char *_texture_id){
	GUIViewerData *data=_this->data;
	TextureManager *texture_manager=GUIWidget_GetTextureManager(_this->widget);
	Texture *texture=TextureManager_Get(texture_manager,_texture_id);
	if(texture!=NULL){
		data->texture=texture;
	}
}

void GUIViewer_Delete(GUIViewer *_this){
	if(_this == NULL) return;
	GUIViewerData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	TextBox_Delete(_this->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
