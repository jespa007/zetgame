#include "zg_gui.h"


typedef struct{
	List *widgets;
	GUIWidget *parent;
	Vector2i position_local, position_world, position_screen;
	Vector2i dimensions;

	// events
	CallbackWidgetUpdate post_update,draw,update;
	CallbackWidgetOnSetDimension on_set_width;
	CallbackWidgetOnSetDimension on_set_height;
	CallbackWidgetAttachWidget attach_widget;
	TextBox				*	textbox; // only for label/button/textarea
	Texture				*texture; // label/textarea



}GUIWidgetData;

GUIWidget  * GUIWidget_New(int x, int y, uint16_t width, uint16_t height){
	GUIWidget *widget=NEW(GUIWidget);
	GUIWidgetData *data=NEW(GUIWidgetData);
	data->widgets = List_New();
	widget->data=data;

	widget->is_enabled=true;
	widget->color=COLOR4F_WHITE;
	widget->background_color=COLOR4F_WHITE;
	widget->opacity=ALPHA_VALUE_SOLID;

	data->position_local.x=x;
	data->position_local.y=y;

	data->dimensions.x=width;
	data->dimensions.y=height;

	return widget;
}

void GUIWidget_SetPositionX(GUIWidget *_this,int x){
	GUIWidgetData *data=_this->data;
	data->position_local.x=x;
}

void GUIWidget_SetPositionY(GUIWidget *_this,int y){
	GUIWidgetData *data=_this->data;
	data->position_local.y=y;
}

void GUIWidget_SetPosition2i(GUIWidget *_this,int x, int y){
	GUIWidget_SetPositionX(_this,x);
	GUIWidget_SetPositionY(_this,y);
}

void GUIWidget_SetPositionAbsolute2i(GUIWidget *_this,int x,int y){
	GUIWidgetData *data=_this->data;
	data->position_world.x=x;
	data->position_world.y=y;
}

void GUIWidget_SetWidth(GUIWidget *_this,uint16_t width){
	GUIWidgetData *data=_this->data;
	bool modify_width=data->dimensions.x != width;

	data->dimensions.x = width;

	if(modify_width && data->on_set_width.ptr_function!=NULL){
		data->on_set_width.ptr_function(data->on_set_width.calling_widget,width);
	}
}

void GUIWidget_SetHeight(GUIWidget *_this,uint16_t height){
	GUIWidgetData *data=_this->data;
	bool modify_height=data->dimensions.y != height;

	data->dimensions.y = height;

	if(modify_height && data->on_set_height.ptr_function!=NULL){
		data->on_set_height.ptr_function(data->on_set_height.calling_widget,height);
	}
}

void GUIWidget_SetDimensions(GUIWidget *_this,uint16_t width, uint16_t height){
	GUIWidget_SetHeight(_this,height);
	GUIWidget_SetWidth(_this,width);
}

Vector2i GUIWidget_GetDimensions(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->dimensions;
}

void GUIWidget_AttachWidgetBase(GUIWidget *_this, GUIWidget *widget_to_attach){

	GUIWidgetData *data=_this->data;
	GUIWidgetData *data_widget=NULL;

	if(widget_to_attach==NULL){
		Log_Error("widget null");
		return;
	}

	data_widget=widget_to_attach->data;

	if(data_widget->parent!=NULL){
		Log_Error("Widget is already added. Please detach first");
		return;
	}

	List_Add(data->widgets, widget_to_attach);

	// now widget has this parent
	data_widget->parent=_this;

}

void GUIWidget_AttachWidget(GUIWidget *_this, GUIWidget *widget_to_attach){

	GUIWidgetData *data=_this->data;

	if(data->attach_widget.ptr_function!=NULL){
		data->attach_widget.ptr_function(data->attach_widget.calling_widget,widget_to_attach);
	}else{
		GUIWidget_AttachWidgetBase(_this,widget_to_attach);
	}
}

void GUIWidget_AttachWidgetFunctionOverride(GUIWidget *_this,CallbackWidgetAttachWidget attach_widget){
	GUIWidgetData *data=_this->data;
	data->attach_widget=attach_widget;
}

void GUIWidget_SetDrawFunction(GUIWidget *_this,CallbackWidgetUpdate draw){
	GUIWidgetData *data=_this->data;
	data->draw=draw;
}

void GUIWidget_SetPostUpdateFunction(GUIWidget *_this,CallbackWidgetUpdate post_update){
	GUIWidgetData *data=_this->data;
	data->post_update=post_update;
}

void GUIWidget_UpdateFunctionOverride(GUIWidget *_this,CallbackWidgetUpdate update){
	GUIWidgetData *data=_this->data;
	data->update=update;
}

void GUIWidget_OnSetHeight(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_height){
	GUIWidgetData *data=_this->data;
	data->on_set_height=on_set_height;
}

void GUIWidget_OnSetWidth(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_width){
	GUIWidgetData *data=_this->data;
	data->on_set_width=on_set_width;
}


bool	GUIWidget_IsPointCollision(GUIWidget *_this,Vector2i point){
	GUIWidgetData *data=_this->data;
	return Vector2i_PointRectCollision(
				 point
				,data->position_screen
				,Vector2i_Add(data->position_screen,data->dimensions)
			);
}


Vector2i GUIWidget_GetPosition(GUIWidget *_this,WidgetPosition widget_pos){
	GUIWidgetData *data=_this->data;
	switch(widget_pos){
	case WIDGET_POSITION_WORLD:
		return data->position_world;
	case WIDGET_POSITION_SCREEN:
		return data->position_screen;
	default:
		break;
	}

	return data->position_local;
}


void GUIWidget_UpdateChilds(GUIWidget *_this){
	GUIWidgetData *data=_this->data;

	for(unsigned i = 0; i < data->widgets->count ; i++) {
		GUIWidget *widget=data->widgets->items[i];
		GUIWidget_Update(widget);
	}
}

void GUIWidget_UpdatePosition(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	data->position_world=data->position_local;
	data->position_screen=data->position_local;

	if(data->parent!=NULL){
		GUIWidgetData *parent_data=data->parent->data;
		data->position_world=Vector2i_Add(data->position_world,parent_data->position_world);
		data->position_screen=Vector2i_Add(data->position_screen,parent_data->position_screen);
	}
}

void GUIWidget_UpdateWidget(GUIWidget *_this){

	GUIWidgetData *data=_this->data;
	GUIWidget_UpdatePosition(_this);

	if(data->post_update.ptr_function!=NULL){
		data->post_update.ptr_function(data->post_update.calling_widget);
	}

	// add widget to render list...
	if(data->draw.ptr_function!=NULL){
		data->draw.ptr_function(data->draw.calling_widget);
	}


	GUIWidget_UpdateChilds(_this);

}

void GUIWidget_Update(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	if(data->update.ptr_function != NULL){
		data->update.ptr_function(data->update.calling_widget);
	}
	else{
		GUIWidget_UpdateWidget(_this);
	}
}

void GUIWidget_Delete(GUIWidget *_this){
	if(_this == NULL) return;

	GUIWidgetData *_data= _this->data;

	if(_data->textbox != NULL){
		Textbox_Delete(_this->textbox);
	}

	List_Delete(_data->widgets);
	FREE(_data);
	FREE(_this);
}


