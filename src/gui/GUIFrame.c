#include "zg_gui.h"

void GUIWidget_SetPositionAbsolute2i(GUIWidget *_this,int x,int y);
void GUIWidget_UpdatePosition(GUIWidget *_this);
void GUIWidget_UpdateChilds(GUIWidget *_this);
static void GUIFrame_Update(void *gui_frame);

GUIFrame * GUIFrame_New(int x, int y, uint16_t width, uint16_t height){
	GUIFrame * gui_frame=NEW(GUIFrame);
	gui_frame->widget=GUIWidget_New( x,  y,  width,  height);

	GUIWidget_UpdateFunctionOverride(gui_frame->widget
				,(CallbackWidgetUpdate){
					.ptr_function=GUIFrame_Update
					,.calling_widget=gui_frame
				}
		);


	return gui_frame;
}


void GUIFrame_Update(void * gui_frame){
	GUIFrame *_this=gui_frame;
	Vector2i position,dimensions;

	// update first position...
	GUIWidget_UpdatePosition(_this->widget);

	position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_SCREEN);
	dimensions=GUIWidget_GetDimensions(_this->widget);

	ViewPort_Push(position.x,position.y,dimensions.x,dimensions.y,PROJECTION_MODE_ORTHO);

	Graphics_ClearScreen(_this->widget->background_color);

	// reset absolute position due frame starts as a new screen...
	GUIWidget_SetPositionAbsolute2i(_this->widget,0,0);

	GUIWidget_UpdateChilds(_this->widget);

	ViewPort_Pop();

}


void GUIFrame_Delete(GUIFrame *_this){

	if(_this == NULL) return;

	GUIWidget_Delete(_this->widget);

	FREE(_this);

}
