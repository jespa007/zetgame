#include "zg_gui.h"


static void GUIViewer_DrawWidget(void *gui_texture);

typedef struct{
	Texture *texture;
}GUIViewerData;

GUIViewer *GUIViewer_New(int x, int y, uint16_t width, uint16_t height){
	GUIViewer *viewer = ZG_NEW(GUIViewer);

	// SETUP WIDGET
	viewer->widget=GUIWidget_New( x,  y,  width,  height);
	viewer->widget->type=WIDGET_TYPE_VIEWER;
	viewer->widget->gui_ptr=viewer;

	viewer->textbox=Textbox_New();


	// SETUP DATA
	GUIViewerData *data = ZG_NEW(GUIViewerData);
	data->texture = NULL;
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
	Transform transform=Transform_DefaultValues();

	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dim=GUIWidget_GetDimensions(_this->widget);

	Graphics_DrawRectangleTextured4i(position.x,position.y,dim.x,dim.y,COLOR4F_WHITE,_this->texture,NULL);

	Transform_SetPosition2i(&transform,position.x,position.y);
	Textbox_Draw(_this->textbox,&transform,&_this->widget->color);
}


void GUIViewer_Delete(GUIViewer *_this){
	if(_this == NULL) return;
	GUIViewerData *data=_this->data;

	GUIWidget_Delete(_this->widget);

	Textbox_Delete(_this->textbox);
	Texture_Delete(_this->texture);

	ZG_FREE(data);
	ZG_FREE(_this);
}
