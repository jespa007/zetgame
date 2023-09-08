#include "@zg_gui.h"

void ZG_GUIWidget_SetPositionAbsolute2i(ZG_GUIWidget *_this,int x,int y);
void ZG_GUIWidget_UpdatePosition(ZG_GUIWidget *_this);
void ZG_GUIWidget_UpdateChilds(ZG_GUIWidget *_this);
static void ZG_GUIFrame_Update(void *gui_frame);

ZG_GUIFrame * ZG_GUIFrame_New(int x, int y, uint16_t width, uint16_t height){
	ZG_GUIFrame * gui_frame=ZG_NEW(ZG_GUIFrame);
	gui_frame->widget=ZG_GUIWidget_New( x,  y,  width,  height);
	gui_frame->widget->type=ZG_GUI_WIDGET_TYPE_FRAME;
	gui_frame->widget->gui_ptr=gui_frame;

	ZG_GUIWidget_UpdateFunctionOverride(gui_frame->widget
				,(ZG_CallbackWidgetUpdate){
					.ptr_function=ZG_GUIFrame_Update
					,.calling_widget=gui_frame
				}
		);


	return gui_frame;
}


void ZG_GUIFrame_Update(void * gui_frame){
	ZG_GUIFrame *_this=gui_frame;
	ZG_Vector2i position,dimensions;

	// update first position...
	ZG_GUIWidget_UpdatePosition(_this->widget);

	position=ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_POSITION_SCREEN);
	dimensions=ZG_GUIWidget_GetDimensions(_this->widget);

	ZG_ViewPort_Push(position.x,position.y,dimensions.x,dimensions.y,ZG_PROJECTION_MODE_ORTHO);

	ZG_Graphics_ClearScreen(ZG_GUIWidget_GetBackgroundColor4f(_this->widget));

	// reset absolute position due frame starts as a new screen...
	ZG_GUIWidget_SetPositionAbsolute2i(_this->widget,0,0);

	ZG_GUIWidget_UpdateChilds(_this->widget);

	ZG_ViewPort_Pop();

}


void ZG_GUIFrame_Delete(ZG_GUIFrame *_this){

	if(_this == NULL) return;

	ZG_GUIWidget_Delete(_this->widget);

	ZG_FREE(_this);

}
