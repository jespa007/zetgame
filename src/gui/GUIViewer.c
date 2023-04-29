#include "zg_gui.h"


static void GUIViewer_DrawWidget(void *gui_texture);

typedef struct{
	Texture 	*texture;
	TextBox 	*textbox;
}GUIViewerData;

GUIViewer *GUIViewer_New(int x, int y, uint16_t width, uint16_t height){
	GUIViewer *viewer = ZG_NEW(GUIViewer);

	// SETUP WIDGET
	viewer->widget=GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;


	// SETUP DATA
	GUIViewerData *data = ZG_NEW(GUIViewerData);
	data->texture = NULL;
	data->textbox=TextBox_New();
	viewer->data=data;

	GUIWidget_SetDrawFunction(viewer->widget
			,(CallbackWidgetUpdate){
				.ptr_function=GUIViewer_DrawWidget
				,.calling_widget=viewer
			}
	);

	return viewer;
}

static void GUIViewer_DrawWidget(void *gui_texture){
	GUIViewer *_this=gui_texture;
	GUIViewerData *data=_this->data;
	Transform transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	if(position.y==50){
		int kk=0;
	}

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,COLOR4F_WHITE,data->texture,NULL);

	Transform_SetPosition2i(&transform,position.x,position.y);
	TextBox_Draw(data->textbox,&transform,&_this->widget->color);
}

void		GUIViewer_SetImage(GUIViewer *_this, const char *_image){
	GUIViewerData *data=_this->data;
	TextureManager *texture_manager=GUIWidget_GetTextureManager(_this->widget);
	Texture *texture=TextureManager_Get(texture_manager,_image);
	if(texture!=NULL){
		data->texture=texture;
	}
}


void GUIViewer_Delete(GUIViewer *_this){
	if(_this == NULL) return;
	GUIViewerData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	TextBox_Delete(data->textbox);

	ZG_FREE(data);
	ZG_FREE(_this);
}
